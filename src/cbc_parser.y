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
#include "ast_assignment.h"
#include "ast_declaration.h"
#include "ast_declaration_block.h"
#include "ast_control_flow.h"
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
    CbBooleanDataType boolean_val;
    CbStringDataType  string_val;
};

%token <identifier>  IDENTIFIER
%token <integer_val> INTEGER
%token <float_val>   FLOAT
%token <boolean_val> BOOLEAN
%token <string_val>  STRING
%token               IF THEN ELSE ENDIF
                     WHILE DO END
                     FOR TO NEXT
%token               ENDOFFILE

%right    ASSIGNMENT
%left     LOGICAL_AND LOGICAL_OR
%nonassoc LOGICAL_NOT
%left     COMPARISON_EQ COMPARISON_GT COMPARISON_LT COMPARISON_SE COMPARISON_GE
          COMPARISON_LE COMPARISON_NE
%left     '+' '-'
%left     '*' '/'

%type <ast> expression
            statement statement_list
            var_declaration var_declaration_block var_declaration_list
            var_access

/* Output parameter: The AST of the parsed codeblock */
%parse-param {CbAstNode** result_ast}
%error-verbose

/* Destructors: Destroy discarded symbols in case of errors */
%destructor {
    cb_ast_node_destroy($$);
} expression statement statement_list var_declaration var_access

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
    var_declaration_list statement_list {
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
    | IF expression THEN statement_list ENDIF {
                            $$ = (CbAstNode*) cb_ast_if_node_create(
                                $2, $4, NULL
                            );
                            /*
                             * TODO: It might be neccessary to push the line
                             *       number at the opening keyword "if".
                             *       Otherwise yylineno will correspond to the
                             *       "endif"s line number.
                             */
                            cb_ast_node_set_line($$, yylineno);
                        }
    | IF expression THEN statement_list ELSE statement_list ENDIF {
                            $$ = (CbAstNode*) cb_ast_if_node_create(
                                $2, $4, $6
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | WHILE expression DO statement_list END {
                            $$ = (CbAstNode*) cb_ast_while_node_create($2, $4);
                            cb_ast_node_set_line($$, yylineno);
                        }
    | FOR expression TO expression DO statement_list NEXT {
                            $$ = (CbAstNode*) cb_ast_for_node_create($2, $4, $6);
                            cb_ast_node_set_line($$, yylineno);
                        }
    ;

var_declaration_list:
    '|' var_declaration_block '|'
                        { $$ = $2; }
    ;

var_declaration_block:
    var_declaration     {
                            $$ = (CbAstNode*) cb_ast_declaration_block_node_create();
                            cb_ast_node_set_line($$, yylineno);
                            cb_ast_declaration_block_node_add(
                                (CbAstDeclarationBlockNode*) $$,
                                (CbAstDeclarationNode*) $1
                            );
                        }
    | var_declaration_block ',' var_declaration {
                            cb_ast_declaration_block_node_add(
                                (CbAstDeclarationBlockNode*) $1,
                                (CbAstDeclarationNode*) $3
                            );
                            $$ = $1;
                        }
    ;

var_declaration:
    IDENTIFIER          {
                            $$ = (CbAstNode*) cb_ast_declaration_node_create(
                                CB_AST_DECLARATION_TYPE_VARIABLE,
                                $1
                            );
                            cb_ast_node_set_line($$, yylineno);
                            memfree($1); /* free duplicated string */
                        }
    ;

var_access:
    IDENTIFIER          {
                            $$ = (CbAstNode*) cb_ast_variable_node_create($1);
                            cb_ast_node_set_line($$, yylineno);
                            memfree($1); /* free duplicated string */
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
    | BOOLEAN           {
                            CbVariant* value = cb_boolean_create($1);
                            $$ = (CbAstNode*) cb_ast_value_node_create(value);
                            cb_ast_node_set_line($$, yylineno);
                            cb_variant_destroy(value);
                        }
    | STRING            {
                            CbVariant* value = cb_string_create($1);
                            memfree($1); /* free duplicated string */
                            $$ = (CbAstNode*) cb_ast_value_node_create(value);
                            cb_ast_node_set_line($$, yylineno);
                            cb_variant_destroy(value);
                        }
    | var_access        {
                            $$ = $1;
                        }
    | var_access ASSIGNMENT expression {
                            $$ = (CbAstNode*) cb_ast_assignment_node_create(
                                $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_EQ expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_EQ, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_GT expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_GT, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_LT expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_LT, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_SE expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_SE, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_GE expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_GE, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_LE expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_LE, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression COMPARISON_NE expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_COMPARISON_NE, $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
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
    | expression LOGICAL_AND expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_LOGICAL_AND,
                                $1, $3
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | expression LOGICAL_OR expression {
                            $$ = (CbAstNode*) cb_ast_binary_node_create(
                                CB_BINARY_OPERATOR_TYPE_LOGICAL_OR,
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
    | LOGICAL_NOT expression {
                            $$ = (CbAstNode*) cb_ast_unary_node_create(
                                CB_UNARY_OPERATOR_TYPE_LOGICAL_NOT,
                                $2
                            );
                            cb_ast_node_set_line($$, yylineno);
                        }
    | '(' expression ')' {  $$ = $2; }
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
