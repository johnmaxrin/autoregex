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

    return 0;
}

int markNullable(NodeType *root)
{

    if (!root)
        return 0;
    switch (root->type)
    {
    case TYPEWILD:
        root->isNullable = 0;
        return 0;
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

int **generateDFATable(NodeType *root, int **followPos)
{

    State *nullState = (State *)malloc(sizeof(State));
    nullState->isMarked = 1;
    nullState->symbolPos = calloc(1, sizeof(int));

    State *initState = (State *)malloc(sizeof(State));
    initState->stateID = 1;
    initState->symbolPos = root->firstPos;
    initState->isMarked = 0;

    // Create symbol array of the pattern.
    // Eg. For ab#, Symbol array will be {49,50}
    int *symbolArray = calloc(ARRAYLENGTH, sizeof(int));
    int *mapSym2Pos = calloc(ARRAYLENGTH, sizeof(int)); // For mapping symbol array and positions.
    int *symbolArraySize = (int *)malloc(sizeof(int));
    *symbolArraySize = -1;
    generateSymbolArray(root, symbolArray, mapSym2Pos, symbolArraySize);

    // Create unique symbol array of the patter
    // Eg. For aab#, UNQSYM will be {49,50}
    uniqueArray = calloc(ARRAYLENGTH, sizeof(int));
    uniqueSize = (int *)malloc(sizeof(int));
    *uniqueSize = 0; // Will increment as suffix
    generateUnqSymArray(symbolArray, *symbolArraySize, uniqueArray, uniqueSize);

    // Allocate State Array
    State *stateArray[DSTATEARRAYSIZE];
    int *stateArraySize = malloc(sizeof(int));
    *stateArraySize = 0;
    for (int i = 0; i < DSTATEARRAYSIZE; ++i)
        stateArray[i] = (State *)malloc(sizeof(State));
    // Done Allocation

    // Allocate DFA Table
    //int **dfaTable; [20x76]
    dfaTable = calloc(TRANSITIONTABLESIZE,sizeof(int *));
    for(int i=0; i<TRANSITIONTABLESIZE; ++i)
        dfaTable[i] = calloc(CHARACTERSUPPORT, sizeof(int));
    // Done Allocation



    // Add the NULL and first pos of Root as first and second State.
    stateArray[(*stateArraySize)++] = nullState;
    stateArray[(*stateArraySize)++] = initState;

    // Exits when there is not unmarked state.
    while (1)
    {
        State *currentState = NULL;

        for (int i = 0; i < *stateArraySize; ++i)
        {
            if (!stateArray[i]->isMarked)
            {
                currentState = stateArray[i];
                currentState->isMarked = 1;
                break;
            }
        }

        if (currentState == NULL)
            break; // Break from While Loop

        // For each symbol
        for (int i = 0; i < *uniqueSize; ++i)
        {

            // Merge the followPos of all positions of in
            // UniqueArray[i] in the  currentState
            // and check whether it is present in the current set of
            // States in stateArray.
            int *u = calloc(ARRAYLENGTH, sizeof(int));
            int *uSize = calloc(1, sizeof(int));

            makeUnion(u, uSize, currentState->symbolPos, symbolArray, *symbolArraySize, mapSym2Pos, symbolArray[i], followPos);
            int found = checkUandStateArrays(u, *uSize, stateArray, *stateArraySize);

            if (!found)
            {
               // It is a new state. Make it a state and add it to the StateArray.
               State *newState = (State *)malloc(sizeof(State));
               newState->stateID = *stateArraySize;
               newState->symbolPos = u;
               newState->isMarked = 0;
               stateArray[(*stateArraySize)++] = newState;
               dfaTable[currentState->stateID][uniqueArray[i]] = newState->stateID;
            }
            else if(found == NULLSTATE)
               dfaTable[0][uniqueArray[i]] = 0;
            else
               dfaTable[currentState->stateID][uniqueArray[i]] = found;

        }
    }
    
    // Make Final States Array
    finalArray = calloc(ARRAYLENGTH, sizeof(int));
    finalArraySize = calloc(1,sizeof(int));
    *finalArraySize = finalState(root->concatNode.right->position,stateArray,*stateArraySize,finalArray);
    

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

void generateSymbolArray(NodeType *root, int *symbolArray, int *mapSym2Pos, int *index)
{
    if (!root)
        return;

    switch (root->type)
    {
    case TYPECHAR:
        symbolArray[++*index] = root->charNode.value - '.';
        mapSym2Pos[root->position] = *index;
        return;

    case TYPEWILD:
        symbolArray[++*index] = '.' - '.';
        mapSym2Pos[root->position] = *index;
        return;

    case TYPECONCAT:
        generateSymbolArray(root->concatNode.left, symbolArray, mapSym2Pos, index);
        generateSymbolArray(root->concatNode.right, symbolArray, mapSym2Pos, index);
        return;

    case TYPESTAR:
        generateSymbolArray(root->starNode.prevNode, symbolArray, mapSym2Pos, index);
        return;

    case TYPEOR:
        generateSymbolArray(root->orNode.left, symbolArray, mapSym2Pos, index);
        generateSymbolArray(root->orNode.right, symbolArray, mapSym2Pos, index);
        return;

    default:
        break;
    }

    return;
}

void generateUnqSymArray(int *symbolArray, int symbolArraySize, int *uniqueArray, int *uniqueArraySize)
{
    int found = 0;
    for (int i = 0; i < symbolArraySize; ++i)
    {
        found = 0;
        for (int j = 0; j < *uniqueArraySize; ++j)
        {
            if (symbolArray[i] == uniqueArray[j])
            {
                found = 1;
                break;
            }
        }

        if (!found)
            uniqueArray[(*uniqueArraySize)++] = symbolArray[i];
    }
}

void makeUnion(int *u, int *uSize, int *currentStatePos, int *symbolArray, int symbolArraySize, int *map2SymPos, int symbol, int **followPos)
{
    for (int i = 0; currentStatePos[i] != 0; ++i)
    {
        if (symbol == symbolArray[map2SymPos[currentStatePos[i]]])
        {
            for (int j = 0; followPos[currentStatePos[i]][j] != 0; ++j)
            {
                int found = 0;
                for (int k = 0; u[k] != 0; ++k)
                {
                    if (u[k] == followPos[currentStatePos[i]][j])
                    {
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    u[(*uSize)++] = followPos[currentStatePos[i]][j];
            }
        }
    }
}

int checkUandStateArrays(int *u, int uSize, State **stateArray, int stateArraySize)
{
    int found = 0, cmpRes=0;


    for (int i = 0; i < stateArraySize; ++i)
    {

        int *checkArray, checkArraySize = 0;
        checkArray = stateArray[i]->symbolPos;

        for (checkArraySize; checkArray[checkArraySize] != 0; ++checkArraySize)
            ;

        if (uSize != checkArraySize)
            continue;

        qsort(u, uSize, sizeof(int), compare);
        qsort(checkArray, uSize, sizeof(int), compare);

        cmpRes = compareArray(u,checkArray,uSize);

        if(cmpRes == NULLSTATE)
        {
            found = NULLSTATE;
            break;
        }

        else if(cmpRes)
        {
            found = i;
            break;
        }
        
    }

    return found;
}

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int compareArray(int *a, int *b, int size)
{   
    if(!size)
        return NULLSTATE;

    for (int i = 0; i < size; ++i)
    {
        if (a[i] != b[i])
            return 0; // Arrays are not equal.
    }
    return 1; // Arrays are equal.
}

int finalState(int finalElm, State **stateArray, int stateArraySize, int *finalArray)
{
    int finalStateSize = 0;
    for(int i=0; i<stateArraySize; ++i)
    {
        for(int j=0; stateArray[i]->symbolPos[j]!=0; ++j)
        {
            if(finalElm == stateArray[i]->symbolPos[j])
            {
                finalArray[finalStateSize++] = stateArray[i]->stateID;
                break;
            }
        }
    }

    return finalStateSize;
}