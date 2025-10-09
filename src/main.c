#include <stdio.h>
#include "scanner.h"

#define CHECK_FILE(file, path)                     \
    if (file == NULL)                              \
    {                                              \
        printf("Could not open file: %s\n", path); \
        return 1;                                  \
    }

#define IN_FILE_PATH "./asset/file.txt"
#define OUT_FILE_PATH "./asset/out_file.txt"

int main()
{

    FILE *file = fopen(IN_FILE_PATH, "r");
    FILE *out_file = fopen(OUT_FILE_PATH, "w");

    CHECK_FILE(file, IN_FILE_PATH);
    CHECK_FILE(out_file, OUT_FILE_PATH);

    int i = 0;
    while (1)
    {
        i++;
        Token t = scanner(file);
        const char *token_names[] = {

            "BEGIN",
            "END",
            "READ",
            "WRITE",
            "ID",
            "INT_LITERAL",
            "FLOAT_LITERAL",
            "CURLY_BRACE_OPEN",
            "CURLY_BRACE_CLOSE",
            "SCAN_OF",
            "PLUS_OP",
            "MINUS_OP",
            "SCANEOF",
            "L_PAREN",
            "R_PAREN",
            "SEMICOLON",
            "COMMA",
            "ASSIGN_OP",
        };

        fprintf(out_file, "%s ", token_names[t]);

        if (i > 100)
            break;
    }

    return 0;
}