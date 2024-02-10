
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYSTYPE char 
#include "../includes/model.h"
#include "../build/y.tab.h"

%}

%option reentrant bison-bridge 
 

%%
[a-z A-Z 0-9]     {*yylval = yytext[0]; return CHAR;}
\|                {*yylval = yytext[0]; return OR;}  
\.                {*yylval = yytext[0]; return WILD;}
\*                {*yylval = yytext[0]; return STAR;}
\#                {return *yytext;}
[()]              {return *yytext;}
.                 {;}
%%

int yywrap(yyscan_t yyscanner)
{
return 1;
}