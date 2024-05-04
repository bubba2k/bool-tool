#include "lex.h"
#include "tokens.h"
#include "parse.h"

TreeNode *lp_treenode_create(int type, const char *name, TreeNode *_left, TreeNode *_right)
{
	TreeNode *node = malloc(sizeof(TreeNode));
	node->type 		= type;
	snprintf(node->name, 64, "%s", name);

	node->left  = _left;
	node->right = _right;

	return node;
}

static const Token end_token = { .type = LP_TOK_UNKNOWN, .name = "\0" };
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
TreeNode *parse_impl();
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
	TreeNode *a = parse_impl();

	if(next_token->type == LP_TOK_EQU)
	{
		scan_token();
		return lp_treenode_create(LP_TOK_EQU, "=", a, parse_expr());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_impl()
{
	TreeNode *a = parse_disj();

	if(next_token->type == LP_TOK_IMPLIC)
	{
		scan_token();
		return lp_treenode_create(LP_TOK_IMPLIC, "->", a, parse_expr());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_disj()
{
	TreeNode *a = parse_conj();

	if(next_token->type == LP_TOK_OR)
	{
		scan_token();
		return lp_treenode_create(LP_TOK_OR, "||", a, parse_disj());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_conj()
{
	TreeNode *a = parse_nega();

	if(next_token->type == LP_TOK_AND)
	{
		scan_token();
		return lp_treenode_create(LP_TOK_AND, "&&", a, parse_conj());
	}
	else
	{
		return a;
	}
}

TreeNode *parse_nega()
{
	if(next_token->type == LP_TOK_NOT)
	{
		scan_token();
		return lp_treenode_create(LP_TOK_NOT, "!", parse_nega(), NULL);
	}
	else
	{
		return parse_atom();
	}
}

TreeNode *parse_atom()
{
	if(next_token->type == LP_TOK_FALSE)
	{
		TreeNode *a = lp_treenode_create(LP_TOK_FALSE, "0", NULL, NULL);
		scan_token();

		return a;
	}
	else if(next_token->type == LP_TOK_TRUE)
	{
		TreeNode *a = lp_treenode_create(LP_TOK_TRUE, "1", NULL, NULL);
		scan_token();

		return a;
	}
	else if(next_token->type == LP_TOK_VARIABLE)
	{
		TreeNode *a = lp_treenode_create(LP_TOK_VARIABLE, next_token->name, NULL, NULL);
		scan_token();

		return a;
	}
	else if(next_token->type == LP_TOK_PAR_OPEN)
	{
		scan_token();

		TreeNode *a = parse_expr();

		// Skip the parenthesis
		scan_token();

		return a;
	}
	else return NULL; // This should never happen!
}

void lp_tree_print_raw_rec(TreeNode *node, int depth)
{
	for(int i = 0; i < depth; i++)
		printf(" ");

	token_print((Token *) node);

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
	if( node->type == LP_TOK_OR || node->type == LP_TOK_AND
		|| node->type == LP_TOK_IMPLIC )
	{
		printf("(");
		lp_tree_print_rec(node->left);
		printf(" %s ", node->name);
		lp_tree_print_rec(node->right);
		printf(")");
	}
	else if( node->type == LP_TOK_NOT )
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
