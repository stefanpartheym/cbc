    /* DEFINITIONS ---------------------------------------------------------- */


%option yylineno
%option nounput
%option noinput
%option never-interactive

%{

#include <stdio.h>
#include "utils.h"
#include "cbc_parser.h"

%}


%%  /* RULES ---------------------------------------------------------------- */


                /* integers */
[0-9]+          {
                    yylval.integer_val = atoi(yytext);
                    return INTEGER;
                }

                /* floats */
[0-9]*\.?[0-9]+ {
                    yylval.float_val = atof(yytext);
                    return FLOAT;
                }

                /* booleans */
"True"|"False"  {
                    yylval.boolean_val = strequ("True", yytext);
                    return BOOLEAN;
                }
                /* strings */
(\'([^']|'')*\')|(\"[^"]*\") {
                    /*
                     * String literals are always in one line only.
                     * An apostrophe can be escaped by writing a double
                     * apostrophe.
                     */
                    char* str = (char*) malloc(yyleng - 1);
                    *str      = '\0'; /* terminate string */
                    
                    /* omit first and last char, which are both quotes */
                    int offset         = 1;
                    yytext[yyleng - 1] = '\0';
                    
                    if (yytext[0] == '\'')
                    {
                        /*
                         * First char is a single quote:
                         * Use extended string quotation logic.
                         * -> Omit all occourences of double apostrophes and
                         *    replace them with a single one.
                         */
                        int i          = 1;
                        char last_char = 0;
                        while (yytext[i] != '\0')
                        {
                            if (last_char == '\'')
                            {
                                offset++;
                                last_char = 0;
                            }
                            else
                            {
                                last_char       = yytext[i];
                                str[i - offset] = last_char;
                            }
                            
                            i++;
                        }
                    }
                    else
                        strncpy(str, (yytext + offset), yyleng - (1 + offset));
                    
                    yylval.string_val = str;
                    return STRING;
                }

                /* identifiers */
[_a-zA-Z][_a-zA-Z0-9]* {
                    yylval.identifier = strdup(yytext);
                    return IDENTIFIER;
                }

                /* single character operators */
[-+*/|,]        { return *yytext; }

                /* assignment operator */
":="            { return ASSIGNMENT; }

                /* end of file */
<<EOF>>         { return ENDOFFILE; }

                /* comments */
"//".*          { ; } /* ignore */

                /* whitespaces */
[ \t\n]+        { ; } /* ignore */

                /* anything else */
.               {
                    yyerror(NULL, "Unexpected character `%c'", *yytext);
                }


%%  /* ROUTINES ------------------------------------------------------------- */

int yywrap(void)
{
    return 1;
}
