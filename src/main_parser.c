#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef enum
{
  SI,
  ALORS,
  SINON,
  NBINT,
  NB_FLOAT,
  ID,
  OPREL_INF,
  OPREL_SUP,
  OPREL_INF_EGAL,
  OPREL_SUPE_GAL,
  OPRELE_GAL,
  OPREL_DIFF,
  FIN,
  ERROR
} token;

int state = 0;
int start = 0;
int line = 1;
char tmp_error[20];






/******************************************************************************
 * retract;
 * Entrées: int retr - le nombre de caractères à reculer dans le fichier
 *          FILE *file - un pointeur vers le fichier source
 * Sorties: aucune
 */
void retract(int retr, FILE *file)
{
  if (!feof(file)) 
    fseek(file, -retr, SEEK_CUR);
  else 
  {
        fseek(file, -retr +1   , SEEK_CUR);
  }

}

void print_error_tmp()
{
  if(tmp_error[0] != '\0') 
  { 
    fprintf( stderr , "\nError at line %d : unknow  %s ", line , tmp_error);
          
  }
}

/******************************************************************************
 * fail;
 * Entrées: int retr - le nombre de caractères à reculer dans le fichier
 *          FILE *file - un pointeur vers le fichier source
 * Sorties: int - la nouvelle valeur de l'état de la machine à états
 */
int fail(int retr, FILE *file)
{
  retract(retr, file);
  switch (start)
  {
  case 0:
    start = 14;
    break;
  case 14:
    start = 23;
    break;
  case 23:
    start = 26;
    break;
  case 26:
    start = 0;
    return -1;
  default: 
    printf("\nSomething else!!");
  }
  return start;
}

/******************************************************************************
 * next_token;
 * Entrées: FILE *file - un pointeur vers le fichier source à analyser
 * Sorties: token - le prochain token identifié dans le fichier source
 */
token next_token(FILE *file)
{
  if (!file) 
  {
    printf("\nError: file not existe!!");
    exit(0);
  }

  // if (feof(file)) return FIN;
  
  int c , ind_buffer =0  ;

  // initialisation du buffer
   while(ind_buffer < 20)
        tmp_error[ind_buffer++] = '\0' ;

    ind_buffer = 0 ;
       
  while (1)
  {
    // if(!feof(file))
      c = fgetc(file);

    switch (state)
    {
/////////////////////////////// EOF //////////////////////////////////////
    case -1:
      if (feof(file)) return FIN;
      fprintf( stderr , "\nError at line %d : unknow %c  \n", line, c);
      return ERROR;
//////////////////////////////////////  MOT CLE ////////////////////////////////
    case 0 :
      if (feof(file)) return FIN;
      while (isspace(c))
      {
        if (c == '\n') line++;
        c = fgetc(file);
      }
      if (c == 's')     state = 1;
      else if(c == 'a') state = 8 ;
      else state = fail(1, file);
      break;
    
    case 1 :
      if (c == 'i') state = 2;
      else state = fail(2, file);
      break;
    
    case 2 :
      if(!isalpha(c))  { state = 3 ; retract(1, file);}
      else if (c == 'n') state = 4;
      else state = fail(3, file);
      break;
    
    case 3 :
      retract(1, file);
      return SI;

    case 4 :
      if (c == 'o') state = 5;
      else state = fail(4, file);
      break;
    
    case 5 :
      if (c == 'n') state = 6;
      else state = fail(5, file);
      break;

    case 6 :
    if(!isalpha(c)) {state = 7 ;  retract(1, file); }
    else state = fail(6 , file);
    break ;

    case 7 :
     retract(1, file);
     return SINON ;
    
    case 8 :
      if (c == 'l') state = 9;
      else state = fail(2, file);
      break;
    
    case 9 :
      if (c == 'o') state = 10;
      else state = fail(3, file);
      break;
    
    case 10 :
      if (c == 'r') state = 11;
      else state = fail(4, file);
      break;
    
    case 11 :
      if (c == 's') state = 12;
      else state = fail(5, file);
      break;
    
    case 12 :
    if(!isalpha(c)) {state = 13 ; retract(1, file);}
     else state = fail(6, file);  
     break ;

    case 13 :
       retract(1, file);
       return ALORS ;
//////////////////////////////////////// OPERL ////////////////////////////
    case 14 :
      if (c == '<') state = 15;
      else if (c == '>') state = 19;
      else if (c == '=') state = 22;
      else state = fail(1, file);
      break;
    
    case 15 :
      if (c == '>') state = 17;
      else if (c == '=') state = 18;
      else 
      {
          state = 16;
          retract(1, file);
      }
      break;
    
    case 16 :
        retract(1, file);
        return OPREL_INF;

    case 17 :
        retract(1, file);
        return OPREL_DIFF; 

    case 18 : 
        retract(1, file);
        return OPREL_INF_EGAL;     
    
    case 19:
        if (c == '=') state = 21 ;
        else 
        {
            state = 20;
            retract(1, file);
        }
        break ;
    
    case 20 :
      retract(1, file);
      return OPREL_SUP;
    
    case 21 :
      retract(1, file);
      return OPREL_SUPE_GAL;
    
    case 22 :
      retract(1, file);
      return OPRELE_GAL;
  ///////////////////////////////// ID /////////////////////
    case 23 :
      if (!feof(file) && (isalpha(c) || c == '_') ) state = 24;
      else state = fail(1, file);
      break;
    
    case 24 :
      if (!feof(file) && (isalnum(c) || c == '_')) state = 24;
      else
      {   
         retract(1, file);
           state = 25 ;
      }
      break ;

    case 25 :
        retract(1, file);
        return ID;
    //////////////////////////////////  entier et réel //////////////////////////
    case 26 :
      tmp_error[ind_buffer++]= (char)c ;
      if (isdigit(c)) state = 27;
      else if (c == '.') state = 32 ;
      else state = fail(1, file);
      break;
    
    case 27 :
      tmp_error[ind_buffer++]= (char)c ;
      if (!feof(file) && isdigit(c)) state = 27;
      else if (c == 'E' || c == 'e')  state = 28;
      else if (c == '.') state = 33;
      else
      {   
          state = 34 ;
          retract(1, file);
      }
      break;

      case 28 :
      tmp_error[ind_buffer++]= (char)c ;
      if (!feof(file) && isdigit(c)) state = 30;
      else if (c == '+' || c == '-') state = 29;
      else 
      {
        print_error_tmp();
        return ERROR ;
      }
      break;  

      case 29 :
      tmp_error[ind_buffer++]= (char)c ;
      if (!feof(file) && isdigit(c)) state = 30;
      else 
      {
        print_error_tmp();
        return ERROR ;
      }
      break ;

      case 30 :
      tmp_error[ind_buffer++]= (char)c ;
      if (!feof(file) && isdigit(c)) state = 30;
      else 
      {    
           state = 31 ;
           retract(1, file);
      }
      break ;

      case 31 :
        retract(1, file);
        return NB_FLOAT; 

      case 32 :
      tmp_error[ind_buffer++]= (char)c ;
      if (!feof(file) && isdigit(c)) state = 33;
      else 
      {
        print_error_tmp();
        return ERROR ;
      }
      break ;
      
      case 33 :
      tmp_error[ind_buffer++]= (char)c ;
      if (!feof(file) && isdigit(c)) state = 33;
      else if (c == 'E' || c == 'e') state = 28 ;
      else 
      {    
           state = 31 ;
           retract(1, file);

      }
      break ;
        
      case 34 :
        retract(1, file);
        return NBINT;
    
    
    default:
      printf("\nSomething else");
      return ERROR;
    }
  }
}

/******************************************************************************
 * token_get_name;
 * Entrées: token tk - un token
 * Sorties: char* - le nom du token sous forme de chaîne de caractères
 */
char *token_get_name(token tk)
{
  switch (tk)
  {
  case SI:
    return "SI";
  case ALORS:
    return "ALORS";
  case SINON:
    return "SINON";
  case OPREL_INF:
    return "OPREL_INF";
  case OPREL_INF_EGAL:
    return "OPREL_INF_EGAL";
  case OPREL_SUP:
    return "OPREL_SUP";
  case OPREL_SUPE_GAL:
    return "OPREL_SUPE_GAL";
  case OPRELE_GAL:
    return "OPRELE_GAL";
  case OPREL_DIFF:
    return "OPREL_DIFF";
  case ID:
    return "ID";
  case NBINT:
    return "NBINT";
  case NB_FLOAT:
    return "NB_FLOAT";
  case FIN:
    return "FIN";
  case ERROR:
    return "ERROR";
  default:
    printf("\nSomething is wrong");
    exit(0);
  }
}

/******************************************************************************
 * Open_File;
 * Entrées: char *filename - le nom du fichier à ouvrir
 *          char *mode - le mode d'ouverture du fichier
 * Sorties: FILE* - un pointeur vers le fichier ouvert
 */
FILE *Open_File(char *filename, char *mode)
{
  FILE *file = fopen(filename, mode);
  if (!file)
  {
    printf("\nErreur lors de l'ouverture de fichier %s", filename);
    exit(0);
  }
  return file;
}

/******************************************************************************
 * lex;
 * Entrées: char *filename - le nom du fichier source à analyser
 * Sorties: aucune
 */
void lex(char *filename)
{
  // Créer le nom du fichier de résultat
  char output_filename[256];
  strcpy(output_filename, filename);
  char *dot = strrchr(output_filename, '.');
  if (dot != NULL) {
    *dot = '\0';
  }
  strcat(output_filename, "_res.txt");
  
  FILE *filesource = Open_File(filename, "r");
  FILE *fileUL = Open_File(output_filename, "w");
  token tk;
  line = 1;
  
  printf("Analyse lexicale du fichier: %s\n", filename);
  printf("Resultat dans: %s\n", output_filename);
  printf("==============================\n");
  
  while ((tk = next_token(filesource)) != FIN)
  {
    start = 0;
    state = 0;
    if (tk != ERROR) 
    {
      fprintf(fileUL, "%s ", token_get_name(tk));
      printf("%s ", token_get_name(tk));
    }
   
  }
  
  fprintf(fileUL, "FIN");
  printf("FIN\n");
  
  fclose(filesource);
  fclose(fileUL);
}

/******************************************************************************
 * main;
 * Entrées: int argc - nombre d'arguments
 *          char *argv[] - tableau des arguments
 * Sorties: int - code de sortie du programme
 */
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <fichier_source>\n", argv[0]);
    printf("Exemple: %s text.txt\n", argv[0]);
    return 1;
  }
  
  lex(argv[1]);
  return 0;
}