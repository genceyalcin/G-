#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#include "variables.h"

#include "../data_structures/ArrayList.h"
#include "../data_structures/StringHashTable.h"
#include "../interpreter.h"

static ArrayList * variablesArray;
static StringHashTable * variablesTable;

void Variables_CreateVariableTable() {
	size_t size = sizeof(Variable *);
	
	variablesArray = ArrayList_CreateEmpty(size);
	variablesTable = StringHashTable_create(500, elfhash);
}

void Variables_AddVariable(Variable * var) {
	
	uint32_t * locationsArray = StringHashTable_getLocationsOf(variablesTable, var->identifier);
	
	if (locationsArray == NULL) {
		uint32_t index = ArrayList_Add(variablesArray, var);
		StringHashTable_addEntry(variablesTable, var->identifier, index);
	} else {
		//ERRORS
		exit(1);
	}
}

void Variables_RemoveVariable(Variable * var);

void Variables_ModifyVariable(Variable * var) {
	
	uint32_t * locationsArray = StringHashTable_getLocationsOf(variablesTable, var->identifier);
	variablesArray->array[locationsArray[0]] = var;
		
}

Variable * Variables_GetVariable(char * identifier) {
	
	uint32_t * locationsArray = StringHashTable_getLocationsOf(variablesTable, identifier);
	if (locationsArray != NULL) {
		Variable * var = ArrayList_GetEntryAtIndex(variablesArray, locationsArray[0]);
		
		return var;
	}
	
	return NULL;
	
}

void Variables_printArray() {
	printf("Index: %d, Array: ( ", variablesArray->index);
	for (uint32_t i = 0; i < variablesArray->index; i++) {
		printf("%s ", ((Variable *)ArrayList_GetEntryAtIndex(variablesArray, i))->identifier);
	}
	printf(")\n");
}
