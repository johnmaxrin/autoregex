%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/model.h"
#include"../build/y.tab.h"


extern int yylex();
void yyerror(char *s);

%}


%union{
    NodeType *node;
    char data;
}

%type <node> regex augment expr term unit atom
%token <data> CHAR WILD STAR OR END



%%

regex :                 augment                 {traverse($1);}     
                        ;

augment :               expr '#'                {
                                                    NodeType *node = catNode();
                                                    node->concatNode.left = $1;
                                                    node->concatNode.right = charNode('#');
                                                    $$ = node;

                                                }
                        ;    

expr :                  term                    {$$ = $1;}       
                        | expr OR term          {
                                                   

                                                   NodeType *node = orNode();
                                                   node->orNode.left = $1;
                                                   node->orNode.right = $3;
                                                   $$ = node;

                                                }
                        ;              

term :                  unit                    {$$ = $1;}
                        | term unit             {
                                                    NodeType *node = catNode();
                                                    node->concatNode.left = $1;
                                                    node->concatNode.right = $2;
                                                    $$ = node;
                                                }
                        ;                 

unit :                  atom STAR               {
                                                   
                                                   NodeType *node = starNode();
                                                   node->starNode.prevNode = $1;
                                                   $$ = node;
                                                    
                                                }
                        | atom                  {$$ = $1;}
                        ;

atom :                  CHAR                    {$$ = charNode($1);}       
                        | WILD                  {$$ = wildNode();}

%%




void yyerror(char *s)
{
    fprintf(stderr,"Parse Error:%s\n",s);
}

