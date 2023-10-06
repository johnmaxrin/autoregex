#include "../includes/helpers.h"


Node *makeNode(char data, int type, int isNull)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->type = type;
    node->isNull = isNull;
    return node;
}

void traverseTree(Node *root)
{
    if (root != NULL)
    {
        traverseTree(root->left);
        if (root->type != CONCAT)
            printf("%c ", root->data);
        traverseTree(root->right);
    }
}

int isAlpha(char a)
{
    if ((a >= 'a' && a <= 'z'))
        return 1;
    else
        return 0;
}