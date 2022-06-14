#include <string.h>
#include "parse.h"

const char *expressions[5] = {	"(a && b) || c -> (a  && d)",
								"(b && b) || adc && e || (e)",
								"a iw  w w a d d && d",
								"j && k && v && T",
								"M || K && !u" };

int parse_expression(const char * expr_str)
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
	printf("Expression is syntactically correct!\n");

	DA_tokens_destroy(tokens);

	return 1;
}

int main()
{
	for(unsigned int i = 0; i < 5; i++)
	{
		parse_expression(expressions[i]);
		printf("\n");
	}

	return 0;
}
