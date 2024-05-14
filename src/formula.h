#ifndef BT_FORMULA_H_INCLUDED
#define BT_FORMULA_H_INCLUDED

#include <stdint.h>
#include "parse.h"
#include "bitfield.h"

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
    /* Truth table is stored inside a bitfield. */
    Bitfield *truths;
} BT_Formula;

BT_Formula *bt_formula_create(const char *expr,
        char *err_msg, size_t err_msg_size);
void bt_formula_destroy(BT_Formula *);
void bt_formula_print_vars(BT_Formula *);
void bt_formula_print_truth_table(BT_Formula *);
Bitfield *bt_formula_eval_truths(BT_Formula *);

/* Print normal forms of a formula. */
void bt_formula_print_cdnf(BT_Formula *formula);
void bt_formula_print_ccnf(BT_Formula *formula);

typedef struct Implicant
{
    /* Variables with 1 are in the implicant. Variables with 0
     * are not. */
    uint64_t mask;
    uint64_t truths;
} Implicant;

/* Minimize */
void bt_implicants_print(Implicant *implicants, unsigned n,
                         DA_vars *vars);
Implicant *compute_prime_implicants(uint64_t *conjunctions,
                                    const unsigned n_conjunctions,
                                    unsigned *n_implicants_out,
                                    const unsigned n_variables);

#endif
