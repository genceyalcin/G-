#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "interpreter.h"
#include "lexer.h"

void Interpreter_Interpret(ParseTree parseTree) {

	if (parseTree.root->nodeType == OP) {
		Number num = Interpreter_VisitOperation(parseTree.root);
		// printf("%d %f\n", num.intRep, num.floatRep);
	}

	else if (parseTree.root->nodeType == KEYWORD) {
		printf("KILL ME\n");
	}

}

/********************************************************
 *
 *					   NODE VISITS
 *
 ********************************************************/

Number Interpreter_VisitOperation(BinaryNode * node) {
	Number left;
	Number right;
	
	if (node->left != NULL && node->left->nodeType == OP) {
		left = Interpreter_VisitOperation(node->left);
	}
	if (node->right != NULL && node->right->nodeType == OP) {
		right = Interpreter_VisitOperation(node->right);
	}
	if (node->left != NULL && node->left->nodeType == NUM) {
		left = Interpreter_VisitNumber(node->left);
	}
	if (node->right != NULL && node->right->nodeType == NUM) {
		right = Interpreter_VisitNumber(node->right);
	}
	
	Number result;
	result.intRep = 0;

	if (node->token.type == ADD_T) {
		result = Interpreter_Add(left, right);
	} 
	else if (node->token.type == SUB_T) {
		result = Interpreter_Subtract(left, right);
	}
	else if (node->token.type == MUL_T) {
		result = Interpreter_Multiply(left, right);
	}
	else if (node->token.type == DIV_T) {
		result = Interpreter_Divide(left, right);
	}

	return result;

}

Number Interpreter_VisitNumber(BinaryNode * node) {
	Number num;

	num.type = node->token.type;

	if (num.type == GINT_T) {
		num.intRep = atoi(node->token.value);
		num.floatRep = 0;
	} else if (num.type == GLOAT_T) {
		num.floatRep = atof(node->token.value);
		num.intRep = 0;
	}

	return num;
}

/********************************************************
 *
 *					NUMBER OPERATIONS
 *
 ********************************************************/

Number Interpreter_Add(Number left, Number right) {
	Number num;

	num.type = left.type;
	num.intRep = left.intRep + right.intRep;
	num.floatRep = left.floatRep + right.floatRep;

	return num;
}

Number Interpreter_Subtract(Number left, Number right) {
	Number num;
	
	num.type = left.type;
	num.intRep = left.intRep - right.intRep;
	num.floatRep = left.floatRep - right.floatRep;

	return num;
}

Number Interpreter_Divide(Number left, Number right) {
	Number num;

	num.type = left.type;
	if (right.intRep != 0) {
		num.intRep = left.intRep / right.intRep;
	}
	if (right.floatRep != 0 && left.floatRep != 0) {
		num.floatRep = left.floatRep / right.floatRep;
	}
	if (right.floatRep != 0 && left.intRep != 0) {
		num.floatRep = (float)left.intRep / right.floatRep;
	}
	if (right.intRep != 0 && left.floatRep != 0) {
		num.floatRep = left.floatRep / right.intRep;
	}
	return num;
}

Number Interpreter_Multiply(Number left, Number right) {
	Number num;
	
	num.type = left.type;
	num.intRep = left.intRep * right.intRep;
	num.floatRep = left.floatRep * right.floatRep;

	return num;
}


