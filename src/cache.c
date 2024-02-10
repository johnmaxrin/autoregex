#include "../includes/cache.h"
#include "../includes/helpers.h"

// Cache logic starts

// Globals
int **dfaTable;
int *finalArray;
int *finalArraySize;
int *uniqueArray;
int *uniqueSize;
// End Globals

extern Cache *cache; // Global cache

int createEntry(const char *pattern)
{

    int idx = -1;

    if ((idx = patternFound(pattern)) != -1)
        return idx;

    else
    {
        // Create DFA Table
        // Store it in cache
        // Do the matching. Make it critical.

        #pragma omp critical
        {
            int **dfaTable;
            // int **dfaTable; [20x76]
            dfaTable = calloc(TRANSITIONTABLESIZE, sizeof(int *));
            for (int i = 0; i < TRANSITIONTABLESIZE; ++i)
                dfaTable[i] = calloc(CHARACTERSUPPORT, sizeof(int));

            yyscan_t *scanner;
            if (yylex_init(&scanner))
                exit(1);

            YY_BUFFER_STATE buf = yy_scan_string(pattern, scanner);
            if (yyparse(scanner,dfaTable))
                exit(1);
            // Creating new entry
            CacheEntry newEntry;

            strcpy(newEntry.pattern, pattern);

            for (int i = 0; i < 20; ++i)
                for (int j = 0; j < 76; ++j)
                    newEntry.dfaTable[i][j] = dfaTable[i][j];

            for (int i = 0; i < 20; ++i)
            {
                newEntry.finalArray[i] = finalArray[i];
                newEntry.uniqueArray[i] = uniqueArray[i];
            }

            newEntry.finalArraySize = *finalArraySize;
            newEntry.uniqueSize = *uniqueSize;
            // End of creation of new entry.

            // Add to cache
            cache->entries[cache->size++] = newEntry;

            // End of add to cache
            
            for (int i = 0; i < TRANSITIONTABLESIZE; ++i)
                free(dfaTable[i]);
            free(dfaTable);

            /*free(finalArray);
            free(finalArraySize);
            free(uniqueArray);
            free(uniqueSize); 
            yy_delete_buffer(buf, scanner);
            yylex_destroy(scanner); */
        }
        // End of Critical Section

        return cache->size - 1;
    }
}

int patternFound(const char *pattern)
{
    int flag = -1;

    if (!cache)
    {
        printf("Cache Null!\n");
        exit(0);
    }

    for (int i = 0; i < cache->size; ++i)
    {

        if (!strcmp(cache->entries[i].pattern, pattern))
        {

            flag = i;
            break;
        }
    }

    return flag;
}

// Cache logic ends
