#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "lexer.h"

static uint32_t lineCount = 1;

/**
 *
 */
Line Lexer_TokenizeLine(const char * const line) {

	Line lineStruct;

	Token * tokensArray = (Token *)calloc(1, sizeof(Token));	// Create a Token array
	uint32_t tokenCount = 0;	// Count how many tokens have been added to the array
	
	for (uint32_t i = 0; i < LINE_MAX; i++) {

		Token thisToken;	// Token to be added to the array

		if (line[i] == TERM) {
			lineStruct.tokensArray = tokensArray;
			lineStruct.noTokens = tokenCount;
			return lineStruct;
		}

		if (line[i] == EQUALS) {									// EQUALS SIGN

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = EQUALS_T;			// Create Token
			thisToken.value = "=";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array


			tokenCount++;		// Increment the count 

		}

		else if (line[i] == ADD) {								// ADD

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = ADD_T;			// Create Token
			thisToken.value = "+";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array


			tokenCount++;		// Increment the count 

		}

		else if (line[i] == SUB) {								// SUB

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = SUB_T;			// Create Token
			thisToken.value = "-";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array


			tokenCount++;		// Increment the count 

		}

		else if (line[i] == MUL) {								// MUL

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = MUL_T;			// Create Token
			thisToken.value = "*";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array


			tokenCount++;		// Increment the count 

		}

		else if (line[i] == DIV) {								// DIV

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = DIV_T;			// Create Token
			thisToken.value = "/";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array

			tokenCount++;		// Increment the count 

		}

		else if (line[i] >= 48 && line[i] <= 57) {				// DIGITS

			// We have to take a different approach to this as 
			// numbers can have periods and variable lengths
			
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken = Lexer_MakeNumberToken(line + i);	// Get the number token using helper function

			if (thisToken.type == INVALID) {
				printf("Syntax error or some shit"); 			// Error handling for a later time
				free(tokensArray);
				exit(1);
			}

			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array

			tokenCount++;		// Increment the count

			i += thisToken.length - 1;
		}

		else if (line[i] == L_PAR) {			// L PAREN

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = L_PAR_T;			// Create Token
			thisToken.value = "(";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array

			tokenCount++;		// Increment the count 

		}

		else if (line[i] == R_PAR) {			// L PAREN

			// Readjust array size
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken.type = R_PAR_T;			// Create Token
			thisToken.value = ")";
			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array

			tokenCount++;		// Increment the count 

		}

		else if ((line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122)) {
			tokensArray = (Token *)realloc(tokensArray, (1 + tokenCount) * sizeof(Token));

			thisToken = Lexer_MakeWordToken(line + i);	// Get the number token using helper function

			if (thisToken.type == INVALID) {
				printf("Syntax error or some shit"); 			// Error handling for a later time
				free(tokensArray);
				exit(1);
			}

			thisToken.line = lineCount;
			thisToken.charPos = i;

			*(tokensArray + tokenCount) = thisToken;		// Add Token to array

			tokenCount++;		// Increment the count

			i += thisToken.length - 1;
		}
	}

	lineCount++;

	lineStruct.tokensArray = tokensArray;
	lineStruct.noTokens = tokenCount;

	return lineStruct;

}


/**
 *
 */
Token Lexer_MakeNumberToken(const char * const start) {

	Token number;						// Token to add number to

	const char * pointer = start;		// Pointer that will keep going until a non-number char is found
	uint32_t dotCount = 0;

	while ((*pointer >= 48 && *pointer <= 57) || *pointer == '.') {
		
		if (*pointer == '.') {
			dotCount++;
		}

		pointer++;
	}

	if (dotCount > 1) {
		number.type = INVALID;
		return number;
	}


	char * numStr = calloc((pointer - start) + 1, sizeof(char)); 
	memcpy(numStr, start, pointer - start);

	number.value = numStr;

	if (dotCount == 1) {
		number.type = GLOAT_T;

	} else {

		number.type = GINT_T;
	}

	number.length = pointer - start;

	return number;
}

/**
 *
 */
Token Lexer_MakeWordToken(const char * const start) {

	Token word;						// Token to add number to

	const char * pointer = start;		// Pointer that will keep going until a non-number char is found

	while ((*pointer >= 65 && *pointer <= 90) || (*pointer >= 97 && *pointer <= 122)) {
		pointer++;
	}

	char * str = calloc((pointer - start) + 1, sizeof(char)); 
	memcpy(str, start, pointer - start);

	word.value = str;

	if (strcmp(str, GINT) == 0 || strcmp(str,GLOAT) == 0) {
		word.type = KEYWORD_T;
	} else {
		word.type = IDENTIFIER_T;
	}

	word.length = pointer - start;

	return word;
}