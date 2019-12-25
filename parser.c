#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "parser.h"
#include "lexer.h"

static Token * tokenPointer;

/**
 *
 *
 */
ParseTree Parser_CreateParseTree(Line line) {
	ParseTree parseTree;
	
	parseTree.root = NULL;

	if (line.tokensArray == NULL) {
		return parseTree;
	}

	tokenPointer = line.tokensArray;

	parseTree.root = Parser_Expr(tokenPointer, line.noTokens, 0);

	return parseTree;
}

BinaryNode * Parser_Factor(Token * token, uint32_t noTokens, uint32_t index) {

	if (tokenPointer->type == GINT_T || tokenPointer->type == GLOAT_T) {
		tokenPointer++;
		return Parser_NumberNode(*token);
	}
	
	if (tokenPointer->type == IDENTIFIER_T) {
		printf("%s\n", tokenPointer->value);
		tokenPointer++;
		return Parser_IdentifierNode(*token);
	}

	if (tokenPointer->type == SUB_T) {
		uint32_t count = 1;
		while (tokenPointer->type == SUB_T) {
			count++;
			tokenPointer++;
		}
		if (tokenPointer->type != GINT_T && tokenPointer->type != GLOAT_T) {
			return NULL;
		}
		if (count % 2 == 0) {
			char * unsignedValue = tokenPointer->value;
			char * signedValue = calloc(strlen(unsignedValue) + 2, sizeof(char));

			signedValue[0] = '-';
			memcpy(signedValue + 1, unsignedValue, strlen(unsignedValue) * sizeof(char));

			tokenPointer->value = signedValue;

			return Parser_NumberNode(*tokenPointer);

		} else {
			return Parser_NumberNode(*tokenPointer);
		}
	}

	if (tokenPointer->type == L_PAR_T) {

	}

	return NULL;
}

BinaryNode * Parser_Term(Token * token, uint32_t noTokens, uint32_t index) {

	BinaryNode * factorLeft = Parser_Factor(tokenPointer, noTokens, index); 
	BinaryNode * newNode;

	while (tokenPointer->type == MUL_T || tokenPointer->type == DIV_T) {
		Token * operatorToken = tokenPointer;
		tokenPointer++;
		BinaryNode * factorRight = Parser_Factor(tokenPointer, noTokens, index);
		newNode = Parser_BinaryOperatorNode(*operatorToken);
		newNode->left = factorLeft;
		newNode->right = factorRight;
		factorLeft = newNode;
	}

	return factorLeft;
}

BinaryNode * Parser_Expr(Token * token, uint32_t noTokens, uint32_t index) {

	Token * keyword = NULL;
	if (tokenPointer->type == KEYWORD_T) {
		keyword = tokenPointer;
		tokenPointer++;
	}
	
	if (tokenPointer->type == IDENTIFIER_T) {
		Token * identifier = tokenPointer;
		tokenPointer++;
		if (tokenPointer->type == EQUALS_T) {
			Token * equals = tokenPointer;
			tokenPointer++;
			if (tokenPointer->type == GINT_T || tokenPointer->type == GLOAT_T || tokenPointer->type == IDENTIFIER_T) {
				BinaryNode * exprResult = Parser_Expr(tokenPointer, noTokens, index);
				BinaryNode * identifierNode = Parser_IdentifierNode(*identifier);
				BinaryNode * eqNode = Parser_EqualsNode(*equals);

				if (keyword != NULL) {
					BinaryNode * keywordNode = Parser_KeywordNode(*keyword);
					identifierNode->right = exprResult;
					identifierNode->left = eqNode;
					keywordNode->right = identifierNode; 
					return keywordNode;
				}
				else {
					identifierNode->right = exprResult;
					identifierNode->left = eqNode;
					return identifierNode;
				}
			}
		} else if (tokenPointer->type == ADD_T || tokenPointer->type == SUB_T) {
			tokenPointer--;
		} else {
			return NULL;
		}
	}

	BinaryNode * factorLeft = Parser_Term(tokenPointer, noTokens, index); 
	BinaryNode * newNode;

	while (tokenPointer->type == ADD_T || tokenPointer->type == SUB_T) {
		Token * operatorToken = tokenPointer;
		tokenPointer++;
		BinaryNode * factorRight = Parser_Term(tokenPointer, noTokens, index);
		newNode = Parser_BinaryOperatorNode(*operatorToken);
		newNode->left = factorLeft;
		newNode->right = factorRight;
		factorLeft = newNode;
	}

	return factorLeft;
}

BinaryNode * Parser_KeywordNode(Token token) {

	BinaryNode * node = malloc(sizeof(BinaryNode));

	node->nodeType = KEYWORD;
	node->token = token;
	node->left = NULL;
	node->right = NULL;

	return node;
}

BinaryNode * Parser_IdentifierNode(Token token) {

	BinaryNode * node = malloc(sizeof(BinaryNode));

	node->nodeType = IDENTIFIER;
	node->token = token;
	node->left = NULL;
	node->right = NULL;

	return node;
}

BinaryNode * Parser_EqualsNode(Token token) {

	BinaryNode * node = malloc(sizeof(BinaryNode));

	node->nodeType = EQ_NODE;
	node->token = token;
	node->left = NULL;
	node->right = NULL;

	return node;
}

BinaryNode * Parser_BinaryOperatorNode(Token token) {

	BinaryNode * node = malloc(sizeof(BinaryNode));

	node->nodeType = OP;
	node->token = token;
	node->left = NULL;
	node->right = NULL;

	return node;

}	

BinaryNode * Parser_NumberNode(Token token) {

	BinaryNode * node = malloc(sizeof(BinaryNode));

	node->nodeType = NUM;
	node->token = token;
	node->left = NULL;
	node->right = NULL;

	return node;

}

void Parser_PrintTree(BinaryNode * thisNode) {

	if (thisNode->left == NULL && thisNode->right == NULL) {
		printf("%s", thisNode->token.value);
		return;
	} else  {
		printf("(");
		printf("%s, (", thisNode->token.value);
		if (thisNode->left != NULL) {
			Parser_PrintTree(thisNode->left);
		} else {
			printf("empty");
		}

		printf(", ");
		if (thisNode->right != NULL) {
			Parser_PrintTree(thisNode->right);
		} else {
			printf("empty");
		}

		printf(")");
		printf(")");
	}
}
