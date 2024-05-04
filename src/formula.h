#ifndef BT_FORMULA_H_INCLUDED
#define BT_FORMULA_H_INCLUDED

#include <stdint.h>
#include "parse.h"

typedef struct Variable {
	char name[64];
	unsigned long hash;
	int val;
} Variable;

/* Use this macro declare a dynamic array of struct Variable
 * type. */
DYNAMIC_ARRAY_GEN_DECL(Variable, vars)

typedef struct BT_Formula 
{
    /* Syntax tree of the boolean expression. */
    TreeNode *syntax_tree;
    /* A list of each (unique) variable present in the formula. */
    DA_vars *variables;
    /* The 'truth table' of the expression. */
    uint64_t truths;
} BT_Formula;

BT_Formula *bt_formula_create(const char *expr,
        char *err_msg, size_t err_msg_size);
void bt_formula_destroy(BT_Formula *);
void bt_formula_print_vars(BT_Formula *);


#endif
