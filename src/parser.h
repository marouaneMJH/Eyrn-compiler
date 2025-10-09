#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

void system_goal(void);
void program(void);
void inst_list(void);
void inst(void);
void id_list(void);
void expression(void);
void expr_list(void);
void add_opp(void);
void prim(void);
void match(Token t);
void syntax_error(Token t);
Token next_token();

#endif
