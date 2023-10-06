/*

Operations contains all four function computed from the syntax tree.

Nullable: Is true for a syntax-tree node n if and only if the 
suexpression can be "made null" or the empty string, even 
though there may be other strings it can represent as well. 

FirstPos: Set of positions in the subtree rooted at n that 
correspond to the first symbol of at least one string in the 
language of the subexpression rooted at n.

LastPos: Set of positions in the subtree rooted at n that 
correspond to the last symbol of at least one string in the 
language of the subexpression rootes at n.

FollowPos: For a position p, is the set of position q in the 
entire syntax tree such that there is some string 
x = an in L((r)#) such that for some i, there is a way to explain
the membership of x in L((r)#) by matching ai to positon p of the
syntax tree and a(i+1) to position q. 

Definitions and algorithm taken from ALSU E2
*/

#include "main.h"

#ifndef OPERATIONS_HEADER
#define OPERATIONS_HEADER

void makeNull(Node *);

#endif