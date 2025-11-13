#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../../include/cli_style.h"

/* ============================================================
   CONFIGURATION
   ============================================================ */
#define IN_FILE_PATH "test_program_.txt"
#define MAX_LEXEME_LEN 64

/* ============================================================
   STRUCTURES ET TYPES
   ============================================================ */
typedef enum {
    T_SI,
    T_ALORS,
    T_SINON,
    T_OPREL,
    T_NB,
    T_ID,
    T_ERR,
    SCAN_EOF
} TokenType;

char *token_names[]= {
    "T_SI",
    "T_ALORS",
    "T_SINON",
    "T_OPREL",
    "T_NB",
    "T_ID",
    "T_ERR",
    "SCAN_EOF"
};


typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME_LEN];
} Token;

/* ============================================================
   VARIABLES GLOBALES
   ============================================================ */
int start = 0;   // état de départ (0, 3, 9, 15, 19, 26)
int state = 0;
int line_n = 1;
char token_buffer[MAX_LEXEME_LEN];
int buf_index = 0;

/* ============================================================
   FONCTIONS OUTILS
   ============================================================ */
void clear_buffer(void) {
    memset(token_buffer, '\0', sizeof(token_buffer));
    buf_index = 0;
}

void buffer_char(int c) {
    if (buf_index < MAX_LEXEME_LEN - 1 && c != EOF) {
        token_buffer[buf_index++] = (char)c;
        token_buffer[buf_index] = '\0';
    }
}

void retract(int retr, FILE *file) {
    if (!feof(file)) fseek(file, -retr, SEEK_CUR);
    else fseek(file, -retr + 1, SEEK_CUR);
}

int fail(int retr, FILE *in_file) {
    clear_buffer();
    retract(retr, in_file);

    switch (start) {
        case 0:  start = 3;  break; // si -> alors
        case 3:  start = 9;  break; // alors -> sinon
        case 9:  start = 15; break; // sinon -> oprel
        case 15: start = 19; break; // oprel -> nb
        case 19: start = 26; break; // nb -> id
        default: start = -1; break;
    }
    return start;
}

int is_delim(int c) {

    if(isspace(c) || c == EOF || c == '\n') return 1;

    return 0;
}

/* ============================================================
   FONCTION PRINCIPALE DE RECONNAISSANCE
   ============================================================ */
Token next_token(FILE *file) {
    if (!file) { fprintf(stderr, "Erreur : fichier introuvable.\n"); exit(1); }

    clear_buffer();
    int c;
    state = start;
    c = fgetc(file);
            // Skip new lines
        if (c == '\n') 
        {
            line_n++;
        };



    while (1) {
        // Reached EOF
        if( c == EOF ) {
            Token token = {SCAN_EOF, "EOF"};
            return token;
        }

        switch (state) {

        /* ------------------- SI (0–2) ------------------- */
        case 0:
            while (isspace(c)) { c = fgetc(file); if (c == '\n') line_n++; }
            if (c == 's') state = 1;
            else { state = fail(1, file); break; }
            break;

        case 1:
            if (c == 'i') state = 2;
            else { state = fail(2, file); break; }
            break;

        case 2:
            if (is_delim(c)) {
                if (c != EOF) retract(1, file);
                Token t = { T_SI, "si" };
                start = 0;
                return t;
            } else state = fail(3, file);
            break;

        /* ------------------- ALORS (3–8) ------------------- */
        case 3: while (isspace(c)) { c=fgetc(file); if (c=='\n') line_n++; }
                if (c=='a') state=4; else state=fail(1,file); break;
        case 4: if (c=='l') state=5; else state=fail(2,file); break;
        case 5: if (c=='o') state=6; else state=fail(3,file); break;
        case 6: if (c=='r') state=7; else state=fail(4,file); break;
        case 7: if (c=='s') state=8; else state=fail(5,file); break;
        case 8:
            if (is_delim(c)) {
                if (c != EOF) retract(1, file);
                Token t = { T_ALORS, "alors" };
                start = 0;
                return t;
            } else state = fail(6, file);
            break;

        /* ------------------- SINON (9–14) ------------------- */
        case 9: while (isspace(c)) { c=fgetc(file); if (c=='\n') line_n++; }
                if (c=='s') state=10; else state=fail(1,file); break;
        case 10: if (c=='i') state=11; else state=fail(2,file); break;
        case 11: if (c=='n') state=12; else state=fail(3,file); break;
        case 12: if (c=='o') state=13; else state=fail(4,file); break;
        case 13: if (c=='n') state=14; else state=fail(5,file); break;
        case 14:
            if (is_delim(c)) {
                if (c != EOF) retract(1, file);
                Token t = { T_SINON, "sinon" };
                start = 0;
                return t;
            } else state = fail(6, file);
            break;

        /* ------------------- OPREL (15–18) ------------------- */
        case 15:
            buffer_char(c);
            if (c == '<') state = 16;
            else if (c == '=') state = 17;
            else if (c == '>') state = 18;
            else state = fail(1, file);
            break;

        case 16:
            buffer_char(c);
            if (c == '=' || c == '>') state = 17;
            else if (is_delim(c)) {
                if (c != EOF) retract(1, file);
                Token t = { T_OPREL, "<" };
                start = 0;
                return t;
            } else state = fail(2, file);
            break;

        case 18:
            buffer_char(c);
            if (c == '=') state = 17;
            else if (is_delim(c)) {
                if (c != EOF) retract(1, file);
                Token t = { T_OPREL, ">" };
                start = 0;
                return t;
            } else state = fail(2, file);
            break;

        case 17:
            if (is_delim(c)) {
                if (c != EOF) retract(1, file);
                Token t;
                t.type = T_OPREL;
                strcpy(t.lexeme, token_buffer);
                start = 0;
                return t;
            } else state = fail(strlen(token_buffer), file);
            break;

        /* ------------------- NB (19–25) ------------------- */
        case 19:
            while (isspace(c)) { c=fgetc(file); if (c=='\n') line_n++; }
            buffer_char(c);
            if (isdigit(c)) state=20;
            else state=fail(1,file);
            break;

        case 20:
            buffer_char(c);
            if (isdigit(c)) state=20;
            else if (c=='.') state=21;
            else if (c=='E') state=23;
            else if (is_delim(c) || !isdigit(c)) {
                if (c!=EOF) retract(1,file);
                Token t={T_NB,""};
                strncpy(t.lexeme,token_buffer,strlen(token_buffer)-1);
                start = 0;
                return t;
            } else state=fail(strlen(token_buffer),file);
            break;

        case 21:
            buffer_char(c);
            if (isdigit(c)) state=22;
            else state=fail(strlen(token_buffer),file);
            break;

        case 22:
            buffer_char(c);
            if (isdigit(c)) state=22;
            else if (c=='E') state=23;
            else if (is_delim(c) || !isdigit(c)) {
                if (c!=EOF) retract(1,file);
                Token t={T_NB,""};
                // strcpy(t.lexeme,token_buffer);
                strncpy(t.lexeme,token_buffer,strlen(token_buffer)-1);

                start = 0;
                return t;
            } else state=fail(strlen(token_buffer),file);
            break;

        case 23:
            buffer_char(c);
            if (c=='+'||c=='-') state=25;
            else if (isdigit(c)) state=24;
            else state=fail(strlen(token_buffer),file);
            break;

        case 24:
            buffer_char(c);
            if (isdigit(c)) state=24;
            else if (is_delim(c) ) {
                if (c!=EOF) retract(1,file);
                Token t={T_NB,""};
                strcpy(t.lexeme,token_buffer);
                start = 0;
                return t;
            } else state=fail(strlen(token_buffer),file);
            break;

        case 25:
            buffer_char(c);
            if (isdigit(c)) state=25;
            else if (is_delim(c)) {
                if (c!=EOF) retract(1,file);
                Token t={T_NB,""};
                strcpy(t.lexeme,token_buffer);
                start = 0;
                return t;
            } else state=fail(strlen(token_buffer),file);
            break;

        /* ------------------- ID (26–27) ------------------- */
        case 26:
            while (isspace(c)) { c=fgetc(file); if (c=='\n') line_n++; }
            buffer_char(c);
            if (isalpha(c)) state=27;
            else state=fail(1,file);
            break;

        case 27:
            if (is_delim(c) || !isalnum(c) ) {
                if (c!=EOF) retract(1,file);
                Token t={T_ID,""};
                strcpy(t.lexeme,token_buffer);
                start = 0;
                return t;
            }
            buffer_char(c);
            if (isalnum(c)) state=27;
            else state=fail(strlen(token_buffer),file);
            break;

        default:
            {
                Token err;
                err.type = T_ERR;
                snprintf(err.lexeme, sizeof(err.lexeme), "Unknown word '%c' in  %d", c,line_n);
                start = 0;
                return err;
            }
        } // end switch
        c = fgetc(file);

    } // end while
}


void scan_file(FILE *file) {
    if (!file) {
        fprintf(stderr, "Erreur : fichier introuvable.\n");
        exit(1);
    }

    printf("====================================\n\n");

    while (1) {
        Token t = next_token(file);
        if (t.type == SCAN_EOF) break;
        if (t.type == T_ERR) printf(FG_RED "Erreur :" RESET" %s\n", t.lexeme);
        else printf("Token: %s  Lexeme: %s\n", token_names[t.type], t.lexeme);
        if (feof(file)) break;
    }

    printf("\n====================================\n");
}



/* ============================================================
   MAIN PROGRAMME
   ============================================================ */
int main() {
    FILE *input_file = fopen(IN_FILE_PATH, "r");
    if (!input_file) { perror("Impossible d’ouvrir le fichier"); return 1; }

    scan_file(input_file);
    fclose(input_file);

    return 0;
}

