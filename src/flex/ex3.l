%{
#include <stdio.h>
%}

%%
/* ------------------------------------------------------------- */
/* a) Mots contenant les 5 voyelles a, e, i, o, u dans cet ordre */
/* ------------------------------------------------------------- */
.*a.*e.*i.*o.*u.*          { printf("Mot contenant les 5 voyelles dans l'ordre : %s\n", yytext); }

/* ------------------------------------------------------------- */
/* b) Mots sur {a,b} commençant ou finissant par a               */
/* ------------------------------------------------------------- */
^(a(a|b)*)|((a|b)*a)$      { printf("Mot commençant ou finissant par 'a' : %s\n", yytext); }

/* ------------------------------------------------------------- */
/* c) Mots sur {a,b} sans deux occurrences consécutives du même caractère */
/* ------------------------------------------------------------- */
/* Exemples valides : ababab, baba, ab, ba, a, b */
^((ab)*|(ba))|[ab]$               { printf("Mot sans deux lettres identiques consécutives : %s\n", yytext); }

/* ------------------------------------------------------------- */
/* d) Mots sur {a,b} ne contenant pas la sous-chaîne 'aba'       */
/* ------------------------------------------------------------- */
/* Exemple valide : abba, bbab, bbb, mais pas ababa ni abaa */
^([^a]|a(b[^a])*)$         { printf("Mot ne contenant pas la sous-chaîne 'aba' : %s\n", yytext); }

/* ------------------------------------------------------------- */
/* Tout autre mot non reconnu                                    */
/* ------------------------------------------------------------- */
[a-zA-Z]+                  { printf("Mot non reconnu : %s\n", yytext); }
[\n\t ]+                   ;   /* Ignorer espaces et sauts de ligne */
.                          ;   /* Ignorer tout autre caractère */
%%

int main() {
    printf("=== Test des expressions régulières ===\n");
    yylex();
    return 0;
}
