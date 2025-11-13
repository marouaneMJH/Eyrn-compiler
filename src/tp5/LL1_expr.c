// ll1_expr.c — Analyseur LL(1) (descente recursive) avec evaluation
// Compile:  gcc -std=c11 -O2 ll1_expr.c -o ll1_expr
// Usage:    ./ll1_expr
// Exemple:  input:  3 + 4 * (2 - 1)

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define CHECK_FILE(file, path)                     \
    if (file == NULL)                              \
    {                                              \
        printf("Could not open file: %s\n", path); \
        return 1;                                  \
    }

// -------------------- LEXER --------------------
typedef enum {
    T_NUM, T_PLUS, T_MINUS, T_MUL, T_DIV, T_LP, T_RP, T_EOF, T_BAD
} Token;

typedef struct {
    const char *src;      // entree brute
    size_t i;             // position courante
    long num;             // valeur si T_NUM
    Token t;              // lookahead courant
} Lexer;

static void lexer_skip_ws(Lexer *L) {
    while (isspace((unsigned char)L->src[L->i])) L->i++;
}

static void lexer_next(Lexer *L) {
    lexer_skip_ws(L);
    char c = L->src[L->i];
    if (c == '\0') { L->t = T_EOF; return; }

    if (isdigit((unsigned char)c)) {
        long v = 0;
        while (isdigit((unsigned char)L->src[L->i])) {
            v = v*10 + (L->src[L->i]-'0');
            L->i++;
        }
        L->num = v;
        L->t = T_NUM;
        return;
    }

    L->i++;
    switch (c) {
        case '+': L->t = T_PLUS;  break;
        case '-': L->t = T_MINUS; break;
        case '*': L->t = T_MUL;   break;
        case '/': L->t = T_DIV;   break;
        case '(': L->t = T_LP;    break;
        case ')': L->t = T_RP;    break;
        default:  L->t = T_BAD;   break;
    }
}

// -------------------- PARSER (LL(1)) --------------------
// Grammaire sans recursivite gauche (E, E', T, T', F).
// Chaque fonction implemente une regle et retourne la valeur evaluee.
// Si erreur, on imprime un message et quitte (code simple/solide).

typedef struct { Lexer L; } Parser;

static void syntax_error(const char *msg, const Lexer *L) {
    fprintf(stderr, "[Erreur syntaxe] %s pres de position %zu\n", msg, L->i);
    exit(1);
}

static void expect(Parser *P, Token tk) {
    if (P->L.t != tk) syntax_error("Symbole inattendu", &P->L);
    lexer_next(&P->L);
}

// F -> (E) | nb
static long parse_F(Parser *P);

// T' -> * F T' | / F T' | ε
static long parse_Tp(Parser *P, long acc);

// E' -> + T E' | - T E' | ε
static long parse_Ep(Parser *P, long acc);

// T -> F T'
static long parse_T(Parser *P) {
    long v = parse_F(P);
    return parse_Tp(P, v);
}

// E -> T E'
static long parse_E(Parser *P) {
    long v = parse_T(P);
    return parse_Ep(P, v);
}

static long parse_F(Parser *P) {
    if (P->L.t == T_LP) {
        lexer_next(&P->L);         // consomme '('
        long v = parse_E(P);       // parse E
        if (P->L.t != T_RP) syntax_error("Manque ')'", &P->L);
        lexer_next(&P->L);         // consomme ')'
        return v;
    } else if (P->L.t == T_NUM) {
        long v = P->L.num;
        lexer_next(&P->L);         // consomme nombre
        return v;
    }
    syntax_error("Facteur attendu (nombre ou '('E')')", &P->L);
    return 0; // unreachable
}

static long parse_Tp(Parser *P, long acc) {
    if (P->L.t == T_MUL) {
        lexer_next(&P->L);
        long f = parse_F(P);
        // gestion / verif overflow laissee simple
        return parse_Tp(P, acc * f);
    } else if (P->L.t == T_DIV) {
        lexer_next(&P->L);
        long f = parse_F(P);
        if (f == 0) syntax_error("Division par zero", &P->L);
        return parse_Tp(P, acc / f);
    }
    // ε
    return acc;
}

static long parse_Ep(Parser *P, long acc) {
    if (P->L.t == T_PLUS) {
        lexer_next(&P->L);
        long t = parse_T(P);
        return parse_Ep(P, acc + t);
    } else if (P->L.t == T_MINUS) {
        lexer_next(&P->L);
        long t = parse_T(P);
        return parse_Ep(P, acc - t);
    }
    // ε
    return acc;
}

// -------------------- MAIN --------------------
// -------------------- MAIN --------------------
int main(void) {
    char buf[4096];   // tampon pour stocker le contenu du fichier
    FILE *f = fopen("file.txt", "r");  // ouvre le fichier en lecture

    if (!f) { // verifie si l’ouverture a echoue
        perror("Erreur ouverture file.txt");
        return 1;
    }

    // Lit le contenu du fichier (une ligne ou une expression complète)
    if (!fgets(buf, sizeof(buf), f)) {
        fprintf(stderr, "Erreur : fichier vide ou non lisible.\n");
        fclose(f);
        return 1;
    }

    fclose(f); // on peut fermer le fichier après lecture

    // (Optionnel) afficher ce qui a ete lu
    printf("Expression lue depuis file.txt : %s\n", buf);

    // Initialisation du parseur
    Parser P;
    P.L.src = buf;   // la source du lexer est la chaîne lue du fichier
    P.L.i = 0;       // position initiale = 0
    lexer_next(&P.L); // lecture du premier token

    // Demarre l'analyse syntaxique
    long result = parse_E(&P);

    // Verifie qu’il ne reste plus rien à analyser
    if (P.L.t != T_EOF && P.L.t != T_RP) {
        syntax_error("Entree non totalement consommee", &P.L);
    }

    printf("Expression valide Resultat = %ld\n", result);
    return 0;
}

