#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parser.h"
#include "interpreter.h"
#include "lexer.h"

#include "trackers/variables.h"

void Interpreter_Interpret(ParseTree parseTree) {
	
	if (parseTree.root->nodeType == OP) {
		Number num = Interpreter_VisitOperation(parseTree.root);
		printf("%d %f\n", num.intRep, num.floatRep);
	}

	else if (parseTree.root->nodeType == KEYWORD) {
		Interpreter_VisitKeyword(parseTree.root);
	}

}

/********************************************************
 *
 *					   NODE VISITS
 *
 ********************************************************/

void Interpreter_VisitKeyword(BinaryNode * node) {
	Interpreter_VisitIdentifier(node);
}

void * Interpreter_VisitIdentifier(BinaryNode * node) {

	char * keyType;

	if (node->nodeType == KEYWORD) {
		keyType = node->token.value;
		node = node->right;
	} else {
		keyType = NULL;
	}
	
	if (node->left != NULL && node->left->nodeType == EQ_NODE) {
		if (node->right->nodeType == OP) {
			Number num = Interpreter_VisitOperation(node->right);
			Number * pointer = calloc(1, sizeof(Number));
			*pointer = num;
			
			Variable * var = calloc(1, sizeof(Variable));
				
			var->identifier = node->token.value;
			var->var = pointer;
			
			if (keyType != NULL) {
				var->keyword = keyType;
				Variables_AddVariable(var);
			}
			else {
				Variable * varReplace = Variables_GetVariable(var->identifier);
				if (varReplace != NULL) {
					var->keyword = varReplace->keyword;
					Variables_ModifyVariable(var);
				}
			}
			
			Variables_printArray();
			
			printf("%s is equal to %d %f\n", node->token.value, num.intRep, num.floatRep);
		}
	}
	
	
	else  if (node->nodeType == IDENTIFIER) {
		Variable * var = Variables_GetVariable(node->token.value);
		return var->var;
	}
	
	return NULL;
	
}

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
	if (node->right != NULL && node->right->nodeType == IDENTIFIER) {
		right = *((Number *)Interpreter_VisitIdentifier(node->right));
	}
	if (node->left != NULL && node->left->nodeType == IDENTIFIER) {
		left = *((Number *)Interpreter_VisitIdentifier(node->left));
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
	
	if (right.intRep != 0  && left.intRep != 0) {
		num.type = right.type;
		num.intRep = left.intRep + right.intRep;
		num.floatRep = 0;
	}
	if (right.floatRep != 0 && left.floatRep != 0) {
		num.type = left.type;
		num.floatRep = left.floatRep + right.floatRep;
		num.intRep = 0;
	}
	if (right.floatRep != 0 && left.intRep != 0) {
		num.type = right.type;
		num.floatRep = (float)left.intRep + right.floatRep;
		num.intRep = 0;
	}
	if (right.intRep != 0 && left.floatRep != 0) {
		num.type = left.type;
		num.floatRep = left.floatRep + right.intRep;
		num.intRep = 0;
	}

	return num;
}

Number Interpreter_Subtract(Number left, Number right) {
	Number num;
	
	
	if (right.intRep != 0  && left.intRep != 0) {
		num.type = right.type;
		num.intRep = left.intRep - right.intRep;
		num.floatRep = 0;
	}
	if (right.floatRep != 0 && left.floatRep != 0) {
		num.type = left.type;
		num.floatRep = left.floatRep - right.floatRep;
		num.intRep = 0;
	}
	if (right.floatRep != 0 && left.intRep != 0) {
		num.type = right.type;
		num.floatRep = (float)left.intRep - right.floatRep;
		num.intRep = 0;
	}
	if (right.intRep != 0 && left.floatRep != 0) {
		num.type = left.type;
		num.floatRep = left.floatRep - right.intRep;
		num.intRep = 0;
	}
	return num;
}

Number Interpreter_Divide(Number left, Number right) {
	Number num;

	if (right.intRep != 0 && left.intRep != 0) {
		num.type = right.type;
		num.intRep = left.intRep / right.intRep;
		num.floatRep = 0;
	}
	if (right.floatRep != 0 && left.floatRep != 0) {
		num.floatRep = left.floatRep / right.floatRep;
		num.intRep = 0;
	}
	if (right.floatRep != 0 && left.intRep != 0) {
		num.floatRep = (float)left.intRep / right.floatRep;
		num.intRep = 0;
	}
	if (right.intRep != 0 && left.floatRep != 0) {
		num.floatRep = left.floatRep / right.intRep;
		num.intRep = 0;
	}
	return num;
}

Number Interpreter_Multiply(Number left, Number right) {
	Number num;
	
	if (right.intRep != 0 && left.intRep != 0) {
		num.type = right.type;
		num.intRep = left.intRep / right.intRep;
		num.floatRep = 0;
	}
	if (right.floatRep != 0 && left.floatRep != 0) {
		num.floatRep = left.floatRep / right.floatRep;
		num.intRep = 0;
	}
	if (right.floatRep != 0 && left.intRep != 0) {
		num.floatRep = (float)left.intRep / right.floatRep;
		num.intRep = 0;
	}
	if (right.intRep != 0 && left.floatRep != 0) {
		num.floatRep = left.floatRep / right.intRep;
		num.intRep = 0;
	}
	
	return num;
}

/** Hashes a C-string to an unsigned integer.
 *  Pre:
 *       str points to a zero-terminated char array
 */
uint32_t elfhash(const char* str) {

   assert(str != NULL );      // self-destuct if called with NULL
   
   uint32_t hashvalue = 0,    // value to be returned
            high;             // high nybble of current hashvalue
            
   while ( *str ) {           // continue until *str is '\0'
		
      hashvalue = (hashvalue << 4) + *str++;      // shift high nybble out,
                                                  //   and add in next char
      
      if ( (high = (hashvalue & 0xF0000000)) ) {  // if high nybble != 0000
         hashvalue = hashvalue ^ (high >> 24);    //   fold it back in
      }
      
      hashvalue = hashvalue & 0x7FFFFFFF;         // zero high nybble
   }
   
   return hashvalue;
}
