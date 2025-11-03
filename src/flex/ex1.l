%{
#include <stdio.h>
%}

%option noyywrap

%%
"/*"([^*]|\*+[^*/])*\*+"/"   { /* ignorer commentaire */ }

"for"|"if"|"while"|"else"|"switch"|"break"|"continue"       { printf("<mot-clé:%s>", yytext); }

"="|"=="|"+"|"-"|"*"|"/"        { printf("<op:%s>", yytext); }
"("|")"|"{"|"}"                 { printf("<sym:%s>", yytext); }

[a-zA-Z][a-zA-Z0-9]*            { printf("<id:%s>", yytext); }
[0-9]*(\.[0-9]+)?               { printf("<num:%s>", yytext); }
[\n\t ]+                        { printf("%s", yytext);}     /* ignorer espaces et sauts de ligne. pour le moment en lesser le pourr voir le content */

%%


int main()
{
    yylex();
    return 0;
}

