#include <stdio.h>
#include "../includes/main.h"
#include "../includes/helpers.h"

int main(int c, char *argv[])
{
    int patternLen = 0, finalState = 0;
    if (c > 1)
    {

        // Generate table for the pattern
        char augmentedPattern[50];
        // char pattern[50] = "aa*";
        augment(argv[1], augmentedPattern);
        Node *root = makeSyntaxTree(augmentedPattern);

        traverseTree(root);
    }

    else
        printf("Format is ./a.out <add here>\n");

    return 0;
}

void augment(char *pattern, char *augmentedPattern)
{

    int len = strlen(pattern);
    pattern[len] = '#';
    pattern[len + 1] = '\0';
    strcpy(augmentedPattern, pattern);
}

Node *makeSyntaxTree(char *pattern)
{

    int len = strlen(pattern), i = 0;
    Node *currentNode = NULL;

    while (i < len)
    {

        // Initial CHAR
        if (currentNode == NULL)
        {
            if (pattern[i] == '.')
            {
                if (pattern[i + 1] == '*')
                {
                    currentNode = makeNode('*', STAR, 1);
                    currentNode->left = makeNode('.', WILD, 0);
                    ++i;
                }
                else
                    currentNode = makeNode(pattern[i], WILD, 0);
            }
            else if (isAlpha(pattern[i]))
            {
                if (pattern[i + 1] == '*')
                {
                    currentNode = makeNode('*', STAR, 1);
                    currentNode->left = makeNode(pattern[i], CHAR, 0);
                    ++i;
                }

                else
                    currentNode = makeNode(pattern[i], CHAR, 0);
            }
            else
            {
                printf("Wrong regular expression syntax\n");
                exit(0);
            }
        }

        else
        {

            if (pattern[i] == '.')
            {
                if (pattern[i + 1] == '*')
                {
                    Node *temp1 = makeNode('*', STAR, 1);
                    temp1->left = makeNode('.', WILD, 0);

                    Node *concat = makeNode('O', CONCAT, 0);
                    concat->left = currentNode;
                    concat->right = temp1;
                    currentNode = concat;
                    ++i;
                }

                else
                {
                    Node *concat = makeNode('O', CONCAT, 0);
                    concat->left = currentNode;
                    concat->right = makeNode(pattern[i], WILD, 0);
                    currentNode = concat;
                }
            }
            else if (isAlpha(pattern[i]))
            {
                if (pattern[i + 1] == '*')
                {
                    printf("Entered\n");
                    Node *temp1 = makeNode('*', STAR, 1);
                    temp1->left = makeNode(pattern[i], CHAR, 0);

                    Node *concat = makeNode('O', CONCAT, 0);
                    concat->left = currentNode;
                    concat->right = temp1;
                    currentNode = temp1;
                    ++i;
                }

                else
                {
                    Node *concat = makeNode('O', CONCAT, 0);
                    concat->left = currentNode;
                    concat->right = makeNode(pattern[i], CHAR, 0);
                    currentNode = concat;
                }
            }
            else if (pattern[i] == '#')
            {
                if (pattern[i + 1] == '*')
                {
                    Node *temp1 = makeNode('*', STAR, 1);
                    temp1->left = makeNode('#', END, 0);

                    Node *concat = makeNode('O', CONCAT, 0);
                    concat->left = currentNode;
                    concat->right = temp1;
                    currentNode = temp1;
                    ++i;
                }
                else
                {
                    Node *concat = makeNode('O', CONCAT, 0);
                    concat->left = currentNode;
                    concat->right = makeNode('#', END, 0);
                    currentNode = concat;
                }
            }

            else if (pattern[i] == '|')
            {
                if (pattern[i + 1] == '*')
                {
                    printf("Regular expression syntax error!\n");
                    exit(0);
                }

                else
                {

                    Node * or = makeNode('|', OR, 0);
                    Node *temp;
                    or->left = currentNode;

                    if (pattern[i + 1] == '.')
                    {
                        if (pattern[i + 2] == '*')
                        {
                            temp = makeNode('*', STAR, 1);
                            temp->left = makeNode('.', WILD, 0);
                            or->right = temp;
                            currentNode = or ;
                            ++i;
                        }

                        else
                        {
                            temp = makeNode('.', WILD, 0);
                            or->right = temp;
                            currentNode = or ;
                        }

                        ++i;
                    }

                    else if (isAlpha(pattern[i + 1]))
                    {
                        if (pattern[i + 2] == '*')
                        {
                            temp = makeNode('*', STAR, 1);
                            temp->left = makeNode(pattern[i + 1], CHAR, 0);
                            or->right = temp;
                            currentNode = or ;
                            ++i;
                        }
                        else
                        {
                            temp = makeNode(pattern[i + 1], CHAR, 0);
                            or->right = temp;
                            currentNode = or ;
                        }
                        ++i;
                    }
                }
            }
        }

        ++i;
    }

    return currentNode;
}