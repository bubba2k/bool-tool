#include "eval.h"
#include <string.h>

typedef struct Variable
{
	char name[64];
	unsigned long hash;
	int val;
} Variable;

DYNAMIC_ARRAY_GEN_DECL(Variable, vars)
DYNAMIC_ARRAY_GEN_IMPL(Variable, vars)

/* Quick hashing function for strings */
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

unsigned lp_pow(unsigned a, unsigned b)
{
	int res = a;
	while(--b) res *= a;

	return res;
}

int lp_tree_eval(TreeNode *node, DA_vars *vars)
{
	switch(node->type)
	{
		default:
			fprintf(stderr, "Critical error in eval: Unknown node type.\n");
			break;
		case LP_TOK_AND:
			return lp_tree_eval(node->left, vars) &  lp_tree_eval(node->right, vars);
			break;
		case LP_TOK_OR:
			return lp_tree_eval(node->left, vars) |  lp_tree_eval(node->right, vars);
			break;
		case LP_TOK_IMPLIC:
			return lp_tree_eval(node->left, vars) <= lp_tree_eval(node->right, vars);
			break;
		case LP_TOK_EQU:
			return lp_tree_eval(node->left, vars) == lp_tree_eval(node->right, vars);
			break;
		case LP_TOK_NOT:
			return !lp_tree_eval(node->left, vars);
		case LP_TOK_TRUE:
			return 1;
			break;
		case LP_TOK_FALSE:
			return 0;
			break;
		case LP_TOK_VARIABLE:
			for(size_t i = 0; i < vars->size; i++)
			{
				if(vars->data[i].hash == hash(node->name))
					return vars->data[i].val;
			}
			fprintf(stderr, "ERROR: Var not found!");
			break;
	}
	
	return 0;
}

void lp_tree_find_vars(TreeNode *node, DA_vars *vars)
{
	if(node->type == LP_TOK_VARIABLE)
	{
		int new = 1;
		unsigned long hash_val = hash(node->name);

		// We do not want any duplicates in the list!
		for(size_t i = 0; i < vars->size; i++)
		{
			if(DA_vars_get(vars, i)->hash == hash_val)
				new = 0;
		}

		// Only add variable to list if not available already
		if(new)
		{
			Variable var = { .hash = hash_val, .val = 0 };
			strcpy(var.name, node->name);
			DA_vars_push_back(vars, &var);
		}
	}
	
	if(node->left)  lp_tree_find_vars(node->left,  vars);
	if(node->right) lp_tree_find_vars(node->right, vars);
}

// Sorts the dynamic array of variables alphabetically according to their names
void DA_vars_sort(DA_vars *vars)
{
	int sorted = 1;
	do
	{
		sorted = 1;
		for(size_t i = 0; i < vars->size - 1; i++)
		{
			if(strcmp(vars->data[i].name, vars->data[i+1].name) > 0)
			{
				struct Variable tmp = vars->data[i];
				vars->data[i] 		= vars->data[i+1];
				vars->data[i+1] 	= tmp;

				sorted = 0;
			}
		}
	} while (!sorted);
}

void lp_tree_print_truthtable(TreeNode *tree)
{
	DA_vars *vars = DA_vars_create(10);
	lp_tree_find_vars(tree, vars);

	DA_vars_sort(vars);
	

	if(vars->size == 0) // Can't print table without variables
	{
		printf("%d\n", lp_tree_eval(tree, vars));
		return;
	}

	// Print table head
	for(size_t i = 0; i < vars->size; i++)
	{
		printf(" %s |", vars->data[i].name);
	}
	printf("\n");
	for(size_t i = 0; i < vars->size; i++)
	{
		int len = strlen(vars->data[i].name) + 3;
		while(len--) printf("-");
	}
	printf("---");
	printf("\n");
	
	// A little hack to generate all possible rows
	// (Yes, this will fail and explode if there are more than 64 variables)
	// TODO: Invert table
	unsigned long num_rows = lp_pow(2, vars->size);
	unsigned long i = 0;
	for(i = 0; i < num_rows; i++)
	{
		// Update the variable values
		for(size_t j = 0; j < vars->size; j++)
		{
			vars->data[vars->size - j - 1].val = (i >> j) & 0x1;
		}

		// Print the variable names
		for(size_t j = 0; j < vars->size; j++)
		{
			int len = strlen(vars->data[j].name) - 1;
			while(len--) printf(" ");
			printf(" %d |", vars->data[j].val);
		}

		printf(" %d\n", lp_tree_eval(tree, vars));
	}

	DA_vars_destroy(vars);
}

