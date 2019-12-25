#ifndef VARIABLES_H
#define VARIABLES_H

struct _Variable {
	char * identifier;
	char * keyword;
	void * var;
};
typedef struct _Variable Variable;

void Variables_CreateVariableTable();

void Variables_AddVariable(Variable * var);

void Variables_ModifyVariable(Variable * var);

void Variables_RemoveVariable(Variable * var);

Variable * Variables_GetVariable(char * identifier);

void Variables_printArray();

#endif
