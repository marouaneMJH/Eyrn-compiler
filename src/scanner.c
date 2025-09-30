#include <stdio.h>

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

extern char token_buffer[];

typedef enum TokenTyped
{

    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INT_LITERAL,
    SCAN_OF,
    PLUS_OP,
    MINUS_OP,
    SCANEOF,
    L_PAREN,
    R_PAREN,
    SEMICOLON,
    COMMA,
    ASSIGN_OP,
} Token;

#include <stdio.h>
#include <ctype.h>

/* Function prototypes */
void clear_buffer(void);
void buffer_char(int c);
int check_reserved(void);
void lexical_error(int c);

// todo:
// bool check_comment(FILE *file);
// bool handle_comment(FILE *file);

// return single token
int scanner(FILE *file)
{
    int in_char,
        c;
    clear_buffer();

    if (feof(file))
        return SCANEOF;

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
            return check_reserved();
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
                lexical_error(in_char);
            }
        }
        else if (in_char == '{')
        {
            /* comment start: skip until '}' */
            do
            {
                c = getc(file);
                if (c == EOF)
                {
                    lexical_error(in_char);
                    return SCANEOF;
                }
            } while (c != '}');
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
            lexical_error(in_char);
        }
    }

    return SCANEOF;
}

// Put the new character to the chars buffer (save the hole variable name )
// todo: work with dynamic token_buffer
void buffer_char(int c)
{
    size_t len = strlen(token_buffer);
    token_buffer[len] = (char)c;
    token_buffer[len + 1] = '\0';
}