#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include "../includes/model.h"

typedef enum
{
    FIRSTPOSARRAY,
    LASTPOSARRAY,
    FOLLOWPOSARRAY
}arrayType;

int generateDFA(NodeType *root);

// Thompson DFA Methods
int markNullable(NodeType *root);
int* generateFirstPos(NodeType *root);
int* generateLastPos(NodeType *root);
void generateFollowPos(NodeType *root, int **);


// Thomson DFA Helpers
int markPos(NodeType *root, int *pos);
void mergeSets(int *left, int *right, int *dest);
void allocateArray(NodeType *root,int type);

#endif