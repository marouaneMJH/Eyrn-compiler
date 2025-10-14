#include "./../include/index.h"

#define CHECK_FILE(file, path)                     \
    if (file == NULL)                              \
    {                                              \
        printf("Could not open file: %s\n", path); \
        return 1;                                  \
    }

#define IN_FILE_PATH "./asset/test_program.txt"
#define IN_SCANNER_FILE_PATH "./asset/file.txt"
#define OUT_FILE_PATH "./asset/out_file.txt"

int main()
{
    FILE *input_file = fopen(IN_FILE_PATH, "r+");
    FILE *input_scanner_file = fopen(IN_SCANNER_FILE_PATH, "r");
    FILE *output_file = fopen(OUT_FILE_PATH, "w");

    CHECK_FILE(input_file, IN_FILE_PATH);
    CHECK_FILE(output_file, OUT_FILE_PATH);
    CHECK_FILE(input_scanner_file, IN_SCANNER_FILE_PATH);

    // printf("Starting parser...\n");
    // run_scanner(input_scanner_file, input_file);
    // printf("Starting parser...\n");

    printf("Starting parser...\n");

    // Initialize parser with input and output files
    init_parser(input_file, output_file);

    // Start parsing from the system goal
    printf("Parsing program...\n");
    system_goal();

    printf("Parsing completed successfully!\n");

    fclose(input_file);
    fclose(output_file);

    return 0;
}