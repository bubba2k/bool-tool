#ifndef LP_TOKENLOOKUP_H_INCLUDED
#define LP_TOKENLOOKUP_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "dynamic_array_gen.h"

typedef enum {	LP_TOK_UNKNOWN		  	= 0,
			 	LP_TOK_BINARY_OP 		= 1, 
			  	LP_TOK_UNARY_OP  		= 2,
			  	LP_TOK_VARIABLE			= 3, 
			  	LP_TOK_BRACKET_OPEN		= 4, 
		      	LP_TOK_BRACKET_CLOSE	= 5, 
			  	LP_TOK_TRUE				= 6, 
			  	LP_TOK_FALSE			= 7	 } Tokentype;

typedef enum {	LP_OP_AND		=  101,
				LP_OP_OR		=  102,
				LP_OP_IMPLIC	=  103,

				LP_OP_NOT		=  104
} Operator;

typedef struct Token
{
	Tokentype 	ttype;
	int 		tdetail;
	char		name[64];
} Token;



// dynamic array of tokens
DYNAMIC_ARRAY_GEN_DECL(Token, tokens)

const char *token_get_str(Token *token);
void token_print(Token *token);

void DA_tokens_print(DA_tokens *arr);

void token_copy(Token *dest, const Token *src);

typedef struct TokenLookupEntry
{
	Token token;
	const char *identifier;
} TokenLookupEntry;
#define TOKEN_TABLE_SIZE 8
extern const TokenLookupEntry token_table[TOKEN_TABLE_SIZE];

#endif
