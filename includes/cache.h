#ifndef CACHE_HEADER
#define CACHE_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* yyscan_t;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yylex_init (yyscan_t* scanner);
extern int yylex_destroy (yyscan_t scanner);
extern YY_BUFFER_STATE yy_scan_string (const char * str , yyscan_t scanner);
extern void yy_delete_buffer (YY_BUFFER_STATE buf , yyscan_t scanner );
extern int yyparse(void* scanner, int **dfaTable);


typedef struct CacheEntry
{
    char pattern[20];
    int dfaTable[20][76];
    int finalArray[20];
    int uniqueArray[20];
    int uniqueSize;
    int finalArraySize;
} CacheEntry;

typedef struct Cache
{
    int size;
    int hit;
    int miss;
    CacheEntry entries[100];
} Cache;



int createEntry(const char *pattern, Cache *cache); // Returns the entry index.
int patternFound(const char *pattern, Cache *cache);


#endif
