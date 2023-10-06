#include <stdlib.h>
#include <string.h>

#ifndef MAIN_HEADER
#define MAIN_HEADER

// Structures
typedef struct Node
{
    struct Node *left;
    struct Node *right;

    int isNull;
    char data;
    int type;

} Node;

typedef enum
{
    CHAR,
    STAR,
    OR,
    WILD,
    END,
    CONCAT
} NODETYPES;

// Regex Functions
void augment(char *, char *);
Node *makeSyntaxTree(char *);
void makeNull(Node *);

#endif