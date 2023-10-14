%{

#include <stdio.h>
#include <stdlib.h>
#include "../includes/model.h"
#include "../build/y.tab.h"


%}


%%
[a-z A-Z 0-9]     {yylval.data = yytext[0];  return CHAR;}
\|                {yylval.data = yytext[0];  return OR;}  
\.                {yylval.data = yytext[0]; return WILD;}
\*                {yylval.data = yytext[0]; return STAR;}
\#                {return yytext[0];}
.                 {;}
%%


int yywrap()
{
    return(1);
}