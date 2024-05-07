#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "formula.h"

int lp_getline(char *buf, int size, FILE *file)
{
	fgets(buf, size, file);

	char *eol = strstr(buf, "\n");

	if(!eol) return 0;

	*eol = '\0';
	return 1;
}

#define LP_LINE_MAX_SIZE 1024

#if DEBUG == 1
/* DEBUG MAIN */

const char *expressions[5] = {	"(a & b) || c -> (a  & d)",
								"(1 & b) || adc & e || (e)",
								"a 1 w w a 0 d & d",
								"j & k & 0 & 1",
								"M | K & !u" };

int bt_process_expression_debug(const char *expr_str)
{
	const size_t err_msg_size = 256;
	char err_msg[err_msg_size];

    printf("[ Processing '%s' ]\n", expr_str);

    /* Attempt to create a formula from the given expression
     * string. */
    BT_Formula *formula = bt_formula_create(expr_str, 
                          err_msg, err_msg_size);
    if(formula == NULL) {
        fprintf(stdout, "Failed to construct syntax "
                "tree from expression '%s': %s\n", 
                expr_str, err_msg);
        return 0;
    }

    printf("Variables in formula:\n");
    bt_formula_print_vars(formula);

	printf("Raw tree print:\n");
	lp_tree_print_raw(formula->syntax_tree);

	printf("Expression print:\n");
	lp_tree_print(formula->syntax_tree);
	printf("\n");

    bt_formula_print_truth_table(formula);
    printf("\n");

    printf("Canonical CNF:\n");
    bt_formula_print_cdnf(formula);
    printf("Canonical DNF:\n");
    bt_formula_print_ccnf(formula);

    bt_formula_destroy(formula);

	return 1;
}

int main()
{
    /* Run the default test expressions. */
    for(unsigned i = 0; i < 5; i++)
        bt_process_expression_debug(expressions[i]);

	char expr[LP_LINE_MAX_SIZE];

    /* Start the 'REPL'. */
	while(1)
	{
		printf("Enter your expression:\n");

		lp_getline(expr, LP_LINE_MAX_SIZE, stdin);
		printf("\n");

		lp_parse_expression_debug(expr);
		printf("\n");
	}

	return 0;
}

/* DEBUG MAIN END */
#else
/* RELEASE MAIN */

void bt_process_expression(const char *expr_str)
{
	const size_t err_msg_size = 256;
	char err_msg[err_msg_size];

    /* Attempt to create a formula from the given expression
     * string. */
    BT_Formula *formula = bt_formula_create(expr_str, 
                          err_msg, err_msg_size);

    if(formula == NULL) {
        fprintf(stdout, "Failed to construct syntax "
                "tree from expression '%s': %s\n", 
                expr_str, err_msg);
        return;
    }

    bt_formula_print_truth_table(formula);

    printf("\nCanonical CNF:\n");
    bt_formula_print_cdnf(formula);
    printf("Canonical DNF:\n");
    bt_formula_print_ccnf(formula);

    bt_formula_destroy(formula);
}

int main()
{
	char expr[LP_LINE_MAX_SIZE];

    /* Start the REPL */
	while(1)
	{
		printf("Enter your expression:\n");

		lp_getline(expr, LP_LINE_MAX_SIZE, stdin);
		printf("\n");

		bt_process_expression(expr);
	}

	return 0;
}

#endif
