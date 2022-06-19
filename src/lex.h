#ifndef LP_LEX_H_INCLUDED
#define LP_LEX_H_INCLUDED

#include "tokens.h"


/* Perform lexical analysis of the given string. Found tokens are appended to 
   the given dynamic array of tokens (which must be initialized beforehand)
   1 is returned. 
   On failure, returns 0 and copies an error message to err_msg_buf of size 
   err_msg_buf_size, unless err_msg_buf == NULL. */
int lex_expression(const char *expr, DA_tokens *tokens, char *err_msg_buf, size_t err_msg_buf_size);

/* Check whether the list of tokens represents a grammatically correct expression.
   Copies an error message of maximum length err_msg_size to err_msg, if err_msg
   is not NULL. 																   */
int syntactically_correct(DA_tokens *tokens, char *err_msg, size_t err_msg_size);

#endif
