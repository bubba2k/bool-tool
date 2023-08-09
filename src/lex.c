#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "tokens.h"
#include "lex.h"

const char *blank_space = " \t";

/* Check if char is a member of the given string. */
int char_is_one_of(char c, const char *chars)
{
	unsigned len = strlen(chars);
	for(unsigned int i = 0; i < len; i++)
		if( c == chars[i] ) return 1;

	return 0;
}

/* Returns 1 if a token matches the given string at the current position and copies the 
   token and length of the matched string to *token and *len respectively.
   Else returns 0 and leaves all pointers untouched. */
int token_lookup(const char *pos, Token *token, int *len)
{
	for(unsigned int i = 0; i < TOKEN_TABLE_SIZE; i++)
	{
		if(strstr(pos, token_table[i].name) == pos)
		{
			token_copy(token, &token_table[i]);
			*len   = strlen(token_table[i].name);
			return 1;
		}
	}

	return 0;
}

/* Attempt to parse a variable. Reads a variable into var_str_ptr.
 * The length of the variable name is put into len.
 * ALWAYS free var_str after use of this function,
   even if it returns 0 */
int parse_variable(const char *pos, char **var_str_ptr, int *len)
{
	*var_str_ptr = (char *) calloc(64, 1); // Max var length is 64
	unsigned int i = 0;

  // Variable name must start with an alphabetic character
  if(isalpha(*pos))
    (*var_str_ptr)[i++] = *pos++;
  else 
    return 0;
    	
	while((isalnum(*pos) || *pos == '_' ) && i < 64)
	{
		(*var_str_ptr)[i++] = *pos++;
	}

	if(i == 0 || i == 64) // Failure!
		return 0;
	else // Success!
	{
		*len = i;
		return 1;
	}
}

/* Perform lexical analysis of the given string. Found tokens are appended to 
   the given dynamic array of tokens (which must be initialized beforehand)
   1 is returned. 
   On failure, returns 0 and copies an error message to err_msg_buf of size 
   err_msg_buf_size, unless err_msg_buf == NULL. */
int lex_expression(const char *expr, DA_tokens *tokens, char *err_msg_buf, size_t err_msg_buf_size)
{
	/* Note: This rather simplistic approach to lexing only works as long
			 as none of the token words are infixes of other token words.
			 An exception are "T" or "F", which could possibly be infix
			 of a variable name. That might need fixing later.		      */

	const char *pos = expr;
	while(*pos != '\0')
	{
		// Skip all blanks
		if(char_is_one_of(*pos, blank_space)) { pos++; continue; };

		Token token;
		int step;
		char *variable_str;

		// Check for our built-in tokens
		if(token_lookup(pos, &token, &step))
		{
			// If successful, append the parsed token to the list and step forward
			DA_tokens_push_back(tokens, &token);
			pos += step;
		}
		else if(parse_variable(pos, &variable_str, &step)) // Else, the token must either be a variable or invalid
		{
			token.type   = LP_TOK_VARIABLE;
			snprintf(token.name, LP_TOKEN_MAX_SIZE, "%s", variable_str); // Copy the variables name into the token struct

			DA_tokens_push_back(tokens, &token);
			pos += step;

			free(variable_str);
		}
		else
		{
			free(variable_str); // variable_str must be freed even if parse_variable() fails

			/* Output our error message */
			if(err_msg_buf != NULL)
				snprintf(err_msg_buf, err_msg_buf_size, "Illegal symbol '%c' at index %ld", *pos, pos - expr + 1);

			return 0;
		}
	}

	return 1;
}


/* Syntactical analysis */
typedef enum { NONE = 0, UNARY_OP = 1, BINARY_OP = 2, PAR_OPEN = 3, PAR_CLOSE = 4, ATOM = 5 }
		AnalysisState;

const char *state_enum_get_str(AnalysisState state)
{
	switch(state)
	{
		case NONE:		return 	"None";
		case UNARY_OP:	return	"Unary Operator";
		case BINARY_OP:	return	"Binary Operator";
		case PAR_OPEN:	return	"'('";
		case PAR_CLOSE:	return	"')'";
		case ATOM:		return	"Atom";
		default:		return	"UNKNOWN";
	}
}

/* Table of what state transitions are legal in our grammar */
static const unsigned int state_trans_table[6][6] = {
/*					NONE	UNARY	BINARY	P_OPEN	P_CLOSE	ATOM	*/
/* NONE	 	 */	 {	1,		1,		0,		1,		0,		1	  },
/* UNARY 	 */  {	0,		1,		0,		1,		0,		1	  },
/* BINARY	 */  {	0,		1,		0,		1,		0,		1	  },
/* P_OPEN    */  {	0,		1,		0,		1,		0,		1	  },
/* P_CLOSE   */  {	1,		0,		1,		0,		1,		0	  },
/* ATOM      */  {	1,		0,		1,		0,		1,		0	  }};

AnalysisState resolve_state(Token *token)
{
	switch(token->type)
	{
		case LP_TOK_NOT:  			return UNARY_OP; 	break;
		case LP_TOK_AND: 			return BINARY_OP;	break;
		case LP_TOK_OR:				return BINARY_OP;	break;
		case LP_TOK_IMPLIC:			return BINARY_OP;	break;
		case LP_TOK_EQU:			return BINARY_OP;	break;
		case LP_TOK_VARIABLE:  		return ATOM;		break;
		case LP_TOK_TRUE:		   	return ATOM;		break;
		case LP_TOK_FALSE:		   	return ATOM;		break;
		case LP_TOK_PAR_OPEN:		return PAR_OPEN;	break;
		case LP_TOK_PAR_CLOSE:		return PAR_CLOSE;	break;
		default:			   		return NONE;		break;
	}
}

/* Check whether the list of tokens represents a grammatically correct expression.
   In case of error, copies an error message of maximum length err_msg_size to 
   err_msg, if err_msg is not NULL. 											 */
int syntactically_correct(DA_tokens *tokens, char *err_msg, size_t err_msg_size)
{
	AnalysisState old = NONE, new = NONE;
	int parenthesis_depth = 0;
	int error = 0;

	// Check all the transitions
	unsigned int i;
	for(i = 0; i <= (tokens->size); i++)
	{
		if(i == tokens->size)
			new = NONE;
		else
			new = resolve_state(tokens->data + i);

		if(new == PAR_OPEN) 
			++parenthesis_depth;
		else if(new == PAR_CLOSE)
		{
			--parenthesis_depth;
			if(parenthesis_depth < 0)
			{
				error = 1;
				break;
			}
		}

		if(!state_trans_table[old][new]) { error = 1; break; }
		old = new;
	}

	if(error || parenthesis_depth != 0)
	{
		if(parenthesis_depth > 0)
		{
			snprintf(err_msg, err_msg_size, "Unterminated parenthesis detected!");
			return 0;
		}
		else if(parenthesis_depth < 0)
		{
			snprintf(err_msg, err_msg_size, "Stray ')' detected!");
			return 0;
		}
		else	// Create a generic error message
		{
			char next[64], prev[64];
 			strcpy(next,  state_enum_get_str(new));
			strcpy(prev, state_enum_get_str(old));

			snprintf(err_msg, err_msg_size, "Illegal %s after %s.", next, prev);

			return 0;
		}
	}

	return 1;
}
