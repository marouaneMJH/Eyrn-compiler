#ifndef SCANNER_H
#define SCANNER_H

#define MAX_LEXEME_LEN 64

typedef enum TokenTyped
{

    BEGIN,
    END,
    READ,
    WRITE,
    ID,
    INT_LITERAL,
    CURLY_BRACE_OPEN,
    CURLY_BRACE_CLOSE,
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

// TODO:
#ifndef CLI_STYLE_H
#define CLI_STYLE_H

// Reset
#define RESET "\033[0m"

// Styles
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define REVERSED "\033[7m"

// Foreground colors
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_BLUE "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"

#endif /* CLI_STYLE_H */

void clear_buffer(void);
void buffer_char(int c);
Token check_reserved(char *token);
void lexical_error(char errorStr[], int errorLine);

Token scanner(FILE *file);
#endif