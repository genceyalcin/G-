#ifndef PARSER_H
#define PARSER_H

#define OP "op"
#define NUM "num"
#define KEYWORD "keyword"
#define IDENTIFIER "identifier"
	
#define FACTOR "factor"
#define TER "term"
#define EXPR "expr"

#include "data_structures/tree.h"
#include "lexer.h"

struct _ParseTree {
	BinaryNode * root;
};
typedef struct _ParseTree ParseTree;

ParseTree Parser_CreateParseTree(Line line);

BinaryNode * Parser_Factor(Token * token, uint32_t noTokens, uint32_t index);

BinaryNode * Parser_Term(Token * token, uint32_t noTokens, uint32_t index);

BinaryNode * Parser_Expr(Token * token, uint32_t noTokens, uint32_t index);

BinaryNode * Parser_KeywordNode(Token token);

BinaryNode * Parser_IdentifierNode(Token token);

BinaryNode * Parser_BinaryOperatorNode(Token token);

BinaryNode * Parser_NumberNode(Token token);

BinaryNode * Parser_UnaryNode(Token token);

void Parser_PrintTree(BinaryNode * thisNode);

#endif