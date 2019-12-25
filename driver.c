/**
 *	Driver for G++
 * 	
 * 	Invocation:
 *	g++ <file_name_1> <argument_1> ... <argument_n>
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "version.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#include "trackers/variables.h"

/**
 *	Main function:
 */
int main(int argc, char ** argv) {
 
	FILE * gpp_file;		// Source file

	if (argc >= 2) {
		gpp_file = fopen(argv[1], "r");		// Look for the source file

		if (gpp_file == NULL) {
			printf("Couldn't find file %s\n", argv[1]);
			exit(0);
		}
	}
		
	Variables_CreateVariableTable();
	
	char line[LINE_MAX];

	while (argc >= 2 && fgets(line, LINE_MAX, gpp_file) != NULL) {

		Line lineTokens = Lexer_TokenizeLine(line);

		for (uint32_t i = 0; i < lineTokens.noTokens; i++) {
			printf("(%s, %s, %d, %d) ", (lineTokens.tokensArray + i)->type, (lineTokens.tokensArray + i)->value, (lineTokens.tokensArray + i)->line, (lineTokens.tokensArray + i)->charPos);
		}

		printf("\n");

		ParseTree parseTree = Parser_CreateParseTree(lineTokens);
		
		if (parseTree.root != NULL) {
			Parser_PrintTree(parseTree.root);
			printf("\n");
			Interpreter_Interpret(parseTree);
		}

		printf("\n");

		for (uint32_t i = 0; i < LINE_MAX; i++) {	// Clear fgets buffer
			line[i] = '\0';
		}

		free(lineTokens.tokensArray); // THIS HAS TO BE CHANGED LATER. SOME VALUES ARE CALLOC'D
	}

	if (argc == 1) {
		printf("Current Version : %s %s\n", VERSION_NUMBER, VERSION_STABLE);
	}

}
