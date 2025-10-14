#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEME_LEN 64

typedef enum TokenTyped
{
    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INT_LITERAL,
    FLOAT_LITERAL,
    CURLY_BRACE_OPEN,
    CURLY_BRACE_CLOSE,
    SCAN_OF,
    PLUS_OP,
    MINUS_OP,
    L_PAREN,
    R_PAREN,
    SEMICOLON,
    COMMA,
    ASSIGN_OP,
} Token;

void clear_buffer(void);

void buffer_char(int c);

Token check_reserved(char *token);

void lexical_error(char errorStr[], int errorLine);

Token scanner(FILE *file);

void run_scanner(FILE *in_file, FILE *out_file);

#endif