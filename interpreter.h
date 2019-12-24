#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

struct _Number{
	char * type;
	int32_t intRep;
	float floatRep;
};
typedef struct _Number Number;

Number Interpreter_Add(Number left, Number right);
Number Interpreter_Subtract(Number left, Number right);
Number Interpreter_Divide(Number left, Number right);
Number Interpreter_Multiply(Number left, Number right);

void Interpreter_Interpret(ParseTree parseTree);

Number Interpreter_VisitOperation(BinaryNode * node);

Number Interpreter_VisitNumber(BinaryNode * node);

#endif