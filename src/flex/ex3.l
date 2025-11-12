%{
#include <stdio.h>
%}
%option noyywrap
%%

.*a.*e.*i.*o.*u.*          { printf("Mot contenant les 5 voyelles dans l'ordre : %s\n", yytext); }

^(a(a|b)*)|((a|b)*a)$      { printf("Mot commençant ou finissant par 'a' : %s\n", yytext); }
^((ab)*|(ba))|[ab]$               { printf("Mot sans deux lettres identiques consécutives : %s\n", yytext); }
^([^a]|a(b[^a])*)$         { printf("Mot ne contenant pas la sous-chaîne 'aba' : %s\n", yytext); }

[a-zA-Z]+                  { printf("Mot non reconnu : %s\n", yytext); }

[\n\t ]+                   ;   /* Ignorer espaces et sauts de ligne */

.                          ;   /* Ignorer tout autre caractère */
%%

int main() {
    printf("=== Test des expressions régulières ===\n");
    yylex();
    return 0;
}
