/* ============================== clutmaker ===================================
 * main.c
*/

#include "main.h"
#include "gnuplot_i.h"
#define FORMAT_ERROR 10
#define MAX_NUMBER_LENGTH 64
#define MAX_FUNCTION_LENGTH 256
#define MAX_NO_ELEMENTS 1024*64
#define PI 3.14159265

bool verbose = 1;
// =100 * ( 1 + (0,003908*10) + (-5,78E-07*10^2) + (-4,2E-12 * (10-100) * 10^3))
/* Structs */

typedef struct operation{
	double num_one;
	double num_two;
	char symbol;
}operation;

char is_operation(char arg){
	if (arg == '+' || arg == 'p' || arg == '*' || arg == '/' || arg == '^'){
		return arg;
	}
	return false;
}
void str_to_float(char *arg, operation *op){
	// if (verbose == true)printf("str_to_float in:\t\t%s\n", arg);

	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	char operation = 0;
	int operator_position = 0;
	for (int i = 0; i<=arg_len; i++){
		if (i > 2*MAX_NUMBER_LENGTH) exit_function(EXIT_FAILURE);
		if (arg[i] == ',') arg[i] = '.';
		if(is_operation(arg[i])){
			op->symbol = arg[i];
			strncpy(num_one_str, arg, i);
			num_one_str[i] = '\0';
			operator_position = i+1;
		}else if (arg[i] == '\0' && operator_position > 1){
			strncpy(num_two_str, &arg[operator_position], i-operator_position);
			num_two_str[i-operator_position] = '\0';
		}
	}
	// char **endptr;
	op->num_one = atof(num_one_str);
	// op->num_one = strtod(num_one_str, endptr);
	op->num_two = atof(num_two_str);
	// op->num_two = strtod(num_two_str, endptr);
	// if(endptr == NULL)exit_function(errno);

	// if (verbose == true)printf("str_to_float out:\t\t%lf%c%lf\n",
	                            // op->num_one, op->symbol, op->num_two);
}
char parse_exponents(char *arg, char *ret, char operation){

	if (verbose == true)printf("parse_exponents in:\t\t%s\n", arg);
	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	// char operation = '^';
	int operator_position = 0;
	int prev_operator_position = 0;
	int next_operator_position = 0;
	// for (int i = 0; i<=arg_len; i++){
	for (int i = arg_len; i>=0; i--){
		if (arg[i] == ',') arg[i] = '.';
		// if (i > 2*MAX_NUMBER_LENGTH) exit_function(EXIT_FAILURE);
		if(arg[i] == operation){
		// if(is_operation(arg[i])){
			operator_position = i;

			next_operator_position = i+1;
			if (arg[next_operator_position] != '\0'){
				// while(is_operation(arg[next_operator_position]) == false){
				while(!is_operation(arg[next_operator_position])){
						next_operator_position++;
						if (arg[next_operator_position] == '\0'){
							break;
						}
				}
				// printf("i:%d \t nop: %d\n", i, next_operator_position);
			}

			prev_operator_position = i-1;
			if (arg[prev_operator_position] != '\0'){
				while(!is_operation(arg[prev_operator_position])){
					prev_operator_position--;
					if (arg[prev_operator_position] == 0){
						prev_operator_position = 0;
						break;
					}
				}
				// printf("i:%d \t pop: %d\n", i, prev_operator_position);
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

			// num_one_str[next_operator_position-prev_operator_position] = '\0';
			// ret[next_operator_position-prev_operator_position-1] = '\0';
		}
	}

	// printf("op_pos %d\n", operator_position);
	if(operator_position > 0){
		if (verbose == true)printf("parse_exponents out:\t\t%s\n", ret);
		return operation;
	}else{
		return 0;
	}
	// return operator_position;
}
char parse_multiplication(char *arg, char *ret, char operation){
	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	if (verbose == true)printf("parse_multiplication in:\t%s\n", arg);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	int operator_position = 0;
	int prev_operator_position = 0;
	int next_operator_position = 0;
	for (int i = arg_len; i>=0; i--){
		if (arg[i] == ',') arg[i] = '.';
		if(is_operation(arg[i]) == '*' || is_operation(arg[i]) == '/'){
			operator_position = i;

			next_operator_position = i+1;
			if (arg[next_operator_position] != '\0'){
				// while(is_operation(arg[next_operator_position]) == false){
				while(!is_operation(arg[next_operator_position])){
						next_operator_position++;
						if (arg[next_operator_position] == '\0'){
							break;
						}
				}
				// printf("i:%d \t nop: %d\n", i, next_operator_position);
			}

			prev_operator_position = i-1;
			if (arg[prev_operator_position] != '\0'){
				while(!is_operation(arg[prev_operator_position])){
					prev_operator_position--;
					if (arg[prev_operator_position] == 0){
						prev_operator_position = 0;
						break;
					}
				}
				// printf("i:%d \t pop: %d\n", i, prev_operator_position);
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
			// num_one_str[next_operator_position-prev_operator_position] = '\0';
			// ret[next_operator_position-prev_operator_position-1] = '\0';
		}
	}

	// printf("op_pos %d\n", operator_position);
	if(operator_position > 0){
		if (verbose == true)printf("parse_multiplication out:\t%s\n", ret);
		return operation;
	}else{
		return 0;
	}
	// return operator_position;
}
char parse_addition(char *arg, char *ret, char operation){
	if (verbose == true)printf("parse_addition in:\t\t%s\n", arg);
	unsigned int arg_len = strlen(arg);
	if (arg_len < 3) exit_function(EXIT_FAILURE);

	char num_one_str[MAX_NUMBER_LENGTH], num_two_str[MAX_NUMBER_LENGTH];
	int operator_position = 0;
	int prev_operator_position = 0;
	int next_operator_position = 0;
	for (int i = arg_len; i>=0; i--){
		if (arg[i] == ',') arg[i] = '.';
		if(is_operation(arg[i]) == '+' || is_operation(arg[i]) == '-'){
			operator_position = i;

			next_operator_position = i+1;
			if (arg[next_operator_position] != '\0'){
				// while(is_operation(arg[next_operator_position]) == false){
				while(!is_operation(arg[next_operator_position])){
						next_operator_position++;
						if (arg[next_operator_position] == '\0'){
							break;
						}
				}
				// printf("i:%d \t nop: %d\n", i, next_operator_position);
			}

			prev_operator_position = i-1;
			if (arg[prev_operator_position] != '\0'){
				while(!is_operation(arg[prev_operator_position])){
					prev_operator_position--;
					if (arg[prev_operator_position] == 0){
						prev_operator_position = 0;
						break;
					}
				}
				// printf("i:%d \t pop: %d\n", i, prev_operator_position);
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
			// num_one_str[next_operator_position-prev_operator_position] = '\0';
			// ret[next_operator_position-prev_operator_position-1] = '\0';
		}
	}

	// printf("op_pos %d\n", operator_position);
	if(operator_position > 0){
		if (verbose == true)printf("parse_addition out:\t\t%s\n", ret);
		return operation;
	}else{
		return 0;
	}
	// return operator_position;
}
int parse_parenthesis(char *arg, char *ret){
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
		// printf("par_string: %s\n", ret);
		return 1;
	}
	return 0;
}
double perform_op(operation *op){
	double res = 0;
	switch (op->symbol){
		case '+':	res = op->num_one + op->num_two;		break;
		case '-':	res = op->num_one - op->num_two;		break;
		case '*':	res = op->num_one * op->num_two;		break;
		case '/':	res = op->num_one / op->num_two;		break;
		case '^':	res = pow(op->num_one, op->num_two);break;
		default:		break;
	}
	if(verbose == true){
		printf("perform_op:\t\t\t%lf %c %lf = %lf\n", op->num_one, op->symbol, op->num_two, res);
	}
	return res;
}
void str_insert_double(char *dest, char *op_string, double res){
	if(verbose == 1)printf("str_insert_double in:\t\t%s   replace %s with %lf\n", dest, op_string, res);
	//EXAMPLE INPUT:  "1+4^2+1-2
	//EXAMPLE OUTPUT: "1+16.000000+1-2"
	char buf[100];
	char res_str[100];
	sprintf(res_str, "%lf", res);
	char *tail = strstr(dest, op_string)+strlen(op_string);
	// printf("buf %s, dest %s, op %s\n", buf, dest, op_string);
	strcpy(buf, strstr(dest, op_string)+strlen(op_string));

	strcpy(strstr(dest, op_string), res_str);
	strcat(dest, buf);
	if(verbose == 1)printf("str_insert_double out:\t\t%s\n\n", dest);

	// printf("arg: %s\n", dest);
	// printf("tail: %s\n", tail);
}
double perform_calculation(char *arg){
	operation op;
	op.symbol = ' ';
	char op_string[256];
	double res = 0;
	// printf("perform_calculation:\t\t%s\n", arg);

	while(parse_exponents(arg, op_string, '^')){
		// printf("%s\n", op_string);
		// printf("%s\n\n", arg);
		str_to_float(op_string, &op);
		res = perform_op(&op);
		str_insert_double(arg, op_string, res);
	}
	while(parse_multiplication(arg, op_string, '*')){
		// printf("op: %s\n", op_string);
		// printf("%s\n\n", arg);
		str_to_float(op_string, &op);
		res = perform_op(&op);
		str_insert_double(arg, op_string, res);
	}
	while(parse_addition(arg, op_string, '+')){
		// printf("op: %s\n", op_string);
		printf("HALLO%s\n\n", arg);
		str_to_float(op_string, &op);
		res = perform_op(&op);
		str_insert_double(arg, op_string, res);
	}
	// while(parse_addition(arg, op_string, '-')){
	// 	// printf("op: %s\n", op_string);
	// 	// printf("%s\n\n", arg);
	// 	str_to_float(op_string, &op);
	// 	res = perform_op(&op);
	// 	str_insert_double(arg, op_string, res);
	// }
	if (op.symbol == ' '){
		res = atof(arg);
	}
		// str_to_float(op_string, &op);

	return res;
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

/* Main */
int main(int argc, char **argv){
	if (argc < 3){
		fprintf(stderr, "argc < 3\n");
		exit(EXIT_FAILURE);
	}

	int opcount = 0;
	char arg[MAX_FUNCTION_LENGTH] = {0};
	strcpy(arg, argv[1]);

	char op_string[MAX_FUNCTION_LENGTH] = {0};
	operation op;
	double res;

	char variable = 0;
	int variable_count = 0;
	double from, stepsize, to;
	if (argc == 3){
		if(sscanf(argv[2], "%c=%lf:%lf:%lf", &variable, &from, &stepsize, &to) != 4){
			fprintf(stderr, "<x=start:stepsize:end>\n");
			exit_function(FORMAT_ERROR);
		}
		for(int i = 0; i<strlen(argv[1]); i++){
			if (argv[1][i] == variable){
				variable_count++;
			}
		}
	}

	int output_size = (to-from)/stepsize;
	char output_array[MAX_NO_ELEMENTS] = {0};
	char output_csv[MAX_NO_ELEMENTS] = {0};
	sprintf(output_array, "float table[%d] =\n{", output_size+1);

	double plotx[output_size];
	double ploty[output_size];
	int ct = 0;

	for (double x = from; x<=to; x = x+stepsize){
		strcpy(arg, argv[1]);
		if (verbose == 1)printf("---Calculating---\t\t%s, x=%.2lf\n", arg, x);
		// printf("%lf\n", x);

		for(int i = 0; i<variable_count; i++){
				str_insert_double(arg, "x", x);
			}

			// printf("arg_before_parenthesis: %s\n", arg);
		if(parse_parenthesis(arg, op_string)){
			char strbuf[256];
			strcpy(strbuf, op_string);
			res = perform_calculation(strbuf);
			// printf("strbuf:%s\n", strbuf);
			// printf("res: %lf\n", res);
			str_insert_double(arg, op_string, res);
		}
			// printf("arg_after_parenthesis: %s\n", arg);


		// printf("arg_to_func:%s\n", arg);
		while(res = parse_functions(arg, op_string)){
			printf("op_string: %s\n", op_string);
			// str_to_float(op_string, &op);
			// res = perform_op(&op);
			printf("sin_result:%lf\n", res);
			// str_insert_double(arg, op_string, res);
		}


		// printf("%s\n", arg);
		// printf("op_string:%s\n", op_string);

		res = perform_calculation(arg);

		if(verbose == true){
			printf("x=%.2lf -> %s = %lf\n", x,argv[1], res);
			printf("inserting: %lf at %d\n\n", res, ct);
		}
		plotx[ct]=x;
		ploty[ct]=res;
		ct++;

		char strbuf[256];
		sprintf(strbuf, "%lf", res);
		strcat(output_array, strbuf);

		for(int i = 0; i<strlen(strbuf); i++){
			if(strbuf[i] == '.') strbuf[i] = ',';
		}

		strcat(output_csv, strbuf);

		if (x < to){
			strcat(output_array, ", ");
			strcat(output_csv, ";\n");
		}
		if (ct%5 == 0){
			strcat(output_array, "\n");
		}
	}
	strcat(output_array, "};");
	// printf("\n%s\n", output_array);

	FILE *out = fopen("output.c", "w+");
	if (out == NULL){
		perror("fopen");
		exit_function(EXIT_FAILURE);
	}
	fprintf(out, "/* %s with %s */\n\n%s",argv[1], argv[2], output_array);
	fclose(out);

	FILE *out_csv = fopen("output.csv", "w+");
	if (out_csv == NULL){
		perror("fopen");
		exit_function(EXIT_FAILURE);
	}
	fprintf(out_csv, "%s", output_csv);
	fclose(out_csv);


	// for(int i = 0; i<output_size; i++){
	// 	plotx[i] = (double)i;
	// 	ploty[i] = (double)i*(double)i;
	// }

	gnuplot_ctrl *plot;
	plot = gnuplot_init();

	gnuplot_cmd(plot, "set terminal png");
	gnuplot_cmd(plot, "set output \"output.png\"");

	gnuplot_resetplot(plot);
	gnuplot_set_xlabel(plot, "x");
	gnuplot_set_ylabel(plot, argv[1]);
	// gnuplot_setstyle(plot, "linespoints");
	gnuplot_setstyle(plot, "lines");
	gnuplot_plot_xy(plot, plotx, ploty, output_size, argv[1]);
	gnuplot_close(plot);
	//100*(1+0.0039083*x-0,0000005,775*x^2)
	return 0;
}

/**
* parse_arguments() -
*
* @argc: Argument count.
* @argv: Argument vector.
*
* Returns: Number of threads to run.
*/
// int parse_arguments(int argc, char *argv[]){
// 	int opt = 0;
// 	int error = 0;
//
// 	while ((opt = getopt(argc, argv, "p:t:")) != -1) {
// 		switch (opt) {
// 			case 'p':
// 				break;
// 			case 't':
// 				break;
// 			default:
// 				error = 1;
// 		}
// 	}
//
// 	if (error == 1){
// 		fprintf(stderr, "Usage: %s\n", argv[0]);
// 		exit(EXIT_FAILURE);
// 	}
// 	return error;
// }


/* exit_function() - Cleanup and exit function.
* @status: Exit status.
* Returns: Nothing */
void exit_function(int status){
	if (status == FORMAT_ERROR){
		fprintf(stderr, "usage: clutmaker <function> <x=start:stepsize:end>\n");
	}else{
		fprintf(stderr, "%s\n", strerror(status));
	}
	// fprintf(stderr, "exiting %d\n", status);
	exit(status);
}
