#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include "../includes/model.h"

typedef enum
{
    FIRSTPOSARRAY,
    LASTPOSARRAY,
    FOLLOWPOSARRAY
} arrayType;

#define ARRAYLENGTH 20
#define DSTATEARRAYSIZE 50
#define FLPOSSIZE 10
#define TRANSITIONTABLESIZE 50
#define NULLSTATE 99

typedef struct State
{
    int stateID;
    int isMarked;
    int *symbolPos;

} State;

int generateDFA(NodeType *root);

// Thompson DFA Methods
int markNullable(NodeType *root);
int *generateFirstPos(NodeType *root);
int *generateLastPos(NodeType *root);
void generateFollowPos(NodeType *root, int **);
int **generateDFATable(NodeType *root, int **followPos);

// Thomson DFA Helpers
int markPos(NodeType *root, int *pos);
void mergeSets(int *left, int *right, int *dest);
void allocateArray(NodeType *root, int type);
void generateSymbolArray(NodeType *root, int *symbolArray, int *mapSym2Pos, int *index);
void generateUnqSymArray(int *array, int size, int *uniqueArray, int *unqsize);
void makeUnion(int *u, int *uSize, int *currentStatePos, int *symbolArray, int symbolArraySize, int *map2SymPos, int symbol, int **followPos);
int checkUandStateArrays(int *u, int uSize, State **stateArray, int stateArraySize);
int compare(const void *a, const void *b);
int compareArray(int *a, int *b, int size);
#endif