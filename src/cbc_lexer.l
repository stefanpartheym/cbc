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
