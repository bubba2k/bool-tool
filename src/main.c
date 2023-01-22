#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "parse.h"
#include "eval.h"

int lp_getline(char *buf, int size, FILE *file)
{
	fgets(buf, size, file);

	char *eol = strstr(buf, "\n");

	if(!eol) return 0;

	*eol = '\0';
	return 1;
}

const char *expressions[5] = {	"(a && b) || c -> (a  && d)",
								"(1 && b) || adc && e || (e)",
								"a 1 w w a 0 d && d",
								"j && k && 0 && 1",
								"M || K && !u" };

int lp_parse_expression_debug(const char * expr_str)
{
	int success = 1;
	const size_t err_msg_size = 256;
	char err_msg[err_msg_size];

	/* Our array of tokens */
	DA_tokens *tokens = DA_tokens_create(10);

	printf("Parsing expression \"%s\"...\n", expr_str);

	/* Tokenization */
	success = lex_expression(expr_str, tokens, err_msg, err_msg_size);
	if(!success)
	{
		printf("Error while lexing expression \"%s\": %s\n", expr_str, err_msg);
		DA_tokens_destroy(tokens);
		return 0;
	}
	printf("Tokenization stage successful:\n");
	DA_tokens_print(tokens);

	/* Check for correct grammar */
	success = syntactically_correct(tokens, err_msg, err_msg_size);
	if(!success)
	{
		printf("Error: Expression is not syntactically correct! %s\n", err_msg);
		DA_tokens_destroy(tokens);
		return 0;
	}
	printf("Expression is syntactically correct!\nBuilding syntax tree...\n\n");

	TreeNode *tree = lp_tree_create(tokens);

    // Ignore empty trees
    if(tree == NULL) return 1;

	printf("Raw tree print:\n");
	lp_tree_print_raw(tree);

	printf("Expression print:\n");
	lp_tree_print(tree);
	printf("\n");

	lp_tree_print_truthtable(tree);

	lp_tree_destroy(tree);

	DA_tokens_destroy(tokens);

	return 1;
}

void lp_easy_truthtable(const char *expr_str)
{
	int success = 1;
	const size_t err_msg_size = 256;
	char err_msg[err_msg_size];

	/* Our array of tokens */
	DA_tokens *tokens = DA_tokens_create(10);

	/* Tokenization */
	success = lex_expression(expr_str, tokens, err_msg, err_msg_size);
	if(!success)
	{
		printf("Error while lexing expression \"%s\": %s\n", expr_str, err_msg);
		DA_tokens_destroy(tokens);
		return;
	}

	/* Check for correct grammar */
	success = syntactically_correct(tokens, err_msg, err_msg_size);
	if(!success)
	{
		printf("Error: Expression is not syntactically correct! %s\n", err_msg);
		DA_tokens_destroy(tokens);
		return;
	}

	TreeNode *tree = lp_tree_create(tokens);

	lp_tree_print_truthtable(tree);

	lp_tree_destroy(tree);
	DA_tokens_destroy(tokens);
}

#define LP_LINE_MAX_SIZE 1024

int main()
{
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
