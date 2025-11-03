#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define CHECK_FILE(file, path)                     \
if (file == NULL)                              \
{                                              \
    printf("Could not open file: %s\n", path); \
    return 1;                                  \
}
#define IN_FILE_PATH "test_program.txt"
// #define IN_SCANNER_FILE_PATH "./../../../asset/file.txt"
// #define OUT_FILE_PATH "./../../../asset/out_file.txt"


#define MAX_LEXEME_LEN 64
typedef enum {
    T_SI,
    T_ALORS,
    T_SINON,
    T_OPREL,
    T_NB,
    T_ID,
    T_ERR,
    SCAN_EOF,
} TokenType;



typedef struct {
    TokenType type;
    char lexeme[256];
} Token;



char token_buffer[MAX_LEXEME_LEN];

// /* Function prototypes */

int buf_index = 0;
int line_n = 1;
int err_char = 0;

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




int start =0,state=0;
int lexical_value;

char* create_string_copy( char* originalString);
int fail(int retr, FILE *in_file);
Token next_token(FILE* file);
void retract(int retr, FILE *file);





int main()
{
    FILE *input_file = fopen(IN_FILE_PATH, "r+");
    // FILE *input_scanner_file = fopen(IN_SCANNER_FILE_PATH, "r");
    // FILE *output_file = fopen(OUT_FILE_PATH, "w");

    CHECK_FILE(input_file, IN_FILE_PATH);
    // CHECK_FILE(output_file, OUT_FILE_PATH);
    // CHECK_FILE(input_scanner_file, IN_SCANNER_FILE_PATH);

    // printf("Starting parser...\n");
    // run_scanner(input_scanner_file, input_file);
    // printf("Starting parser...\n");

    printf("Starting parser...\n");
    Token token = next_token(input_file);
     printf("Token: %d, Lexeme:%s\n",token.type, token.lexeme);

    // while (1)
    // {
    //     if (token.type == SCAN_EOF) {
    //         break;
    //     }
    //     printf("Token: %s, Lexeme: %s\n", 
    //            token.type == T_SI ? "T_SI" :
    //            token.type == T_ALORS ? "T_ALORS" :
    //            token.type == T_SINON ? "T_SINON" :
    //            token.type == T_OPREL ? "T_OPREL" :
    //            token.type == T_NB ? "T_NB" :
    //            token.type == T_ID ? "T_ID" :
    //            token.type == T_ERR ? "T_ERR" : "UNKNOWN",
    //            token.lexeme
    //     );


    // }


    printf("Parsing completed successfully!\n");

    fclose(input_file);
    // fclose(output_file);

    return 0;

}


void retract(int retr, FILE *file)
{
  if (!feof(file)) 
    fseek(file, -retr, SEEK_CUR);
  else 
  {
        fseek(file, -retr +1   , SEEK_CUR);
  }

}

int fail(int retr, FILE *in_file)
{
    clear_buffer();
  retract(retr, in_file);
  switch (start)
  {
    case 0:
        start = 3;
        break;
    case 3:
        start = 9;
        break;
    case 9:
        start = 15;
        break;
    case 15:
        start =19 ;
        break;
    case 19:
        start =26;
        break;

    default:
        start = -1;
  }
  return start;
}




/******************************************************************************
 * next_token;
 * Entrées: FILE *file - un pointeur vers le fichier source à analyser
 * Sorties: token - le prochain token identifié dans le fichier source
 */
Token next_token(FILE *file)
{
  if (!file) 
  {
    printf("\nError: file not exist!!");
    exit(0);
  }

  int c ;

  
  // initialisation du buffer
//    while(ind_buffer < 20)
    // tmp_error[ind_buffer++] = '\0' ;
    // ind_buffer = 0 ;

  clear_buffer();

  while (1)
  {
    // if(!feof(file))
    c = fgetc(file);
    // while (isspace(c))
    // {
    //     if (c == '\n') line_n++;
    //     if(c== EOF)
    //     {
    //         Token token = {
    //             type: SCAN_EOF,
    //             lexeme: "EOF",
    //         };
    //         return token;
    //     }
        
    //     c = fgetc(file);
    // }


    switch (state)
    {
/////////////////////////////// EOF //////////////////////////////////////
    // case -1:
    //   if (feof(file)) return FIN;
    //   fprintf( stderr , "\nError at line_n %d : unknow %c  \n", line_n, c);
    //   return ERROR;
//////////////////////////////////////  MOT CLE ////////////////////////////////
    case 0 :
      while (isspace(c))
      {
        if (c == '\n') line_n++;
        c = fgetc(file);
      }
      if (c == 's')  state = 1;
      else state = fail(1,file);
      break;

    case 1 :
      if (c == 'i') state = 2;
      else state = fail(2,file);
      break;
    case 2 :
      if(isspace(c))
      {
        if(c=='\n') line_n++;

        Token token ={
            type: T_SI,
            lexeme: "SI"
        };
        return token;
      }
      else
        state=fail(3,file);
    break;
        
    case 3 :
        while (isspace(c))
        {
            if (c == '\n') line_n++;
            c = fgetc(file);
        }
      if (c == 'a')  state = 4;
      else state = fail(1,file);
      break;

    case 4 :
      if (c == 'l') state = 5;
      else state = fail(2, file);
      break;
    
    case 5 :
      if (c == 'o') state = 6;
      else state = fail(3, file);
      break;

    case 6 :
      if (c == 'r') state = 7;
      else state = fail(4, file);
      break;
    case 7 :
      if (c == 's') state = 8;
      else state = fail(5, file);
      break;

    case 8 :
        if(isspace(c))
        {
            if(c=='\n') line_n++;
            Token token ={
                type: T_ALORS,
                lexeme: "ALORS"
            };
            return token;
        }
        else
            state=fail(6,file);
        break;

    case 9:
        while (isspace(c))
        {
            if (c == '\n') line_n++;
            c = fgetc(file);
        }

        if (c == 's')  state = 10;
        else state = fail(1,file);
        break;

    case 10:
        if (c == 'i')  state = 11;
        else state = fail(2,file);
        break;

    case 11:
        if (c == 'n')  state = 12;
        else state = fail(3,file);
        break;

    case 12:
        if (c == 'o')  state = 13;
        else state = fail(4,file);
        break;
    
    case 13:
        if (c == 'n')  state = 14;
        else state = fail(5,file);
        break;

    case 14:
        if(isspace(c))
        {
            if(c=='\n') line_n++;
            Token token ={
                type: T_SINON,
                lexeme: "SINON"
            };
            return token;
        }
        else
            state=fail(6,file);
        break;


//////////////////////////////// OPERL ////////////////////////////
    case 15 :
        buffer_char(c);
        if (c == '<') state = 16;
        else if (c == '=') state = 17;
        else if (c == '>') state = 18;
        else state = fail(1, file);
        break;

    case 16 :
        buffer_char(c);
        if (c == '>' || c=='=' )  state = 17;
        else if(c=='\n' || isspace(c) || isalpha(c) || isdigit(c))
        {
            retract(1, file);
            Token token ={
                type: T_OPREL,
                lexeme: "<"
            };
            return token;
        }
        else
        {
            state = fail(2, file);
        }
        break;

    case 18 :
    buffer_char(c);
        if (c=='=' )  state = 17;
        else if(c=='\n' || isspace(c) || isalpha(c) || isdigit(c))
        {
            retract(1, file);
            Token token ={
                type: T_OPREL,
                lexeme: ">"
            };
            return token;
        }
        else
            state = fail(2, file);
        break;

    

    case 17 :
        if(c=='\n' || isspace(c) || isalpha(c) || isdigit(c))
        {
            retract(1, file);
            
            Token token;
            
            token.type = T_OPREL;
            strcpy(token.lexeme, create_string_copy(token_buffer));


            return token;
        }
        else
            state = fail(strlen(token_buffer), file);
        break;
    
    //////////////////////////////////  NB /////////////////////

    case 19 :

        while (isspace(c))
        {
            if (c == '\n') line_n++;
            c = fgetc(file);
        }
        buffer_char(c);
        if (isdigit(c)) state = 20;
        else state = fail(1, file);
        break;

    case 20 :
        if(isspace(c))
        {
            retract(1, file);
            Token token ;

            token.type = T_NB;
            strcpy(token.lexeme, create_string_copy(token_buffer));

            return token;
        }
        buffer_char(c);
        if (isdigit(c)) state = 20;
        if (c='E') state = 23;
        if (c='.') state = 21;

        else state = fail(strlen(token_buffer), file);

        break;

        case 21 :
            buffer_char(c);
            if (isdigit(c)) state = 22;

            else
                state = fail(strlen(token_buffer), file);

            break;

        case 22 :
            if(isspace(c))
            {
                Token token;

                token.type = T_NB;
                strcpy(token.lexeme, create_string_copy(token_buffer));

                return token;
            }
            buffer_char(c);
            if (isdigit(c)) state = 22;
            else if (c=='E') state = 23;
            else
                state = fail(strlen(token_buffer), file);
            break;

        case 23 :
            buffer_char(c);
            if (c=='+' || c=='-') state = 25;
            else if (isdigit(c)) state = 24;
            else
                state = fail(strlen(token_buffer), file);
        
        case 24 :
            buffer_char(c);
            if (isdigit(c)) state = 25;

            else
                state = fail(strlen(token_buffer), file);

        case 25 :
            if(isspace(c))
            {
                Token token;
                strcpy(token.lexeme,create_string_copy(token_buffer));
                token.type = T_NB;
                
                return token;
            }
            buffer_char(c);
            if (isdigit(c)) state = 25;
            else
                state = fail(strlen(token_buffer), file);
            break;


    //////////////////////////////////  ID /////////////////////
    case 26 :
        
        while (isspace(c))
        {
            if (c == '\n') line_n++;
            c = fgetc(file);
        }

        buffer_char(c);
        if (isalpha(c)) state = 27;
        
        else state = fail(1, file);
        
        break;
    
    case 27 :
        if(!isalnum(c) )
        {
            retract(1, file);
            Token token;
            token.type = T_ID;
            strcpy(token.lexeme, create_string_copy(token_buffer));
            return token;
        }

        buffer_char(c);
        if (isalnum(c) ) state = 27;
        else
            state = fail(strlen(token_buffer), file);
        break ;


     default:
      fprintf( stderr , "\nError at line_n %d : unknow %c  \n", line_n, c);
      Token token = {
          type: T_ERR,
          lexeme: "ERROR",
      };

      return token;

//     }
  }
}

}
char* create_string_copy( char* originalString)
{
    if (originalString == NULL) {
        return NULL; // Handle null input
    }

    size_t length = strlen(originalString);
    // Allocate memory for the new string, including the null terminator
    char* newString = (char*)malloc((length + 1) * sizeof(char));

    if (newString == NULL) {
        perror("Memory allocation failed"); // Handle allocation failure
        return NULL;
    }

    // Copy the contents of the original string to the new string
    strcpy(newString, originalString);

    return newString; // Return the pointer to the new string
}






