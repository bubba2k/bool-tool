#include "lex.h"
#include "tokens.h"
#include "parse.h"

TreeNode *lp_treenode_create(int type, int var_hash, const char *name, TreeNode *_left, TreeNode *_right)
{
	TreeNode *node = malloc(sizeof(TreeNode));
	node->type 		= type;
	node->var_hash	= var_hash;
	snprintf(node->name, 63, "%s", name);

	node->left  = _left;
	node->right = _right;

	return node;
}

static const Token end_token = { .ttype = -1, .tdetail = -1 };
int token_index		= 	 0;
DA_tokens *tokens 	= NULL;
Token *next_token 	= NULL;

int scan_token() 
{
	if((size_t) token_index >= tokens->size)
	{
		next_token = (Token *) &end_token;
		return 0;
	}

	next_token = DA_tokens_get(tokens, token_index++);

	return 1;
}

TreeNode *parse_expr();
TreeNode *parse_disj();
TreeNode *parse_conj();
TreeNode *parse_nega();
TreeNode *parse_atom();

TreeNode *lp_tree_create(DA_tokens *_tokens)
{
	// Setup the environment
	tokens 		= _tokens;
	token_index = 0;
	next_token	= NULL;

	// Scan the first token and start building the tree
	scan_token();
	TreeNode *tree = parse_expr();

	return tree;
}

TreeNode *parse_expr()
{
	TreeNode *a = parse_disj();

	scan_token();
	if(	(next_token->ttype == LP_TOK_BINARY_OP)
		&& (next_token->tdetail == LP_OP_IMPLIC) )	
	{
		scan_token();
		return lp_treenode_create(LP_OP_IMPLIC, -1, "->", a, parse_expr());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_disj()
{
	TreeNode *a = parse_conj();
	
	scan_token();
	if( (next_token->ttype == LP_TOK_BINARY_OP)
		&& (next_token->tdetail == LP_OP_OR)		)
	{
		scan_token();
		return lp_treenode_create(LP_OP_OR, -1, "||", a, parse_disj());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_conj()
{
	TreeNode *a = parse_nega();
	scan_token();

	if( (next_token->ttype == LP_TOK_BINARY_OP)
		&& (next_token->tdetail == LP_OP_AND)	)
	{
		scan_token();
		return lp_treenode_create(LP_OP_AND, -1, "&&", a, parse_conj());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_nega()
{
	if( (next_token->ttype == LP_TOK_UNARY_OP)
		&& (next_token->tdetail == LP_OP_NOT)	)
	{
		scan_token();
		return lp_treenode_create(LP_OP_NOT, -1, "!", parse_atom(), NULL);
	}
	else
	{
		return parse_atom();
	}
}

TreeNode *parse_atom()
{
	if(next_token->ttype == LP_TOK_FALSE)
	{
		return lp_treenode_create(LP_TOK_FALSE, -1, "0", NULL, NULL);
	}
	else if(next_token->ttype == LP_TOK_TRUE)
	{
		return lp_treenode_create(LP_TOK_TRUE, -1, "1", NULL, NULL);
	}
	else if(next_token->ttype == LP_TOK_VARIABLE)
	{
		return lp_treenode_create(LP_TOK_VARIABLE, next_token->tdetail, next_token->name, NULL, NULL);
	}
	else if(next_token->ttype == LP_TOK_BRACKET_OPEN)
	{
		scan_token();

		TreeNode *a = parse_expr();

		scan_token();

		return a;
	}
	else return NULL; // This should never happen!
}

void lp_tree_print_raw_rec(TreeNode *node, int depth)
{
	for(int i = 0; i < depth; i++)
		printf("\t");

	printf("{%d, %d}\n", node->type, node->var_hash);

	if(node->left)
		lp_tree_print_raw_rec(node->left, depth + 1);
	if(node->right)
		lp_tree_print_raw_rec(node->right, depth + 1);
}

void lp_tree_print_raw(TreeNode *node)
{
	lp_tree_print_raw_rec(node, 0);
	printf("\n");
}


void lp_tree_print_rec(TreeNode *node)
{
	if( node->type == LP_OP_OR || node->type == LP_OP_AND
		|| node->type == LP_OP_IMPLIC )
	{
		printf("(");
		lp_tree_print_rec(node->left);
		printf(" %s ", node->name);
		lp_tree_print_rec(node->right);
		printf(")");
	}
	else if( node->type == LP_OP_NOT )
	{
		printf("!(");
		lp_tree_print_rec(node->left);
		printf(")");
	}
	else if( node->type == LP_TOK_TRUE )
	{
		printf("1");
	}
	else if( node->type == LP_TOK_FALSE )
	{
		printf("0");
	}
	else if( node->type == LP_TOK_VARIABLE )
	{
		printf("%s", node->name);
	}
}

void lp_tree_print(TreeNode *node)
{
	lp_tree_print_rec(node);
	printf("\n");
}

void lp_tree_destroy(TreeNode *node)
{
	if(node->left)	lp_tree_destroy(node->left);
	if(node->right)	lp_tree_destroy(node->right);

	free(node);
}
