#ifndef TREE_H
#define TREE_H

#include "../lexer.h"

typedef struct _BinaryNode BinaryNode;
struct _BinaryNode {
	char * nodeType;
	Token token;

	BinaryNode * left;
	BinaryNode * right;
};


#endif