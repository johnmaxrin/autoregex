#ifndef REGEX_API
#define REGEX_API
#include "../includes/cache.h"

int match(char *, int, Cache *cache);
int regexapi(char *pattern, char *string, Cache *cache);

#endif
