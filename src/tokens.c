#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokens.h"

DYNAMIC_ARRAY_GEN_IMPL(Token, tokens)

const char *token_get_str(Token *token)
{
	if(token->ttype == LP_TOK_UNARY_OP)
	{
		switch(token->tdetail)
		{
			case LP_OP_NOT:
				return "UnOp NOT";		break;

			default:
				return "UnOp UNKNOWN"; 	break;
		}
	}
	else if(token->ttype == LP_TOK_BINARY_OP)
	{
		switch(token->tdetail)
		{
			case LP_OP_AND:
				return "BiOp AND";			break;
			case LP_OP_OR:
				return "BiOp OR";			break;
			case LP_OP_IMPLIC:
				return "BiOp IMPLICATION";	break;

			default:
				return "UnOp UNKNOWN";		break;
		}
	}
	else if(token->ttype == LP_TOK_BRACKET_OPEN ) 
		return "(";
	else if(token->ttype == LP_TOK_BRACKET_CLOSE)
		return ")";
	else if(token->ttype == LP_TOK_TRUE )
		return "TRUE";
	else if(token->ttype == LP_TOK_FALSE)
		return "FALSE";
	else if(token->ttype == LP_TOK_VARIABLE)
		return "VARIABLE";

	return "UNKNOWN";
}	

void token_print(Token *token)
{
	if(token->ttype == LP_TOK_VARIABLE)
		printf("Token{%s, \"%s\", ID: %d}", token_get_str(token), token->name, token->tdetail);
	else
		printf("Token{%s}", token_get_str(token));
}

void DA_tokens_print(DA_tokens *arr)
{
	printf("Array of %lu tokens;\n", arr->size);
	for(unsigned int i = 0; i < arr->size; i++)
	{
		token_print(arr->data + i);
		printf("\n");
	}
}

void token_copy(Token *dest, const Token *src)
{
	dest->ttype 	= src->ttype;
	dest->tdetail	= src->tdetail;
	memcpy(dest->name, src->name, 64);
}

/* TOKEN LOOKUP TABLE */
const TokenLookupEntry token_table[TOKEN_TABLE_SIZE] = 
{
	{
		{ .ttype = LP_TOK_FALSE, .tdetail = -1, .name = "0" },
		"0"
	},
	{
		{ .ttype = LP_TOK_TRUE, .tdetail = -1, .name = "1" },
		"1"
	},
	{
		{ .ttype = LP_TOK_BRACKET_OPEN,  .tdetail = -1, .name = "(" },
		"("
	},
	{
		{ .ttype = LP_TOK_BRACKET_CLOSE, .tdetail = -1, .name = ")" },
		")"
	},
	{
		{ .ttype = LP_TOK_UNARY_OP, .tdetail = LP_OP_NOT, .name = "!" },
		"!"
	},
	{
		{ .ttype = LP_TOK_BINARY_OP, .tdetail = LP_OP_AND, .name = "&&" },
		"&&"
	},
	{
		{ .ttype = LP_TOK_BINARY_OP, .tdetail = LP_OP_OR, .name = "||"  },
		"||"
	},
	{
		{ .ttype = LP_TOK_BINARY_OP, .tdetail = LP_OP_IMPLIC, .name = "->" },
		"->"
	}
};

