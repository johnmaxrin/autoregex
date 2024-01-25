#include <stdio.h>
#include <string.h>
#include "../includes/model.h"
#include "../build/y.tab.h"

#include "../includes/regexapi.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;

YY_BUFFER_STATE buffer;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int **dfaTable;
int *finalArray;
int *finalArraySize;
int *uniqueArray;
int *uniqueSize;

int regexapi(char *pattern, char *string)
{
    char augmentChar = '#', nullChar = '\0';

    int res = 0;
    int flag = 0;
    char *temparr;
    temparr = pattern;
    // Augmenting the pattern.
    while (*pattern != '\0')
    {
        if (*pattern == '#')
        {
            flag = 1;
            break;
        }

        ++pattern;
    }

    pattern = temparr;

    if (!flag)
    {
        strcat(pattern, &augmentChar);
        strcat(pattern, &nullChar);
    }

    yy_scan_string(pattern);
    yyparse();

    res = match(pattern, string);

    yy_delete_buffer(buffer);

    return res;
}

int match(char *pattern, char *string)
{

    int isWild = 0;
    for (int i = 0; i < *uniqueSize; ++i)
    {
        if (uniqueArray[i] == 0)
        {
            isWild = 1;
            //printf("It's wild!\n");
            break;
        }
    }

    // With Wildcard
    // Issue with this approach
    // 1. If .*a is the pattern, it cannot match aa!
    if (isWild)
    {

        for (int i = 0; i < 20; ++i)
        {
            if (dfaTable[i][0])
            {
                for (int j = 1; j < 76; ++j)
                {
                    if (!dfaTable[i][j])
                        dfaTable[i][j] = dfaTable[i][0];
                }
            }
        }
    }

    // Debug Prints

    
    // -------- Debug print done! -----------  */

    // If no wildcard is there! do this
    int currentState = 1;
    for (int s = 0; string[s] != '\0'; ++s)
    {
        currentState = dfaTable[currentState][string[s] - '.'];
    }

    //printf("String: %s\nFinal state: %d\n", string, currentState);

    for (int i = 0; i < *finalArraySize; ++i)
    {
        if (currentState == finalArray[i])
            return 1;
    }

    return 0;
}