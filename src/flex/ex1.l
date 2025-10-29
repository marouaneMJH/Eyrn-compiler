%{
#include <stdio.h>
%}

%option noyywrap

%%


[a-zA-Z][a-zA-Z0-9]* {printf("<%s>",yytext);}
.[0-9]* {printf("<%s>",yytext);}


%%
printf("<%s>",yytext

int main()
{
    yylex();
    return 0;
}

