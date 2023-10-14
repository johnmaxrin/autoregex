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
        printf("%c",node->charNode.value);
        break;

    case TYPEWILD:
        printf(".");
        break;

    case TYPECONCAT:
        traverse(node->concatNode.left);
        traverse(node->concatNode.right);
        break;

    case TYPEOR:
        traverse(node->orNode.left);
        printf("|");
        traverse(node->orNode.right);
        break;
    
    case TYPESTAR:
        traverse(node->starNode.prevNode);
        printf("*");
        break;


    default:
        printf("Poi valla paniyum edukeda\n");
        break;
    }
    

}