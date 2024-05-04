#include "formula.h"
#include "tokens.h"
#include "parse.h"
#include "lex.h"

/* Generate the implementation for the dynamic array of
 * variables. */
DYNAMIC_ARRAY_GEN_IMPL(Variable, vars)

/* Helper function to print list of variables. */
void var_print(Variable *var)
{
	printf("%s", var->name);
}

void DA_vars_print(DA_vars *arr)
{
	printf("Array of %lu variables;\n{ ", arr->size);
	for(unsigned int i = 0; i < arr->size; i++)
	{
		var_print(arr->data + i);
        printf(", ");
	}
    printf("}\n");
}

/* A simple pow function for unsigned integers, so we do not
   have to link the math library. */
unsigned lp_pow(unsigned a, unsigned b)
{
    if(b == 0) return 1;

	int res = a;
	while(--b) res *= a;

	return res;
}

/* Quick hashing function for strings */
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void lp_tree_find_vars(TreeNode *node, DA_vars *vars)
{
    /* Abort if tree is empty. The list of variables would
     * be empty anyway, obviously. */
    if(node == NULL) return;

	if(node->type == LP_TOK_VARIABLE)
	{
		int new = 1;
		unsigned long hash_val = hash(node->name);

		/* We do not want any duplicates in the list! */
		for(size_t i = 0; i < vars->size; i++)
		{
			if(DA_vars_get(vars, i)->hash == hash_val)
				new = 0;
		}

		/* Only add variable to list if not available already. */
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

/* Sorts the dynamic array of variables in reverse alphabetical 
 * order. */
void DA_vars_sort(DA_vars *vars)
{
	/* Do not attempt to sort empty array! */
	if(vars->size == 0)
		return;

    /* Yes, this is bubblesort. But it is pretty unlikely that 
     * a single formula contains more than a dozen unique
     * variables. Thus, it shall not matter. */
	int sorted = 1;
	do
	{
		sorted = 1;
		for(size_t i = 0; i < vars->size - 1; i++)
		{
			if(strcmp(vars->data[i].name, vars->data[i+1].name) < 0)
			{
				struct Variable tmp = vars->data[i];
				vars->data[i] 		= vars->data[i+1];
				vars->data[i+1] 	= tmp;

				sorted = 0;
			}
		}
	} while (!sorted);
}

BT_Formula *bt_formula_create(const char *expr_str, char *err_msg, size_t err_msg_size)
{
    int success = 1;

	/* The array the tokens are stored in temporarily. */
	DA_tokens *tokens = DA_tokens_create(20);
    /* The array the variables are stored in. */
    DA_vars *vars = DA_vars_create(20);

	/* Tokenization */
	success = lex_expression(expr_str, tokens, err_msg, err_msg_size);
    if(!success) return NULL;

	/* Check for correctness of expression */
	success = syntactically_correct(tokens, err_msg, err_msg_size);
	if(!success) return NULL;

    /* Construct the syntax tree. This is guaranteed to succeed
     * after checking for syntactical correctness above. */
	TreeNode *tree = lp_tree_create(tokens);

    /* If the tree is empty, abort!. An empty formula is not a
     * formula, for our purposes. */
    if(tree == NULL) {
        snprintf(err_msg, err_msg_size, "Expression is empty.");
        return NULL;
    }

    /* Find the variables inside the syntax tree. */
    lp_tree_find_vars(tree, vars);

    /* Allocate the formula struct, assign members. */
    BT_Formula *formula = malloc(sizeof(BT_Formula));
    formula->syntax_tree = tree;
    formula->variables   = vars;
    formula->truths      = 0;

    /* Free the token array. */
	DA_tokens_destroy(tokens);

    /* Return the formula struct. */
    return formula;
}

void bt_formula_destroy(BT_Formula *formula) 
{
    /* Ignore null argument. */
    if(formula == NULL) return;

    /* Free the syntax tree. */
    lp_tree_destroy(formula->syntax_tree);
    /* Free the variable array. */
    DA_vars_destroy(formula->variables);
    /* Free the struct itself. */
    free(formula);
}

void bt_formula_print_vars(BT_Formula *formula)
{
    DA_vars_print(formula->variables);
}