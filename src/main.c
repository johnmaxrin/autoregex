#include <stdio.h>
#include "../includes/model.h"
#include "../build/y.tab.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int main()
{
    char *string;
    string = (char *)malloc(20);
    YY_BUFFER_STATE buffer;
    
    while (1)
    {
        scanf("%s", string);
        yy_scan_string(string);
        yyparse();
    }


    yy_delete_buffer(buffer);
    free(string);
    
    

    return 0;
}