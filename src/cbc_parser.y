    /* DEFINITIONS ---------------------------------------------------------- */
%code requires {

#include <stdio.h>
#include <stdarg.h>
#include "utils.h"
#include "variant.h"
#include "ast.h"
#include "ast_value.h"
#include "ast_unary.h"
#include "ast_binary.h"
#include "ast_variable.h"
#include "ast_declaration.h"
#include "ast_statement_list.h"


extern int yychar;
extern int yylineno;

extern int yylex();
void yyerror(void* data, const char* format, ...);

}

%{

%}

%union {
    CbAstNode*        ast;
    char*             identifier;
    CbIntegerDataType integer_val;
    CbFloatDataType   float_val;
};

%token               ENDOFFILE
%token <identifier>  IDENTIFIER
%token <integer_val> INTEGER
%token <float_val>   FLOAT

%left  '+' '-'
%left  '*' '/'

%type <ast> expression statement statement_list var_declaration

/* Output parameter: The AST of the parsed codeblock */
%parse-param {CbAstNode** result_ast}
%error-verbose

/* Destructors: Destroy discarded symbols in case of errors */
%destructor {
    cb_ast_node_destroy($$);
} expression statement statement_list var_declaration

/*
 * TODO: The following destructor might be useless or even wrong, since the 
 *       identifier string is already freed right after usage.
 *       (see expression rule for IDENTIFIER)
 */
%destructor {
    free($$);
} IDENTIFIER


%%  /* RULES ---------------------------------------------------------------- */


program:
    statement_list ENDOFFILE {
                            *result_ast = $1;
                            YYACCEPT;
                        }
    ;

statement_list:
    var_declaration statement_list {
                            if ($2 == NULL)
                                $$ = $1;
                            else
                                $$ = cb_ast_statement_list_node_create($1, $2);
                            /* do not set line number for this node */
                        }
    | statement ',' statement_list {
                            if ($3 == NULL)
                                $$ = $1;
                            else
                                $$ = cb_ast_statement_list_node_create($1, $3);
                            /* do not set line number for this node */
                        }
    |                   { $$ = NULL; }
    ;
/*
 * Alternative statement list syntax:
 */
/*
    statement           { $$ = $1; }
    | statement_list ',' statement {
                            $$ = cb_ast_statement_list_node_create($1, $3);
                        }
    ;
*/

statement:
    expression          { $$ = $1; }
    ;

var_declaration:
    '|' IDENTIFIER '|'  {
                            $$ = (CbAstNode*) cb_ast_declaration_node_create(
                                CB_AST_DECLARATION_TYPE_VARIABLE,
                                $2
                            );
                            memfree($2); /* free duplicated string */
                            cb_ast_node_set_line($$, yylineno);
                        }
    ;

expression:
    INTEGER             {
                            CbVariant* value = cb_integer_create($1);
                            $$ = (CbAstNode*) cb_ast_value_node_create(value);
                            cb_ast_node_set_line($$, yylineno);
                            cb_variant_destroy(value);
                        }
    | FLOAT             {
                            CbVariant* value = cb_float_create($1);
                            $$ = (CbAstNode*) cb_ast_value_node_create(value);
                            cb_ast_node_set_line($$, yylineno);
                            cb_variant_destroy(value);
                        }
    | IDENTIFIER        {
                            $$ = (CbAstNode*) cb_ast_variable_node_create($1);
                            cb_ast_node_set_line($$, yylineno);
                            memfree($1); /* free duplicated string */
                        }
    | expression '+' expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_ADD,
                                $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression '-' expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_SUB,
                                $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression '*' expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_MUL,
                                $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression '/' expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_DIV,
                                $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | '-' expression    {
                            $$ = (CbAstNode*) cb_ast_unary_node_create(
                                CB_UNARY_OPERATOR_TYPE_MINUS,
                                $2
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    ;


%%  /* ROUTINES ------------------------------------------------------------- */


void yyerror(void* data, const char* format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    vfprintf(stderr, format, arglist);
    fprintf(stderr, "\n");
    va_end(arglist);
}
