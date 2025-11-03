#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../../../include/cli_style.h"

#define MAX_LEXEME_LEN 64






typedef enum {
    SI,
    ALORS,
    SINON,
    OPREL,
    ID,
    NB,
    ERROR

}Token;



typedef struct
{
    Token token;
    char key_word[MAX_LEXEME_LEN];

}KeyWord;

char token_buffer[MAX_LEXEME_LEN];

// /* Function prototypes */

int buf_index = 0;
int line_n = 1;
int err_char = 0;



void clear_buffer(void);

void buffer_char(int c);

Token check_reserved(char *token);

void lexical_error(char errorStr[]);

Token scanner(FILE *file);

void run_scanner(FILE *in_file, FILE *out_file);


int main()
{
    int name;
    int name2;

    return 0;
}

void lexical_error(char errorStr[])
{
    printf(FG_RED BOLD UNDERLINE "LexicalError" FG_MAGENTA "[%d]" RESET ": " FG_MAGENTA " % s \n" RESET,
           line_n,
           errorStr);
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
        lexical_error("[MEMORY_CRITICAL] The given token is null");
        exit(EXIT_FAILURE);
    }


    return ID;
}


// return single token
Token scanner(FILE *file)

{
    int in_char,
        c;
    clear_buffer();


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
            // return 
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
                return NB;
            }
            else
            {
                /* not a float, put back the character and handle '.' as unknown */
                ungetc(c, file);
                char errorBuffer[100];
                snprintf(errorBuffer, sizeof(errorBuffer), "Invalid syntax with '%c'", in_char);
                lexical_error(errorBuffer);
            }
        }

        // comment ignoring
        else if (in_char == '/')
        {
            if (getc(file) == '/')
            {
                for (char c = getc(file); c != EOF && c != '\n'; c = getc(file))
                    ;
                    line_n++;
            }

            else
            {


                // ? attention: make sure to remove this expression cuz we dont have the '/'
                // ungetc(in_char, file);
                lexical_error("Maybe you meant '//' for comments instead of '/'?");
            }
        }

        else
        {
            char errorBuffer[100];
            snprintf(errorBuffer, sizeof(errorBuffer), "Invalid syntax with '%c'", in_char);

            lexical_error(errorBuffer);
        }
    }

}


