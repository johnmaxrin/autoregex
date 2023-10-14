#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>

typedef enum 
{
    TYPECONCAT,
    TYPEOR,
    TYPEWILD,
    TYPECHAR,
    TYPESTAR
} nodeTypeEnum;




// Char
typedef struct charNodeTypeTag 
{
    char value;
}charNodeType;

// Wild
typedef struct wildNodeTypeTag
{
    char value;
}wildNodeType;

// Star
typedef struct starNodeTypeTag
{
    char value;
    struct nodeTypeTag *prevNode;
}starNodeType;

// Or
typedef struct orNodeTypeTag
{
    char value;
    struct nodeTypeTag *left;
    struct nodeTypeTag *right;

}orNodeType;

// Concat
typedef struct concatNodeTypeTag
{
    char value;
    struct nodeTypeTag *left;
    struct nodeTypeTag *right;
}concatNodeType;

typedef struct nodeTypeTag
{
    nodeTypeEnum type;

    union 
    {
        charNodeType charNode;
        wildNodeType wildNode;
        starNodeType starNode;
        orNodeType orNode;
        concatNodeType concatNode;
    };
    
}NodeType;



// Function Declaration

NodeType* charNode(char);
NodeType* wildNode();
NodeType* starNode();
NodeType* orNode();
NodeType* catNode();

NodeType* traverse(NodeType *);


#endif