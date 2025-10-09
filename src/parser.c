
#include "parser.h"
#include <stdio.h>
#include <stdbool.h>

// Global variables
Token current_token;
FILE *file_out;

// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void system_goal(void)
{
    /* <system_goal> ::= <program> SCANEOF */
    program();
    match(SCANEOF);
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
// D'une façon similaire on a la procédure suivante pour program()
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void program(void)
{
    /* <program> ::= begin <inst_list> end */
    match(BEGIN);
    inst_list();
    match(END);
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void inst_list(void)
{
    // BEGIN
    /* <inst_list> ::= <inst> { <inst> } */
    inst();
    while (current_token == SEMICOLON)
        inst();
    // END
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void inst(void)
{
    Token tok = next_token();
    switch (tok)
    {
    case ID:
        /* <inst> ::= ID := <expr>; */
        match(ID);
        match(ASSIGN_OP);
        expression();
        match(SEMICOLON);
        break;
    case READ:
        /* <inst> ::= READ ( <id list> ); */
        match(READ);
        match(L_PAREN);
        id_list();
        match(R_PAREN);
        match(SEMICOLON);
        break;
    case WRITE:
        /* <inst> ::= WRITE ( <expr list> ); */
        match(WRITE);
        match(L_PAREN);
        expr_list();
        match(R_PAREN);
        match(SEMICOLON);
        break;
        // default:
        //     syntax_error(tok);
        //     break;
    }
}
/*
next_token();
return current_token;
*/
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void id_list(void)
{
    /* <id list> ::= ID { , ID } */
    match(ID);
    while (next_token() == COMMA)
    {
        match(COMMA);
        match(ID);
    }
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void expr_list(void)
{
    /* <expr list> ::= <expr> { , <expr> } */
    expression();
    while (next_token() == COMMA)
    {
        match(COMMA);
        expression();
    }
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void expression(void)
{
    add_opp();
    Token token = next_token();
    while (token == PLUS_OP || token == MINUS_OP)
    {
        add_opp();
        token = next_token();
    }
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void add_opp(void)
{
    /* <add op> ::= + | - */
    Token token = next_token();
    if (token == PLUS_OP || token == MINUS_OP)
        match(token);
    else
        syntax_error(token);
}
// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == =
void factor(void)
{
    Token token = next_token();
    switch (token)
    {
    case ID:
        /* <factor> ::= ID */
        match(ID);
        break;
    case INT_LITERAL:
        /* <factor> ::= INTLITERAL */
        match(INT_LITERAL);
        break;
    case L_PAREN:
        /* <factor> ::= ( <expr> ) */
        match(L_PAREN);
        expression();
        match(R_PAREN);
        break;
    default:
        syntax_error(token);
        break;
    }
}

Token next_token()
{
    int token_value;
    if (fscanf(file_out, "%d ", &token_value) == 1)
    {
        return (Token)token_value;
    }
    else
    {
        return SCANEOF; // Return end-of-file token if no more tokens can be read
    }
}

void match(Token t)
{
    current_token = next_token();
    if (current_token != t)
    {
        syntax_error(t);
    }
}

void syntax_error(Token t)
{
    printf("{Error}: %d", t);
}