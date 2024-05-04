#include "eval.h"
#include <string.h>

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

