#ifndef LP_PARSE_H_INCLUDED
#define LP_PARSE_H_INCLUDED

#include "tokens.h"

typedef struct TreeNode TreeNode;
struct TreeNode 
{
	TokenType		type;
	char 			name[64];
	
	TreeNode *left, *right;
};

/* Build a syntax tree from the list of tokens.
 * Make sure to check the list for syntactical correctness with
 * `is_syntactically_correct()` before, as this function does
 * not perform any checks. 										*/
TreeNode *lp_tree_create(DA_tokens *tokens);

/* Recursively destroys and frees a syntax tree. */
void lp_tree_destroy(TreeNode *node);

/* Recursively traverses the syntax tree and builds a string representing it. */
void lp_tree_print(TreeNode *node);

void lp_tree_print_raw(TreeNode *node);

#endif
