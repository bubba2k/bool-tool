#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tokens.h"

DYNAMIC_ARRAY_GEN_IMPL(Token, tokens)

const char *tokentype_get_str(TokenType tokentype)
{
	switch(tokentype)
	{
		default:
			return "UNKNOWN";
			break;
		case LP_TOK_VARIABLE:
			return "VARIABLE";
			break;
		case LP_TOK_PAR_OPEN:
			return "PAR_OPEN";
			break;
		case LP_TOK_PAR_CLOSE:
			return "PAR_CLOSE";
			break;
		case LP_TOK_TRUE:
			return "TRUE";
			break;
		case LP_TOK_FALSE:
			return "FALSE";
			break;
		case LP_TOK_AND:
			return "AND";
			break;
		case LP_TOK_OR:
			return "OR";
			break;
		case LP_TOK_IMPLIC:
			return "IMPLICATION";
			break;
		case LP_TOK_NOT:
			return "NOT";
			break;
	}
}	

void token_print(Token *token)
{
	printf("[%s, \"%s\"]\n", tokentype_get_str(token->type), token->name);
}

void DA_tokens_print(DA_tokens *arr)
{
	printf("Array of %lu tokens;\n", arr->size);
	for(unsigned int i = 0; i < arr->size; i++)
	{
		token_print(arr->data + i);
	}
}

void token_copy(Token *dest, const Token *src)
{
	dest->type = src->type;
	memcpy(dest->name, src->name, 64);
}

/* TOKEN LOOKUP TABLE */
const Token token_table[TOKEN_TABLE_SIZE] = 
{
	{ .type = LP_TOK_FALSE, 	.name = "0"  },
	{ .type = LP_TOK_TRUE,		.name = "1"  },
	{ .type = LP_TOK_PAR_OPEN,  .name = "("  },
	{ .type = LP_TOK_PAR_CLOSE, .name = ")"  },
	{ .type = LP_TOK_NOT, 		.name = "!"  },
	{ .type = LP_TOK_AND, 		.name = "&&" },
	{ .type = LP_TOK_OR, 		.name = "||" },
	{ .type = LP_TOK_IMPLIC, 	.name = "->" },
};

