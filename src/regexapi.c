#include <stdio.h>
#include <string.h>
#include "../includes/model.h"
#include "../build/y.tab.h"
#include "../includes/regexapi.h"
#include "../includes/cache.h"


// Globals 
extern int *finalArray;
extern int *finalArraySize;
extern int *uniqueArray;
extern int *uniqueSize;
// End Globals

extern Cache *cache;



int regexapi(char *pattern, char *string)
{
    if (!cache)
    {
       printf("Please initialize Global Cache\n");
       exit(0); 
    }   
    
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

  //  x = createEntry(pattern); if pattern is present returns the table index
  //  else adds it to the table and returns the index.

  //  match(string,x);
  //   
    
    int idx = createEntry(pattern);
    res = match(string,idx);

    return res;
}

int match(char *string, int idx)
{

    /*
        for(int i=0; i<20; ++i)
        {
            for(int j=0; j<76; ++j)
                printf("%d ",dfaTable[i][j]);
            printf("\n");
        }
    */

    int isWild = 0;
    for (int i = 0; i < cache->entries[idx].uniqueSize; ++i)
    {
        if (cache->entries[idx].uniqueArray[i] == 0)
        {
            isWild = 1;
            // printf("It's wild!\n");
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
            if (cache->entries[idx].dfaTable[i][0])
            {
                for (int j = 1; j < 76; ++j)
                {
                    if (!cache->entries[idx].dfaTable[i][j])
                        cache->entries[idx].dfaTable[i][j] = cache->entries[idx].dfaTable[i][0];
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
        currentState = cache->entries[idx].dfaTable[currentState][string[s] - '.'];
    }

    // printf("String: %s\nFinal state: %d\n", string, currentState);

    for (int i = 0; i < cache->entries[idx].finalArraySize; ++i)
    {
        if (currentState == cache->entries[idx].finalArray[i])
            return 1;
    }

    return 0;
}