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

	BinaryNode * factorLeft = Parser_Factor(token, noTokens, index); 
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

	if (tokenPointer->type == KEYWORD_T) {
		printf("Keyword found %s\n", tokenPointer->value);
		Token * keyword = tokenPointer;
		tokenPointer++;
		if (tokenPointer->type == IDENTIFIER_T) {
			printf("Identifer found %s\n", tokenPointer->value);
			Token * identifier = tokenPointer;
			tokenPointer++;
			if (tokenPointer->type == EQUALS_T) {
				printf("Equals found %s\n", tokenPointer->value);
				tokenPointer++;
				if (tokenPointer->type == GINT_T || tokenPointer->type == GLOAT_T) {
					printf("Keyword found %s\n", tokenPointer->value);
					BinaryNode * exprResult = Parser_Expr(tokenPointer, noTokens, index);
					BinaryNode * keywordNode = Parser_KeywordNode(*keyword);
					BinaryNode * identifierNode = Parser_IdentifierNode(*identifier);

					identifierNode->right = exprResult;
					keywordNode->right = identifierNode; 
					return keywordNode;
				}
			} else {
				return NULL;
			}
		} else {
			return NULL;
		}
	}

	BinaryNode * factorLeft = Parser_Term(token, noTokens, index); 
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

// ParseTree Parser_CreateParseTree(Line line) {

// 	ParseTree parseTree;
// 	BinaryNode * root;

// 	for (uint32_t i = 0; i < line.noTokens; i++) {

// 		BinaryNode * newNode;

// 		if (line.tokensArray[i].type == GINT_T || line.tokensArray[i].type == GLOAT_T) {	// FACTOR
// 			newNode = Parser_NumberNode(line.tokensArray[i]);
// 			root = newNode;
// 		}

// 		else if (line.tokensArray[i].type == MUL_T || line.tokensArray[i].type == DIV_T) {		// 
// 			while (line.tokensArray[i].type == MUL_T || line.tokensArray[i].type == DIV_T) {
// 				newNode = Parser_BinaryOperatorNode(line.tokensArray[i], root);
// 				i++;
// 				if (line.tokensArray[i].type == GINT_T || line.tokensArray[i].type == GLOAT_T) {
// 					newNode->right = Parser_NumberNode(line.tokensArray[i]);
// 					root = newNode;
// 					i++;
// 				}
// 				if (i < line.noTokens && line.tokensArray[i].type != MUL_T && line.tokensArray[i].type != DIV_T) {
// 					i--;
// 					break;
// 				}
// 			}
// 		}

// 		else if (line.tokensArray[i].type == ADD_T || line.tokensArray[i].type == SUB_T) {
// 			while (line.tokensArray[i].type == ADD_T || line.tokensArray[i].type == SUB_T) {
// 				newNode = Parser_BinaryOperatorNode(line.tokensArray[i], root);
// 				i++;
// 				if (line.tokensArray[i].type == GINT_T || line.tokensArray[i].type == GLOAT_T) {
// 					newNode->right = Parser_NumberNode(line.tokensArray[i]);
// 					root = newNode;
// 					i++;
// 				}
// 				if (i < line.noTokens && line.tokensArray[i].type != ADD_T && line.tokensArray[i].type != SUB_T) {
// 					i--;
// 					break;
// 				}
// 			}
// 		}
// 	}

// 	parseTree.root = root;

// 	return parseTree;

// }