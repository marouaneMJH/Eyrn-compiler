#include <stdio.h>
#include "scanner.h"

#define FILE_PATH "./asset/file.txt"

int main()
{

    FILE *file = fopen(FILE_PATH, "r");

    if (file == NULL)
    {
        printf("Could not open file %s for reading\n", FILE_PATH);
        return 1;
    }
    int i = 0;
    while (1)
    {
        i++;
        Token t = scanner(file);
        const char *token_names[] = {
            "BEGIN", "END", "READ", "WRITE", "ID",
            "INT_LITERAL", "SCAN_OF", "PLUS_OP", "MINUS_OP",
            "SCANEOF", "L_PAREN", "R_PAREN", "SEMICOLON",
            "COMMA", "ASSIGN_OP"
            // Add all other token types from your enum
        };
        printf("Token: %s, Lexeme: %s\n", token_names[t], "NONE FOR NOW");

        if (i > 20)
            break;
    }

    return 0;
}