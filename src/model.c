#include "../includes/model.h"
#include <stdio.h>
#include <string.h>

NodeType* charNode(char data)
{   

    NodeType *node;
    node = (NodeType *) malloc(sizeof(NodeType));
    node->type = TYPECHAR;

    charNodeType charNode;
    charNode.value = data;

    node->charNode = charNode;

    return node;
}

NodeType* wildNode()
{
    NodeType *node; 
    node = (NodeType *) malloc(sizeof(NodeType));
    node->type = TYPEWILD;

    wildNodeType wildNode;
    node->wildNode = wildNode;

    return node;
}

NodeType* orNode()
{
    NodeType *node;
    node = (NodeType *) malloc(sizeof(NodeType));
    node->type = TYPEOR;

    orNodeType orNode;

    node->orNode = orNode;

    return node;
}

NodeType* starNode()
{   

    NodeType *node;
    node = (NodeType *) malloc(sizeof(NodeType));
    node->type = TYPESTAR;

    starNodeType starNode;
    node->starNode = starNode;

    return node;
}

NodeType* catNode()
{
    NodeType *node;
    node = (NodeType *) malloc(sizeof(NodeType));
    node->type = TYPECONCAT;

    concatNodeType catNode;
    node->concatNode = catNode;

    return node;
}

NodeType* traverse(NodeType *node)
{

    if(!node)
        return 0;
    
    switch (node->type)
    {

    case TYPECHAR:
        printf("%c %d\n",node->charNode.value,node->position);
        break;

    case TYPEWILD:
        printf(". %d\n",node->position);
        break;

    case TYPECONCAT:
        traverse(node->concatNode.left);
        traverse(node->concatNode.right);
        for(int i=0; i<10; ++i)
            printf("%d ",node->firstPos[i]);
        printf("\n LAST POST\n");
        for(int i=0; i<10; ++i)
            printf("%d ",node->lastPos[i]);
        printf("\n");
        
        break;

    case TYPEOR:
        traverse(node->orNode.left);
        printf("|\n");
        traverse(node->orNode.right);
        break;
    
    case TYPESTAR:
        traverse(node->starNode.prevNode);
        printf("*\n");
        break;


    default:
        printf("Poi valla paniyum edukeda\n");
        break;
    }
    

}