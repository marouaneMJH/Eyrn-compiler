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

// todo:
// bool check_comment(FILE *file);
// bool handle_comment(FILE *file);
void lexical_error(char errorStr[], int errorLine)

{
    printf(FG_RED "[SyntaxError]: " RESET " Lexical Error with" FG_RED "%s" RESET " on the line N*" FG_GREEN "%d\n" RESET,
           errorStr, errorLine);
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
        return SCANEOF;
    }

    while ((in_char = getc(file)) != EOF)
    {
        if (isspace(in_char))
            continue; /* skip whitespace */

        else if (isalpha(in_char))
        {
            /* identifier: ID ::= LETTER (LETTER | DIGIT | '_')* */
            buffer_char(in_char);
            for (c = getc(file); isalnum(c) || c == '_'; c = getc(file))
                buffer_char(c);
            ungetc(c, stdin);
            return check_reserved(token_buffer);
        }
        else if (isdigit(in_char))
        {
            /* integer literal: INTLITERAL ::= DIGIT+ */
            buffer_char(in_char);
            for (c = getc(file); isdigit(c); c = getc(file))
                buffer_char(c);
            ungetc(c, stdin);
            return INT_LITERAL;
        }
        else if (in_char == '\n')
        {
            ++line_n;

            return scanner(file);
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
                ungetc(c, stdin);
                lexical_error("Maybe u meant \':=\' instead of \':\'?", line_n);
            }
        }
        else if (in_char == '{')
        {
            /* comment start: skip until '}' */

            // Save the old postion of the cursor
            long cursor_pos = ftell(file);

            {
                c = getc(file);

                if (c == EOF)
                {
                    lexical_error("\'{\' missing  \'}\'", line_n);
                    return SCANEOF;
                }
            }
            while (c != '}')
                ;

            // Reset the cursor position to the old position
            fseek(file, -cursor_pos, SEEK_CUR);
        }

        // comment ignoring
        else if (in_char == '/')
        {
            if (getc(file) == '/')
                for (char c = getc(file); c != EOF && c != '\n'; c = getc(file))
                    ;

            else
                ungetc(c, file);
        }
        // todo: work with functions
        // else if (check_comment())
        //     handle_comment();

        else
        {
            char errorBuffer[100];
            snprintf(errorBuffer, sizeof(errorBuffer), "Invalid syntax with '%c'", in_char);

            lexical_error(errorBuffer, line_n);
        }
    }
    printf("EOF reached\n");
    return SCANEOF;
}
