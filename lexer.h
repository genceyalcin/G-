#ifndef LEXER_H
#define LEXER_H

#include <inttypes.h>

#define ADD '+'
#define SUB '-'
#define MUL '*'
#define DIV '/'
#define L_PAR '('
#define R_PAR ')'
#define L_CUR '{'
#define R_CUR '}'
#define TERM ';'
#define EQUALS '='

#define ADD_T "ADD"
#define SUB_T "SUB"
#define MUL_T "MUL"
#define DIV_T "DIV"
#define L_PAR_T "L_PAR"
#define R_PAR_T "R_PAR"
#define L_CUR_T "L_CUR"
#define R_CUR_T "R_CUR"
#define TERM_T "TERM"

#define INVALID "INVALID"

#define KEYWORD_T "KEYWORD"
#define IDENTIFIER_T "IDENTIFIER"
#define EQUALS_T "EQUALS"

#define GINT "gint"
#define GLOAT "gloat"
#define GATIONAL "gational"

#define GINT_T "GINT"
#define GLOAT_T "GLOAT"
#define GATIONAL_T "GATIONAL"

#define LINE_MAX 500

struct _Token {
	char * type;
	char * value;

	uint32_t length;
	
	uint32_t line;
	uint32_t charPos;
};
typedef struct _Token Token;

struct _Line {
	Token * tokensArray;
	uint32_t noTokens;
};
typedef struct _Line Line;

Line Lexer_TokenizeLine(const char * const line);

Token Lexer_MakeNumberToken(const char * const start);

Token Lexer_MakeWordToken(const char * const start);

#endif