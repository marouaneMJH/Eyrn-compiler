// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// #define MAX_LEXEME_LEN 64

// typedef enum
// {
//     BEGIN,
//     END,
//     READ,
//     WRITE,
//     ID,
//     INTLITERAL,
//     LPAREN,
//     R_PAREN,
//     SEMICOLON,
//     COMMA,
//     PLUSOP,
//     MINUS_OP,
//     ASSIGN_OP,
//     EOL,
//     SCANEOF,
//     ERROR
// } token;

// char token_buffer[MAX_LEXEME_LEN];
// int buf_index = 0;
// int line_n = 1;
// int err_char = 0;

// void clear_buffer()
// {
//     for (int i = 0; i < MAX_LEXEME_LEN; i++)
//     {
//         token_buffer[i] = '\0';
//     }
//     buf_index = 0;
// }

// void buffer_char(int c)
// {
//     if (buf_index < MAX_LEXEME_LEN - 1)
//     {
//         token_buffer[buf_index++] = (char)c;
//         token_buffer[buf_index] = '\0';
//     }
// }

// token check_reserved()
// {
//     if (strcmp(token_buffer, "BEGIN") == 0)
//         return BEGIN;
//     if (strcmp(token_buffer, "END") == 0)
//         return END;
//     if (strcmp(token_buffer, "READ") == 0)
//         return READ;
//     if (strcmp(token_buffer, "WRITE") == 0)
//         return WRITE;
//     return ID;
// }

// void lexical_error()
// {
//     printf("Erreur lexicale à la ligne %d (caractère: '%c')\n", line_n, err_char);
// }

// token scanner(FILE *in)
// {
//     int in_char, c;
//     clear_buffer();

//     while ((in_char = fgetc(in)) != EOF)
//     {
//         if (isspace(in_char))
//         {
//             if (in_char == '\n')
//             {
//                 line_n++;
//                 return EOL;
//             }
//             continue;
//         }

//         if (isalpha(in_char))
//         {
//             buffer_char(in_char);
//             while ((c = fgetc(in)), (isalnum(c) || c == '_'))
//             {
//                 buffer_char(c);
//             }
//             ungetc(c, in);
//             return check_reserved();
//         }

//         if (isdigit(in_char))
//         {
//             buffer_char(in_char);
//             while ((c = fgetc(in)), isdigit(c))
//             {
//                 buffer_char(c);
//             }
//             ungetc(c, in);
//             return INTLITERAL;
//         }

//         if (in_char == '(')
//             return LPAREN;
//         if (in_char == ')')
//             return R_PAREN;
//         if (in_char == ';')
//             return SEMICOLON;
//         if (in_char == ',')
//             return COMMA;
//         if (in_char == '+')
//             return PLUSOP;

//         if (in_char == ':')
//         {
//             c = fgetc(in);
//             if (c == '=')
//                 return ASSIGN_OP;
//             ungetc(c, in);
//             err_char = in_char;
//             return ERROR;
//         }

//         if (in_char == '-')
//         {
//             c = fgetc(in);
//             if (c == '-')
//             {
//                 while ((c = fgetc(in)) != EOF && c != '\n')
//                     ;
//                 line_n++;
//                 return EOL;
//             }
//             ungetc(c, in);
//             return MINUS_OP;
//         }

//         err_char = in_char;
//         return ERROR;
//     }

//     return SCANEOF;
// }

// void print_token(FILE *out, token tk)
// {
//     switch (tk)
//     {
//     case BEGIN:
//         fprintf(out, "BEGIN ");
//         break;
//     case END:
//         fprintf(out, "END ");
//         break;
//     case READ:
//         fprintf(out, "READ ");
//         break;
//     case WRITE:
//         fprintf(out, "WRITE ");
//         break;
//     case ID:
//         fprintf(out, "ID ");
//         break;
//     case INTLITERAL:
//         fprintf(out, "INTLITERAL ");
//         break;
//     case ASSIGN_OP:
//         fprintf(out, "ASSIGN_OP ");
//         break;
//     case LPAREN:
//         fprintf(out, "LPAREN ");
//         break;
//     case R_PAREN:
//         fprintf(out, "R_PAREN ");
//         break;
//     case SEMICOLON:
//         fprintf(out, "SEMICOLON ");
//         break;
//     case COMMA:
//         fprintf(out, "COMMA ");
//         break;
//     case PLUSOP:
//         fprintf(out, "PLUSOP ");
//         break;
//     case MINUS_OP:
//         fprintf(out, "MINUS_OP ");
//         break;
//     case SCANEOF:
//         fprintf(out, "SCANEOF ");
//         break;
//     default:
//         fprintf(out, "ERROR ");
//         break;
//     }
// }

// int main()
// {
//     FILE *source = fopen("codesource.txt", "r");
//     FILE *output = fopen("codeUL.txt", "w");

//     if (!source || !output)
//     {
//         printf("Erreur ouverture fichier\n");
//         return 1;
//     }

//     token tk;
//     do
//     {
//         tk = scanner(source);
//         if (tk == EOL)
//         {
//             fprintf(output, "\n");
//         }
//         else if (tk == ERROR)
//         {
//             lexical_error();
//         }
//         else
//         {
//             print_token(output, tk);
//         }
//     } while (tk != SCANEOF);

//     fclose(source);
//     fclose(output);

//     printf("Analyse lexicale terminée. Résultats dans codeUL.txt\n");
//     return 0;
// }