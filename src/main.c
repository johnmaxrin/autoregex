#include <stdio.h>
#include <string.h>
#include "../includes/model.h"
#include "../build/y.tab.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

int **dfaTable;
int *finalArray;
int *finalArraySize;
int *uniqueArray;
int *uniqueSize;
void match(char *, char *);

int main()
{
    char *string, *pattern, augmentChar = '#', nullChar = '\0';
    string = (char *)malloc(20);
    pattern = (char *)malloc(20);

    YY_BUFFER_STATE buffer;

    while (1)
    {
        printf("Enter a pattern\n");
        scanf("%s", pattern);

        // Augmenting the pattern.
        strcat(pattern, &augmentChar);
        strcat(pattern, &nullChar);

        printf("\nEnter a String to be matched\n");
        scanf("%s", string);

        yy_scan_string(pattern);
        yyparse();

        match(pattern, string);
    }

    yy_delete_buffer(buffer);
    free(string);

    return 0;
}

void match(char *pattern, char *string)
{

    int isWild = 0;
    for (int i = 0; i < *uniqueSize; ++i)
    {
        if (uniqueArray[i] == '.' - '.')
        {
            isWild = 1;
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
                    if(!dfaTable[i][j])
                        dfaTable[i][j] = dfaTable[i][0];
                }
            }
        }
    }


    // If no wildcard is there! do this
    int currentState = 1;
    for (int s = 0; string[s] != '\0'; ++s)
    {
        for (int i = 0; i < *uniqueSize; ++i)
        {
            if (uniqueArray[i] == string[s] - '.')
                currentState = dfaTable[currentState][uniqueArray[i]];
        }
    }

    for (int i = 0; i < *finalArraySize; ++i)
    {
        if (currentState == finalArray[i])
        {
            printf("Match\n\n");
            return;
        }
    }

    printf("Not Match!\n\n");
}