#include <stdio.h>

#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "scanner.h"

char token_buffer[MAX_LEXEME_LEN];

// /* Function prototypes */

int buf_index = 0;
int line_n = 1;
int err_char = 0;

void lexical_error(char errorStr[], int errorLine)

{
    printf(FG_RED "[SyntaxError1]: " FG_MAGENTA " % s " RESET " line N * " FG_GREEN " % d\n " RESET,
           errorStr,
           errorLine);
}

void clear_buffer(void)
{
    for (int i = 0; i < MAX_LEXEME_LEN; i++)
        token_buffer[i] = '\0';

    buf_index = 0;
}

void buffer_char(int c)
{
    if (buf_index < MAX_LEXEME_LEN - 1)
    {
        token_buffer[buf_index++] = (char)c;
        token_buffer[buf_index] = '\0';
    }
}

Token check_reserved(char *token)

{

    if (!token)
    {
        lexical_error("[MEMORY_CRITICAL] The given token is null", line_n);
        exit(EXIT_FAILURE);
    }
    if (strcmp(token, "BEGIN") == 0)
        return BEGIN;
    if (strcmp(token, "END") == 0)
        return END;
    if (strcmp(token, "READ") == 0)
        return READ;
    if (strcmp(token, "WRITE") == 0)
        return WRITE;
    return ID;
}

// return single token
Token scanner(FILE *file)

{
    int in_char,
        c;
    clear_buffer();

    if (feof(file))
    {
        return SCAN_OF;
    }

    while ((in_char = getc(file)) != EOF)
    {
        if (isspace(in_char))
        {

            if (in_char == '\n')
            {
                ++line_n;
                return scanner(file);
            }
            continue; /* skip whitespace */
        }

        else if (isalpha(in_char))
        {

            /* identifier: ID ::= LETTER (LETTER | DIGIT | '_')* */
            buffer_char(in_char);
            for (c = getc(file); isalnum(c) || c == '_'; c = getc(file))
                buffer_char(c);
            ungetc(c, file);
            return check_reserved(token_buffer);
        }
        else if (isdigit(in_char))
        {
            /* number literal: can be INT_LITERAL or FLOAT_LITERAL */
            buffer_char(in_char);
            bool is_float = false;

            /* read digits before decimal point */
            for (c = getc(file); isdigit(c); c = getc(file))
                buffer_char(c);

            /* check for decimal point */
            if (c == '.')
            {
                is_float = true;
                buffer_char(c);
                c = getc(file);

                /* read digits after decimal point */
                if (isdigit(c))
                {
                    buffer_char(c);
                    for (c = getc(file); isdigit(c); c = getc(file))
                        buffer_char(c);
                }
            }

            ungetc(c, file);
            return is_float ? FLOAT_LITERAL : INT_LITERAL;
        }
        // Case where the float only start with .
        else if (in_char == '.')
        {
            /* handle numbers starting with decimal point like .123 */
            c = getc(file);
            if (isdigit(c))
            {
                buffer_char(in_char); /* buffer the '.' */
                buffer_char(c);

                /* read remaining digits */
                for (c = getc(file); isdigit(c); c = getc(file))
                    buffer_char(c);

                ungetc(c, file);
                return FLOAT_LITERAL;
            }
            else
            {
                /* not a float, put back the character and handle '.' as unknown */
                ungetc(c, file);
                char errorBuffer[100];
                snprintf(errorBuffer, sizeof(errorBuffer), "Invalid syntax with '%c'", in_char);
                lexical_error(errorBuffer, line_n);
            }
        }

        else if (in_char == '(')
        {
            return L_PAREN;
        }
        else if (in_char == ')')
        {
            return R_PAREN;
        }
        else if (in_char == ';')
        {
            return SEMICOLON;
        }
        else if (in_char == ',')
        {
            return COMMA;
        }
        else if (in_char == '+')
        {
            return PLUS_OP;
        }
        else if (in_char == '-')
        {
            return MINUS_OP;
        }
        else if (in_char == ':')
        {

            /* look for assignment := */
            c = getc(file);
            if (c == '=')
                return ASSIGN_OP;
            else
            {
                ungetc(c, file);
                lexical_error("maybe u meant \':=\' instead of \':\'?", line_n);
            }
        }
        else if (in_char == '{')
            return CURLY_BRACE_OPEN;

        else if (in_char == '}')
            return CURLY_BRACE_CLOSE;
        // comment ignoring
        else if (in_char == '/')
        {
            if (getc(file) == '/')
                for (char c = getc(file); c != EOF && c != '\n'; c = getc(file))
                    ;

            else
                ungetc(c, file);
        }

        else
        {
            char errorBuffer[100];
            snprintf(errorBuffer, sizeof(errorBuffer), "Invalid syntax with '%c'", in_char);

            lexical_error(errorBuffer, line_n);
        }
    }
    printf("EOF reached\n");
    return SCAN_OF;
}

void runScanner(FILE *in_file, FILE *ou_file)
{
    Token token;
    while (true)
    {
        token = scanner(in_file);
        fprintf(ou_file, "%d ", token);
        if (token == SCAN_OF)
            break;
    }
}
