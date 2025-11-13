#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern FILE *file;
extern FILE *file_out;

void init_parser(FILE *input_file, FILE *output_file);
void system_goal(void);
void program(void);
void inst_list(void);
void inst(void);
void id_list(void);
void expression(void);
void expr_list(void);
void add_op(void);
void prim(void);
void match(Token t);
void syntax_error(Token t, Token expected_token);
Token next_token();

#endif
