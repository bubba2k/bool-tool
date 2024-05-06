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

const char *expressions[5] = {	"(a & b) || c -> (a  & d)",
								"(1 & b) || adc & e || (e)",
								"a 1 w w a 0 d & d",
								"j & k & 0 & 1",
								"M | K & !u" };

int lp_parse_expression_debug(const char *expr_str)
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

    Bitfield *truths = bt_formula_eval_truths(formula);
    bitfield_print_le(truths);
    printf("\n");

    bitfield_destroy(truths);
    bt_formula_destroy(formula);

	return 1;
}

void lp_easy_truthtable(const char *expr_str)
{
	const size_t err_msg_size = 256;
	char err_msg[err_msg_size];

    /* Attempt to create a formula from the given expression
     * string. */
    BT_Formula *formula = bt_formula_create(expr_str, 
                          err_msg, err_msg_size);

    bt_formula_destroy(formula);
}

#define LP_LINE_MAX_SIZE 1024

int main()
{
#if DEBUG == 1
    for(unsigned i = 0; i < 5; i++)
        lp_parse_expression_debug(expressions[i]);
#endif
	char expr[LP_LINE_MAX_SIZE];

	while(1)
	{
		printf("Enter your expression:\n");

		lp_getline(expr, LP_LINE_MAX_SIZE, stdin);
		printf("\n");

		// Debug mode or not
#if DEBUG == 1

		lp_parse_expression_debug(expr);
#else
		lp_easy_truthtable(expr);
#endif

		printf("\n");
	}

	return 0;
}
