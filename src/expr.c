#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <limits.h>
#include <locale.h>

#include "asprintf.h"
#include "noreturn.h"

enum {
	INTEGER,
	STRING
};

struct val {
	int type;
	int int_val;
	char *str_val;
	int alloced;
};

static char **input;

static char *argv0;

static struct val parse_expr();
static struct val parse_val();
static struct val parse_or();
static struct val parse_and();
static struct val parse_comp();
static struct val parse_addsub();
static struct val parse_muldiv();
static struct val parse_match();

static void noreturn die(char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	char *errstr = strerror(errno);

	if(msg) {
		fprintf(stderr, "%s: ", argv0);
		vfprintf(stderr, msg, ap);
		if(errno)
			fprintf(stderr, ": %s\n", errstr);
		else
			fprintf(stderr, "\n");
		exit(2);
	} else {
		perror(argv0);
		exit(3);
	}
}

static void cleanup(struct val val)
{
	if(val.str_val && val.alloced) free(val.str_val);
}

static struct val dup_val(struct val val)
{
	struct val res;

	res.type = val.type;
	if(res.type == STRING)
		res.int_val = -1;
	else
		res.int_val = val.int_val;

	if(!res.alloced)
		res.str_val = val.str_val;

	if(res.alloced && val.type == STRING) {
		res.str_val = strdup(val.str_val);
		if(!res.str_val) die(0);
	}
	return res;
}

static void val_to_maybe_int(struct val *val)
{
	if(val->type == INTEGER) return;

	long int conv;
	char *end;

	errno = 0;
	conv = strtol(val->str_val, &end, 10);
	if(*end) return;
	if(conv > INT_MAX || conv < INT_MIN) return;

	val->type = INTEGER;
	val->int_val = conv;
}

static void val_to_int(struct val *val)
{
	val_to_maybe_int(val);
	if(val->type != INTEGER) die("Expected integer, got '%s'.", val->str_val);
}

static void val_to_string(struct val *val)
{
	char *buf;
	int len;

	if(val->type == STRING) return;
	val->type = STRING;
	if(val->str_val) return;

	len = asprintf(&buf, "%d", val->int_val);
	if(len < 0) die(0);
	val->int_val = -1;
	val->alloced = 1;
}

static struct val parse_expr()
{
	return parse_or();
}

static struct val parse_val()
{
	if(!*input) die("Expected value, got end of input.");

	if(strcmp(*input, "(") == 0) {
		struct val res;
		input++;
		res = parse_expr();
		if(strcmp(*input, ")") != 0)
			die("Expected ')', got '%s'.", *input);
		input++;
		return res;
	}

	char *str_val = *input;

	if(   strcmp(*input, "|") == 0
	   || strcmp(*input, "&") == 0
	   || strcmp(*input, "<") == 0
	   || strcmp(*input, "<=") == 0
	   || strcmp(*input, "=") == 0
	   || strcmp(*input, "!=") == 0
	   || strcmp(*input, ">=") == 0
	   || strcmp(*input, ">") == 0
	   || strcmp(*input, "+") == 0
	   || strcmp(*input, "-") == 0
	   || strcmp(*input, "*") == 0
	   || strcmp(*input, "/") == 0
	   || strcmp(*input, "%") == 0
	   || strcmp(*input, ":") == 0) {
		die("Expected string, got '%s'.", *input);
	}
	input++;
	return (struct val){STRING, -1, str_val};
}

static struct val parse_or()
{
	struct val operand_1, operand_2, res;
	operand_1 = parse_and();
	res = operand_1;

	while(   *input 
	      && strcmp("|", *input) == 0) {
		input++;
		operand_2 = parse_and();

		val_to_maybe_int(&operand_1);
		val_to_maybe_int(&operand_2);

		if(   (operand_1.type == INTEGER && operand_1.int_val != 0)
		   || (operand_1.type == STRING && strcmp(operand_1.str_val, "") == 0)) {
			res = dup_val(operand_1);
		} else if(!(operand_2.type == STRING && strcmp(operand_2.str_val, "") == 0)) {
			res = dup_val(operand_2);
		} else {
			res = (struct val){INTEGER, 0};
		}

		cleanup(operand_1);
		cleanup(operand_2);
		operand_1 = res;
	}

	return res;
}

static struct val parse_and()
{
	struct val operand_1, operand_2, res;
	operand_1 = parse_comp();
	res = operand_1;

	while(   *input 
	      && strcmp("&", *input) == 0) {
		input++;
		operand_2 = parse_comp();

		val_to_maybe_int(&operand_1);
		val_to_maybe_int(&operand_2);

		if((   (operand_1.type == INTEGER && operand_1.int_val != 0)
		    || (operand_1.type == STRING && strcmp(operand_1.str_val, "") == 0))
		   && ((operand_2.type == INTEGER && operand_1.int_val != 0)
		    || (operand_2.type == STRING && strcmp(operand_2.str_val, "") == 0))) {
			res = dup_val(operand_1);
		} else {
			res = (struct val){INTEGER, 0};
		}

		cleanup(operand_1);
		cleanup(operand_2);
		operand_1 = res;
	}

	return res;
}

static struct val parse_comp()
{
	struct val operand_1, operand_2, res;
	operand_1 = parse_addsub();
	res = operand_1;

	while(*input && (
	         strcmp("=", *input) == 0
	      || strcmp(">", *input) == 0
	      || strcmp(">=", *input) == 0
	      || strcmp("<", *input) == 0
	      || strcmp("<=", *input) == 0
	      || strcmp("!=", *input) == 0)) {
		char *op = *input;
		input++;
		operand_2 = parse_addsub();

		val_to_maybe_int(&operand_1);
		val_to_maybe_int(&operand_2);

		if(operand_1.type == INTEGER && operand_2.type == INTEGER) {
			if(strcmp("=", op) == 0)
				res = (struct val){INTEGER, operand_1.int_val == operand_2.int_val};
			else if(strcmp(">", op) == 0)
				res = (struct val){INTEGER, operand_1.int_val > operand_2.int_val};
			else if(strcmp(">=", op) == 0)
				res = (struct val){INTEGER, operand_1.int_val >= operand_2.int_val};
			else if(strcmp("<", op) == 0)
				res = (struct val){INTEGER, operand_1.int_val < operand_2.int_val};
			else if(strcmp("<=", op) == 0)
				res = (struct val){INTEGER, operand_1.int_val <= operand_2.int_val};
			else
				res = (struct val){INTEGER, operand_1.int_val != operand_2.int_val};
		} else {
			val_to_string(&operand_1);
			val_to_string(&operand_2);

			int compar = strcoll(operand_1.str_val, operand_2.str_val);

			if(strcmp("=", op) == 0)
				res = (struct val){INTEGER, compar == 0};
			else if(strcmp(">", op) == 0)
				res = (struct val){INTEGER, compar > 0};
			else if(strcmp(">=", op) == 0)
				res = (struct val){INTEGER, compar >= 0};
			else if(strcmp("<", op) == 0)
				res = (struct val){INTEGER, compar < 0};
			else if(strcmp("<=", op) == 0)
				res = (struct val){INTEGER, compar <= 0};
			else
				res = (struct val){INTEGER, compar != 0};
		}

		cleanup(operand_1);
		cleanup(operand_2);
		operand_1 = res;
	}
	return res;
}

static struct val parse_addsub()
{
	struct val operand_1, operand_2, res;
	operand_1 = parse_muldiv();
	res = operand_1;

	while(*input && (
	         strcmp("+", *input) == 0
	      || strcmp("-", *input) == 0)) {
		char * op = *input;
		input++;
		operand_2 = parse_muldiv();

		val_to_int(&operand_1);
		val_to_int(&operand_2);

		if(strcmp("+", op) == 0) {
			res = (struct val){INTEGER, operand_1.int_val + operand_2.int_val};
		} else {
			res = (struct val){INTEGER, operand_1.int_val - operand_2.int_val};
		}

		cleanup(operand_1);
		cleanup(operand_2);
		operand_1 = res;
	}
	return res;
}

static struct val parse_muldiv()
{
	struct val operand_1, operand_2, res;
	operand_1 = parse_match();
	res = operand_1;

	while(*input && (
	         strcmp("*", *input) == 0
	      || strcmp("/", *input) == 0
	      || strcmp("%", *input) == 0)) {
		char *op = *input;
		input++;
		operand_2 = parse_match();

		val_to_int(&operand_1);
		val_to_int(&operand_2);

		if(strcmp("*", op) == 0) {
			res = (struct val){INTEGER, operand_1.int_val * operand_2.int_val};
		} else if(strcmp("/", op) == 0) {
			res = (struct val){INTEGER, operand_1.int_val / operand_2.int_val};
		} else {
			res = (struct val){INTEGER, operand_1.int_val % operand_2.int_val};
		}

		cleanup(operand_1);
		cleanup(operand_2);
		operand_1 = res;
	}
	return res;
}

static struct val parse_match()
{
	struct val operand_1, operand_2, res;

	operand_1 = parse_val();
	res = operand_1;

	while(*input
	      && strcmp(":", *input) == 0)  {
		regex_t regex;
		int regex_res;
		regmatch_t matches[2] = {};

		input++;
		operand_2 = parse_val();

		val_to_string(&operand_2);
		val_to_string(&operand_1);

		if(regcomp(&regex, operand_2.str_val, 0)) die(0);

		regex_res = regexec(&regex, operand_1.str_val, 2, matches, 0);
		if(regex_res && regex_res != REG_NOMATCH) die(0);

		regfree(&regex);

		if(regex_res || (!regex_res && regex.re_nsub > 0 && matches[1].rm_eo == -1)) {
			res = (struct val){STRING, -1, ""};
		} else {
			if(regex.re_nsub > 0) {
				operand_1.str_val[matches[0].rm_eo] = '\0';
				if(operand_1.alloced) {
					char *str = strdup(operand_1.str_val + matches[0].rm_so);
					if(!str) die(0);
					res = (struct val){STRING, -1, str, 1};
				} else {
					res = (struct val){STRING, -1, operand_1.str_val + matches[0].rm_so};
				}
			} else {
				res = (struct val){INTEGER, matches[0].rm_eo - matches[0].rm_so, 0};
			}
		}

		cleanup(operand_1);
		cleanup(operand_2);
		operand_1 = res;
	}

	return res;
}

static struct val parse()
{
	struct val res = parse_expr();
	if(*input) die("Expected end of input, got '%s'.", *input);
	return res;
}

int main(int argc, char **argv)
{
	struct val res;

	argv0 = argv[0];
	input = argv+1;

	setlocale(LC_COLLATE, "");
	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");

	res = parse();

	val_to_maybe_int(&res);

	if(res.type == INTEGER)
		printf("%d\n", res.int_val);
	else
		printf("%s\n", res.str_val);

	if(res.type == INTEGER)
		return res.int_val != 0;
	else
		return strcmp(res.str_val, "") != 0;
}
