#include "../includes/helpers.h"

int generateDFA(NodeType *root)
{
    int val = 15;
    int *pos = (int *)malloc(sizeof(int));
    *pos = 0;

    int **followPos = calloc(val, sizeof(int *));

    for (int j = 0; j < 15; ++j)
        followPos[j] = calloc(val, sizeof(int));

    markPos(root, pos);
    markNullable(root);
    generateFirstPos(root);
    generateLastPos(root);
    generateFollowPos(root, followPos);

    generateDFATable(root, followPos);

    /*for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
            printf("%d ", followPos[i][j]);

        printf("\n");
    }*/

    // createSymbolListArray(root);
    //  traverse(root);
    return 0;
}

int markNullable(NodeType *root)
{

    if (!root)
        return 0;
    switch (root->type)
    {
    case TYPEWILD:
        root->isNullable = 1;
        return 1;
        break;

    case TYPECHAR:
        root->isNullable = 0;
        return 0;
        break;

    case TYPESTAR:
        root->isNullable = 1;
        return 1;
        break;

    case TYPEOR:
        markNullable(root->orNode.left);
        markNullable(root->orNode.right);
        root->isNullable = (root->orNode.left->isNullable) || (root->orNode.right->isNullable);
        return root->isNullable;
        break;

    case TYPECONCAT:
        markNullable(root->concatNode.left);
        markNullable(root->concatNode.right);
        root->isNullable = (root->concatNode.left->isNullable) && (root->concatNode.right->isNullable);
        return root->isNullable;
        break;

    default:
        break;
    }
}

int *generateFirstPos(NodeType *root)
{
    if (!root)
        return NULL;

    switch (root->type)
    {
    case TYPECHAR:
        allocateArray(root, FIRSTPOSARRAY);
        root->firstPos[0] = root->position;
        return root->firstPos;
        break;

    case TYPEWILD:
        allocateArray(root, FIRSTPOSARRAY);
        root->firstPos[0] = root->position;
        return root->firstPos;
        break;

    case TYPESTAR:
        allocateArray(root, FIRSTPOSARRAY);
        int *starSet = generateFirstPos(root->starNode.prevNode);
        root->firstPos = starSet;
        return root->firstPos;

    case TYPEOR:
        allocateArray(root, FIRSTPOSARRAY);
        int *leftset = generateFirstPos(root->orNode.left);
        int *rightset = generateFirstPos(root->orNode.right);
        mergeSets(leftset, rightset, root->firstPos);
        return root->firstPos;

    case TYPECONCAT:
        allocateArray(root, FIRSTPOSARRAY);
        int *concatleftset = generateFirstPos(root->concatNode.left);
        int *concatrightset = generateFirstPos(root->concatNode.right);

        if (root->concatNode.left->isNullable)
        {
            mergeSets(concatleftset, concatrightset, root->firstPos);
            return root->firstPos;
        }
        else
        {
            root->firstPos = concatleftset;
            return root->firstPos;
        }

    default:
        return 0;
        break;
    }
    return 0;
}

int *generateLastPos(NodeType *root)
{
    if (!root)
        return NULL;

    switch (root->type)
    {
    case TYPECHAR:
        allocateArray(root, LASTPOSARRAY);
        root->lastPos[0] = root->position;
        return root->lastPos;
        break;

    case TYPEWILD:
        allocateArray(root, LASTPOSARRAY);
        root->lastPos[0] = root->position;
        return root->lastPos;
        break;

    case TYPESTAR:
        allocateArray(root, LASTPOSARRAY);
        int *starSet = generateLastPos(root->starNode.prevNode);
        root->lastPos = starSet;
        return root->lastPos;

    case TYPEOR:
        allocateArray(root, LASTPOSARRAY);
        int *leftset = generateLastPos(root->orNode.left);
        int *rightset = generateLastPos(root->orNode.right);
        mergeSets(leftset, rightset, root->lastPos);
        return root->lastPos;

    case TYPECONCAT:
        allocateArray(root, LASTPOSARRAY);
        int *concatleftset = generateLastPos(root->concatNode.left);
        int *concatrightset = generateLastPos(root->concatNode.right);

        if (root->concatNode.right->isNullable)
        {
            mergeSets(concatleftset, concatrightset, root->lastPos);
            return root->lastPos;
        }
        else
        {
            root->lastPos = concatrightset;
            return root->lastPos;
        }

    default:
        break;
    }
    return 0;
}

void generateFollowPos(NodeType *root, int **followPos)
{

    if (!root)
        return;

    switch (root->type)
    {
    case TYPECONCAT:
        int *c1LastPos = root->concatNode.left->lastPos;
        int *c2FirstPos = root->concatNode.right->firstPos;

        int i = 0, j = 0, k = 0;

        while (c1LastPos[i] != 0)
        {
            j = 0;
            k = 0;

            while (followPos[c1LastPos[i]][k++] != 0)
                ;

            --k;

            while (c2FirstPos[j] != 0)
            {
                followPos[c1LastPos[i]][k] = c2FirstPos[j];
                ++k;
                ++j;
            }

            ++i;
        }
        generateFollowPos(root->concatNode.left, followPos);
        generateFollowPos(root->concatNode.right, followPos);
        break;

    case TYPESTAR:

        int *firsPos = root->firstPos;
        int *lastPos = root->lastPos;

        int i2 = 0, k2 = 0, j2 = 0;

        while (lastPos[i2] != 0)
        {
            k2 = 0;
            while (followPos[lastPos[i2]][k2++] != 0)
                ;

            --k2;
            j2 = 0;
            while (lastPos[j2] != 0)
            {
                followPos[lastPos[i2]][k2] = firsPos[j2];
                ++k2;
                ++j2;
            }
            ++i2;
        }
        generateFollowPos(root->starNode.prevNode, followPos);
        break;

    case TYPEOR:

        generateFollowPos(root->orNode.left, followPos);
        generateFollowPos(root->orNode.right, followPos);
        break;

    default:
        break;
    }
}

void generateDFATable(NodeType *root, int **followPos)
{

    int *uniqueSymbolArray, *symbolArray;

    int size = root->concatNode.right->position + 1;
    symbolArray = calloc(size, sizeof(int));
    uniqueSymbolArray = calloc(size, sizeof(int));

    int uniqueSymbolSize = createSymbolListArrays(root, uniqueSymbolArray, symbolArray);
    // int *pos2sym = createPos2SymbolArray(root,symbolList);

    int found = 0;

    State *initialState = (State *)malloc(sizeof(State));
    initialState->stateID = 0;
    initialState->isMarked = 0;
    initialState->symbolPos = root->firstPos;


    State *Dstates[DSTATEARRAYSIZE]; // Optimize
    int DstatesCount = 0;
    Dstates[DstatesCount++] = initialState;

    while (1)
    {
        State *currentState = NULL;
        for (int i = 0; i < DstatesCount; ++i)
        {
            if (!Dstates[i]->isMarked)
            {
                currentState = Dstates[i];
                currentState->isMarked = 1;
                break;
            }
        }

        if (currentState == NULL)
            break;

        for (int i = 0; i < uniqueSymbolSize; ++i)
        {
            // Calculate union of followPos(ipsymbols[i])
            // which are in currentState
            int *U = makeNewState(1, currentState->symbolPos, symbolArray, followPos);
            break;
        }
    }
}

// Helpers

int markPos(NodeType *root, int *pos)
{
    if (!root)
        return 0;

    switch (root->type)
    {
    case TYPECHAR:
        root->position = ++(*pos);
        break;

    case TYPEWILD:
        root->position = ++(*pos);
        break;

    case TYPESTAR:
        markPos(root->starNode.prevNode, pos);
        break;

    case TYPECONCAT:
        markPos(root->concatNode.left, pos);
        markPos(root->concatNode.right, pos);
        break;

    case TYPEOR:
        markPos(root->orNode.left, pos);
        markPos(root->orNode.right, pos);
        break;

    default:
        printf("Error at MarkPos Function\n");
        exit(0);
        break;
    }
}

void allocateArray(NodeType *node, int type)
{
    switch (type)
    {
    case FIRSTPOSARRAY:
        node->firstPos = (int *)malloc(sizeof(int) * FLPOSSIZE);
        for (int i = 0; i < 10; ++i)
            node->firstPos[i] = 0;
        break;

    case LASTPOSARRAY:
        node->lastPos = (int *)malloc(sizeof(int) * FLPOSSIZE);
        for (int i = 0; i < 10; ++i)
            node->lastPos[i] = 0;
        break;

    default:
        break;
    }
}

void mergeSets(int *left, int *right, int *dest)
{
    int ls = 0, rs = 0, isCopy = 0, k = 0;
    while (left[ls] != 0)
        ++ls;

    if (right != NULL)
        while (right[rs] != 0)
            ++rs;

    for (int i = 0; i < ls; ++i)
        dest[i] = left[i];

    k = ls;
    if (right != NULL)
        for (int i = 0; i < rs; ++i)
        {
            isCopy = 0;
            for (int j = 0; j < ls; ++j)
            {
                if (right[i] == left[j])
                    isCopy = 1;
                break;
            }

            if (!isCopy)
                dest[k++] = right[i];
        }
}

void merge(int *mergeArray, int *array1) // Global
{

    int size0 = 0, size1 = 0;

    while (mergeArray[size0++] != 0)
        ;

    --size0;

    while (array1[size1++] != 0)
        ;
    for (int i = 0; i < size1; ++i)
    {
        if (!isDuplicate(mergeArray, array1[i]))
            mergeArray[size0++] = array1[i];
    }
}

int *makeunion(int nargs, int *tempArray, int **followPos)
{

    int *sentArray = (int *)calloc(ARRAYLENGTH, sizeof(int));
    for (int i = 0; i < nargs; ++i)
        merge(sentArray, followPos[tempArray[i]]);

    return sentArray;
}

int isDuplicate(int *array, int element)
{
    int size = 0, isDupli = 0;
    while (array[size++] != 0)
        ;
    --size;
    for (int i = 0; i < size; ++i)
    {
        if (array[i] == element)
        {
            isDupli = 1;
            break;
        }
    }

    return isDupli;
}

int createSymbolListArrays(NodeType *root, int *uniqueArray, int *array)
{
    int size = root->concatNode.right->position + 1;

    int uniqueArraySize = 0;

    int encountered[size];
    for (int i = 0; i < size; ++i)
        encountered[i] = 0;
    traverse4SymList(root, array);

    for (int i = 0; i < size; i++)
    {
        if (!encountered[i])
        {
            uniqueArray[uniqueArraySize] = i;
            uniqueArraySize++;

            encountered[i] = 1;

            for (int j = i + 1; j < size; j++)
            {
                if (array[j] == array[i])
                    encountered[j] = 1;
            }
        }
    }

    return uniqueArraySize - 1;
}

void traverse4SymList(NodeType *root, int *array)
{
    if (!root)
        return;

    switch (root->type)
    {
    case TYPECHAR:
        array[root->position] = root->charNode.value - '0';
        break;

    case TYPEWILD:
        array[root->position] = '.' - '0';
        break;

    case TYPECONCAT:
        traverse4SymList(root->concatNode.left, array);
        traverse4SymList(root->concatNode.right, array);
        break;

    case TYPEOR:
        traverse4SymList(root->orNode.left, array);
        traverse4SymList(root->concatNode.right, array);
        break;

    case TYPESTAR:
        traverse4SymList(root->starNode.prevNode, array);
        break;

    default:
        break;
    }
}

int *makeNewState(int symbol, int *statePosArray, int *symbolArray, int **followPos)
{

    int StatePosArraySize = 0;
    for (int i = 0; i < 5; ++i)
        printf("%d ", statePosArray[i]);

    return NULL;
}

// @TODO Today
// Build DFA Table
// Match Strings