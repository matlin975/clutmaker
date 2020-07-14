/* command_parser.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

#include "main.h"
#include "command_parser.h"

char is_math_operation(char arg){
	if (arg == '+' || arg == 'p' || arg == '*' || arg == '/' || arg == '^'){
		return arg;
	}
	return false;
}

void str_to_float(char *arg, math_operation_t *op){
	if (verbose == true)printf("str_to_float in:\t\t%s\n", arg);

	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	char operation = 0;
	int operator_position = 0;
	for (int i = 0; i<=arg_len; i++){
		if (i > 2*MAX_NUMBER_LENGTH) exit_function(EXIT_FAILURE);
		if (arg[i] == ',') arg[i] = '.';
		if(is_math_operation(arg[i])){
			op->symbol = arg[i];
			strncpy(num_one_str, arg, i);
			num_one_str[i] = '\0';
			operator_position = i+1;
		}else if (arg[i] == '\0' && operator_position > 1){
			strncpy(num_two_str, &arg[operator_position], i-operator_position);
			num_two_str[i-operator_position] = '\0';
		}
	}
	op->num_one = atof(num_one_str);
	op->num_two = atof(num_two_str);

	if (verbose == true)printf("str_to_float out:\t\t%lf%c%lf\n",
	                            op->num_one, op->symbol, op->num_two);
}

int parse_parenthesis(char *arg, char *ret){
    // int verbose = 1;
    if (verbose == true)printf("parse_parenthesis in:\t\t%s\n", arg);
    int opening_pos = 0;
    int closing_pos = 0;
    int no_parenthesis = 0;

    for (int i = 0; i<strlen(arg); i++){
        if (arg[i] == '('){
            arg[i] = ' ';
            opening_pos = i;
            no_parenthesis++;
        }else if (arg[i] == ')'){
            arg[i] = ' ';
            closing_pos = i;
            no_parenthesis++;
        }
    }
    if (no_parenthesis%2 != 0 && no_parenthesis != 0){
        fprintf(stderr, "Parenthesis error: %d\n", no_parenthesis);
        exit_function(EXIT_FAILURE);
        return 0;
    }else if (no_parenthesis >= 2 && (no_parenthesis%2 == 0)){
        strncpy(ret, &arg[opening_pos+1], closing_pos-opening_pos-1);
        if(verbose == true)printf("parse_parenthesis out:\t\t%s\n", ret);
        return 1;
    }
    return 0;
}

char parse_exponents(char *arg, char *ret, char operator){
    // int verbose = 1;
	if (verbose == true)printf("parse_exponents in:\t\t%s\n", arg);
	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	int operator_position = 0;
	int prev_operator_position = 0;
	int next_operator_position = 0;
	for (int i = arg_len; i>=0; i--){
		if (arg[i] == ',') arg[i] = '.';
		if(arg[i] == operator){
			operator_position = i;
			next_operator_position = i+1;
			if (arg[next_operator_position] != '\0'){
				while(!is_math_operation(arg[next_operator_position])){
						next_operator_position++;
						if (arg[next_operator_position] == '\0'){
							break;
						}
				}
			}
			prev_operator_position = i-1;
			if (arg[prev_operator_position] != '\0'){
				while(!is_math_operation(arg[prev_operator_position])){
					prev_operator_position--;
					if (arg[prev_operator_position] == 0){
						prev_operator_position = 0;
						break;
					}
				}
			}
			if (operator_position > 2){
				if (prev_operator_position > 0){
					strncpy(ret, &arg[prev_operator_position+1], next_operator_position-prev_operator_position-1);
					ret[next_operator_position-prev_operator_position-1] = '\0';
				}else{
					strncpy(ret, &arg[prev_operator_position], next_operator_position-prev_operator_position);
					ret[next_operator_position-prev_operator_position] = '\0';
				}
			} else{
				strncpy(ret, &arg[prev_operator_position], next_operator_position-prev_operator_position);
				ret[next_operator_position-prev_operator_position] = '\0';
			}
		}
	}
	if(operator_position > 0){
		if (verbose == true)printf("parse_exponents out:\t\t%s\n", ret);
		return operator;
	}else{
		return 0;
	}
}

char parse_multiplication(char *arg, char *ret, char operator){
	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

    // int verbose = 1;
	if (verbose == true)printf("parse_multiplication in:\t%s\n", arg);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	int operator_position = 0;
	int prev_operator_position = 0;
	int next_operator_position = 0;
	for (int i = arg_len; i>=0; i--){
		if (arg[i] == ',') arg[i] = '.';
		if(is_math_operation(arg[i]) == '*' || is_math_operation(arg[i]) == '/'){
			operator_position = i;
			next_operator_position = i+1;
			if (arg[next_operator_position] != '\0'){
				while(!is_math_operation(arg[next_operator_position])){
						next_operator_position++;
						if (arg[next_operator_position] == '\0'){
							break;
						}
				}
			}
			prev_operator_position = i-1;
			if (arg[prev_operator_position] != '\0'){
				while(!is_math_operation(arg[prev_operator_position])){
					prev_operator_position--;
					if (arg[prev_operator_position] == 0){
						prev_operator_position = 0;
						break;
					}
				}
			}
			if (operator_position > 2){
				if (prev_operator_position > 0){
					strncpy(ret, &arg[prev_operator_position+1], next_operator_position-prev_operator_position-1);
					ret[next_operator_position-prev_operator_position-1] = '\0';
				}else{
					strncpy(ret, &arg[prev_operator_position], next_operator_position-prev_operator_position);
					ret[next_operator_position-prev_operator_position] = '\0';
				}
			} else{
				strncpy(ret, &arg[prev_operator_position], next_operator_position-prev_operator_position);
				ret[next_operator_position-prev_operator_position] = '\0';
			}
		}
	}
	if(operator_position > 0){
		if (verbose == true)printf("parse_multiplication out:\t%s\n", ret);
		return operator;
	}else{
		return 0;
	}
}

char parse_addition(char *arg, char *ret, char operator){
    // int verbose = 1;
	if (verbose == true)printf("parse_addition in:\t\t%s\n", arg);
	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	int operator_position = 0;
	int prev_operator_position = 0;
	int next_operator_position = 0;
	for (int i = arg_len; i>=0; i--){
		if (arg[i] == ',') arg[i] = '.';
		if(is_math_operation(arg[i]) == '+' || is_math_operation(arg[i]) == '-'){
			operator_position = i;
			next_operator_position = i+1;
			if (arg[next_operator_position] != '\0'){
				// while(is_math_operation(arg[next_operator_position]) == false){
				while(!is_math_operation(arg[next_operator_position])){
						next_operator_position++;
						if (arg[next_operator_position] == '\0'){
							break;
						}
				}
			}
			prev_operator_position = i-1;
			if (arg[prev_operator_position] != '\0'){
				while(!is_math_operation(arg[prev_operator_position])){
					prev_operator_position--;
					if (arg[prev_operator_position] == 0){
						prev_operator_position = 0;
						break;
					}
				}
			}
			if (operator_position > 2){
				if (prev_operator_position > 0){
					strncpy(ret, &arg[prev_operator_position+1], next_operator_position-prev_operator_position-1);
					ret[next_operator_position-prev_operator_position-1] = '\0';
				}else{
					strncpy(ret, &arg[prev_operator_position], next_operator_position-prev_operator_position);
					ret[next_operator_position-prev_operator_position] = '\0';
				}
			} else{
				strncpy(ret, &arg[prev_operator_position], next_operator_position-prev_operator_position);
				ret[next_operator_position-prev_operator_position] = '\0';
			}
		}
	}

	if(operator_position > 0){
		if (verbose == true)printf("parse_addition out:\t\t%s\n", ret);
		return operator;
	}else{
		return 0;
	}
}

double parse_functions(char *arg, char *ret){
	if(verbose==true)printf("parse_functions in:\t\t%s\n", arg);
	char *pos;
	double result = 0;
	if(pos = strstr(arg, "sin")){
		sscanf(pos, "sin %lf", &result);
		result = sin(result*PI);
		str_insert_double(arg, pos, result);
		printf("after_func: %s\n", arg);
		if(verbose==true)printf("parse_functions out:\t\t%lf\n", result);
		return result;
	}else if (pos = strstr(arg, "cos")){
		sscanf(pos, "cos %lf", &result);
		result = cos(result*PI);
		str_insert_double(arg, pos, result);
		printf("after_func: %s\n", arg);
		if(verbose==true)printf("parse_functions out:\t\t%lf\n", result);
		return 1;
	}else if (pos = strstr(arg, "tan")){
		sscanf(pos, "tan %lf", &result);
		result = tan(result*PI);
		str_insert_double(arg, pos, result);
		printf("after_func: %s\n", arg);
		if(verbose==true)printf("parse_functions out:\t\t%lf\n", result);
		return 1;
	}else if (pos = strstr(arg, "sqrt")){
		sscanf(pos, "sqrt %lf", &result);
		result = sqrtl(result);
		str_insert_double(arg, pos, result);
		printf("after_func: %s\n", arg);
		if(verbose==true)printf("parse_functions out:\t\t%lf\n", result);
		return 1;
	}else if (pos = strstr(arg, "log")){
		sscanf(pos, "log %lf", &result);
		result = logl(result);
		str_insert_double(arg, pos, result);
		printf("after_func: %s\n", arg);
		if(verbose==true)printf("parse_functions out:\t\t%lf\n", result);
		return 1;
	}
	return 0;
}
