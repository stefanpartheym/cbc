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


                /* keywords */
"print"         { return PRINT; }
"if"            { return IF; }
"then"          { return THEN; }
"else"          { return ELSE; }
"endif"         { return ENDIF; }
"while"         { return WHILE; }
"do"            { return DO; }
"end"           { return END; }
"for"           { return FOR; }
"to"            { return TO; }
"next"          { return NEXT; }

                /* logical gates */
"not"           { return LOGICAL_NOT; }
"and"           { return LOGICAL_AND; }
"or"            { return LOGICAL_OR; }

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
                    size_t size = yyleng - 1;
                    char* str   = (char*) malloc(size);
                    memclr(str, size);

                    /* omit first and last char, which are both quotes */
                    int offset   = 1;
                    yytext[size] = '\0';

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
                        strncpy(str, (yytext + offset), size - offset);

                    yylval.string_val = str;
                    return STRING;
                }

                /* identifiers */
[_a-zA-Z][_a-zA-Z0-9]* {
                    yylval.identifier = strdup(yytext);
                    return IDENTIFIER;
                }

                /* single character operators */
[-+*/|,()]      { return *yytext; }

                /* comparison operators */
"="             { return COMPARISON_EQ; }
">"             { return COMPARISON_GT; }
"<"             { return COMPARISON_LT; }
"=="            { return COMPARISON_SE; } /* strict equality operator */
">="            { return COMPARISON_GE; }
"<="            { return COMPARISON_LE; }
"<>"            { return COMPARISON_NE; }

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
