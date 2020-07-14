/* command_parser.h */

#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

#define MAX_NUMBER_LENGTH 64

char is_math_operation(char arg);
void str_to_float(char *arg, math_operation_t *op);

int parse_parenthesis(char *arg, char *ret);
char parse_exponents(char *arg, char *ret, char operator);
char parse_multiplication(char *arg, char *ret, char operator);
char parse_addition(char *arg, char *ret, char operator);
double parse_functions(char *arg, char *ret);

#endif /* _COMMAND_PARSER_H */
