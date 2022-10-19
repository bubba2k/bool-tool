#ifndef LP_TOKENLOOKUP_H_INCLUDED
#define LP_TOKENLOOKUP_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "dynamic_array_gen.h"

#define LP_TOKEN_MAX_SIZE 63

typedef enum {	LP_TOK_UNKNOWN		  	= 0,
			  	LP_TOK_VARIABLE			= 1, 
			  	LP_TOK_PAR_OPEN			= 2, 
		      	LP_TOK_PAR_CLOSE		= 3, 
			  	LP_TOK_TRUE				= 4, 
			  	LP_TOK_FALSE			= 5,
				LP_TOK_AND				= 6,
				LP_TOK_OR				= 7,
				LP_TOK_IMPLIC			= 8,
				LP_TOK_NOT				= 9,
				LP_TOK_EQU				= 10
} TokenType;

typedef struct Token
{
	TokenType 	type;
	char		name[LP_TOKEN_MAX_SIZE + 1];
} Token;


// dynamic array of tokens
DYNAMIC_ARRAY_GEN_DECL(Token, tokens)

const char *tokentype_get_str(TokenType token);
void token_print(Token *token);

void DA_tokens_print(DA_tokens *arr);

void token_copy(Token *dest, const Token *src);

#define TOKEN_TABLE_SIZE 9
extern const Token token_table[TOKEN_TABLE_SIZE];

#endif
