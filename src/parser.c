#include "../include/parser.h"
#include "../include/cli_style.h"
#include "../include/globals.h"

const char *token_names[] = {
    "BEGIN", "END", "READ", "WRITE", "ID", "INT_LITERAL", "FLOAT_LITERAL",
    "CURLY_BRACE_OPEN", "CURLY_BRACE_CLOSE", "SCAN_OF", "PLUS_OP", "MINUS_OP",
    "L_PAREN", "R_PAREN", "SEMICOLON", "COMMA", "ASSIGN_OP"};

FILE *file_out;
FILE *file;
Token current_token;

void init_parser(FILE *input_file, FILE *output_file)
{
    file = input_file;
    file_out = output_file;
}

void system_goal(void)
{
    /*<system goal> :: <program SCAN_EOF >*/
    current_token = scanner(file);
    program();
    match(SCAN_OF);
}

void program(void)
{
    /* <program> :: begin <inst_list end>*/
    match(BEGIN);
    inst_list();
    match(END);
}

void inst_list(void)
{
    /*<inst_list> ::= <inst> {<inst>} */
    // inst();
    while (true)
    {
        switch (next_token())
        {
        case ID:
        case READ:
        case WRITE:
            inst();
            break;
        default:
            return;
        }
    }
}

void inst(void)
{
    Token tok = next_token();
    switch (tok)
    {
    case ID:
        /*<inst>	::= ID :=<expr>	*/
        match(ID);
        match(ASSIGN_OP);
        expression();
        match(SEMICOLON);
        break;
    case READ:
        match(READ);
        match(L_PAREN);
        id_list();
        match(R_PAREN);
        match(SEMICOLON);
        break;
    case WRITE:
        /* <inst>	::= WRITE*/
        match(WRITE);
        match(L_PAREN);
        expr_list();
        match(R_PAREN);
        match(SEMICOLON);
        break;
    default:
        syntax_error(tok,ID);
        break;
    }
}
void id_list(void)
{
    /* <id_list> ::= ID {, ID}	*/
    match(ID);
    while (next_token() == COMMA)
    {
        match(COMMA);
        match(ID);
    }
}
void expression(void)
{
    Token t;

    /*<expr> ::= <prim> {<addop>	<prim>}	*/
    prim();
    for (t = next_token(); t == PLUS_OP || t == MINUS_OP; t = next_token())
    {
        add_op();
        prim();
    }
}
void expr_list(void)
{
    /* <expr_list>	::= <expr> {,<expr>} */
    expression();
    while (next_token() == COMMA)
    {
        match(COMMA);
        expression();
    }
}
void add_op(void)
{
    Token t = next_token();
    /* <add_op> ::= PLUS_OP || MINUS_OP*/
    if (t == PLUS_OP || t == MINUS_OP)
        match(t);
    else
        syntax_error(t,PLUS_OP);
}
void prim(void)
{
    Token tok = next_token();
    switch (tok)
    {
    case L_PAREN:
        /* <prim ::= (<expr>)>	*/
        match(L_PAREN);
        expression();
        match(R_PAREN);
        break;
    case ID:
        /* <prim ::= ID>*/
        match(ID);
        break;
    case INT_LITERAL:
        /*<prim> ::= INTLITERAL */
        match(INT_LITERAL);
        break;
    case FLOAT_LITERAL:
        /*<prim> ::= FLOAT_LITERAL */
        match(FLOAT_LITERAL);
        break;
    default:
        syntax_error(tok,L_PAREN);
        break;
    }
}

Token next_token(void)
{
    return current_token;
}

void match(Token expected_token)
{
    if (current_token == expected_token)
    {
        current_token = scanner(file);
    }
    else
    {
        syntax_error(current_token,expected_token);
    }
}

void syntax_error(Token token, Token expected_token)
{

    printf(FG_RED UNDERLINE BOLD "Syntax Error" FG_MAGENTA
        "[%d]" RESET ": Unexpected token"
        FG_CYAN "'%s' " RESET
        "expected token " FG_CYAN
        "'%s' \n" RESET
        , line_n,
        token_names[token],
        token_names[expected_token]);
    // exit(EXIT_FAILURE);
}