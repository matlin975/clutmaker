/* ============================== clutmaker ===================================
 * main.c
 *
 * Author: Mattias Lindström
 * Mail: hed17mlm@cs.umu.se
 *
 * TODO:
 * log function calls to txt file?
 * x-x : bugged
 * plot.png bugged
 * Support for functions: sin, cos, log etc...
 *
 * End goal to handle this:
 * = 100 * ( 1 + (0,003908*10) + (-5,78E-07*10^2) + (-4,2E-12 * (10-100) * 10^3))
*/

#include "main.h"
#include "command_parser.h"
#include "gnuplot_i.h"
#define DEFAULT_PROG_NAME "clutmaker"
#define OPTIONS_STRING "vp"
#define USAGE_FMT "usage: %s [-v verbose] [-p plot] <function> <range>\n"
#define USAGE_EXAMPLE "example: ./clutmaker x^2+x*4+1.013 x=1:0.1:10\n"
#define FORMAT_ERROR 10
// #define MAX_NUMBER_LENGTH 64
// #define MAX_INPUT_FUNCTION_LENGTH 64
#define MAX_OUTPUT_SIZE 1024*64
#define PI 3.14159265

bool verbose = 0;

/* Main */
int main(int argc, char **argv){
	printf("argc: %d\n", argc);
	printf("argv: %s\n", argv[argc-1]);
	if (argc < 3){
		fprintf(stderr, "error: argc < 3\n");
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

    options_t options = {0, 0};
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS_STRING)) != EOF)
        switch(opt){
            case 'p':
				options.plot = 1;
                // printf("i\n");
                break;
            case 'v':
				verbose = 1;
				options.verbose = 1;
                // printf("v\n");
                break;
            default:
                print_usage(argv[0]);
                break;
        }
	printf("options: plot = %d, verbose = %d\n", options.plot, options.verbose);

	int opcount = 0;
	char arg[MAX_INPUT_FUNCTION_LENGTH] = {0};

	if (strlen(argv[argc-2]) < MAX_INPUT_FUNCTION_LENGTH){
		strcpy(arg, argv[argc-2]);
	}else{
		fprintf(stderr, "error: function length exceeds limit of: %d\n", MAX_INPUT_FUNCTION_LENGTH);
		exit(EXIT_FAILURE);
	}

	char op_string[MAX_INPUT_FUNCTION_LENGTH] = {0};
	math_operation_t op;
	double res;

	char variable = 0;
	int variable_count = 0;
	double from, stepsize, to;
	// if (argc == 3){
		if(sscanf(argv[argc-1], "%c=%lf:%lf:%lf", &variable, &from, &stepsize, &to) != 4){
			fprintf(stderr, "<x=start:stepsize:end>\n");
			print_usage(argv[0]);
			exit_function(FORMAT_ERROR);
		}
		for(int i = 0; i<strlen(argv[argc-2]); i++){
			if (argv[argc-2][i] == variable){
				variable_count++;
			}
		}
	// }

	int output_size = (to-from)/stepsize;
	printf("output_size: %d\n", output_size);
	char output_array[MAX_OUTPUT_SIZE] = {0};
	char output_csv[MAX_OUTPUT_SIZE] = {0};
	sprintf(output_array, "float table[%d] =\n{", output_size+1);

	double plotx[output_size];
	double ploty[output_size];
	int ct = 0;

	for (double x = from; x<=to; x = x+stepsize){
		strcpy(arg, argv[argc-2]);
		if (verbose == 1)printf("---Calculating---\t\t%s, x=%.2lf\n", arg, x);

		for(int i = 0; i<variable_count; i++){
			str_insert_double(arg, "x", x);
		}

		if(parse_parenthesis(arg, op_string)){
			char strbuf[256];
			strcpy(strbuf, op_string);
			res = perform_calculation(strbuf);
			str_insert_double(arg, op_string, res);
		}

		while(res = parse_functions(arg, op_string)){
			printf("op_string: %s\n", op_string);
			printf("sin_result:%lf\n", res);
		}

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

	create_png_plot(plotx, ploty, output_size, argv[1], "output.png");
	// 100*(1+0.0039083*x-0,0000005,775*x^2)
	return 0;
}

double perform_calculation(char *arg){
	math_operation_t op;
	op.symbol = ' ';
	char op_string[256];
	double res = 0;
	while(parse_exponents(arg, op_string, '^')){
		str_to_float(op_string, &op);
		res = perform_op(&op);
		str_insert_double(arg, op_string, res);
	}
	while(parse_multiplication(arg, op_string, '*')){
		str_to_float(op_string, &op);
		res = perform_op(&op);
		str_insert_double(arg, op_string, res);
	}
	while(parse_addition(arg, op_string, '+')){
		str_to_float(op_string, &op);
		res = perform_op(&op);
		str_insert_double(arg, op_string, res);
	}
	if (op.symbol == ' '){
		res = atof(arg);
	}
	return res;
}


void str_insert_double(char *dest, char *op_string, double res){
	//EXAMPLE INPUT:  "1+4^2+1-2"
	//EXAMPLE OUTPUT: "1+16.000000+1-2"
	if(verbose == 1)printf("str_insert_double in:\t\t%s   replace %s with %lf\n", dest, op_string, res);
	char buf[100];
	char res_str[100];
	sprintf(res_str, "%lf", res);
	char *tail = strstr(dest, op_string)+strlen(op_string);
	strcpy(buf, strstr(dest, op_string)+strlen(op_string));

	strcpy(strstr(dest, op_string), res_str);
	strcat(dest, buf);
	if(verbose == 1)printf("str_insert_double out:\t\t%s\n\n", dest);
}


double perform_op(math_operation_t *op){
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


/* create_png_plot() - Prints usage instructions.
* @plotx: 			Double array containing x-values to plot.
* @ploty: 			Double array containing y-values to plot.
* @output_size: 	Number of plot-elements (x-y pairs)
* @ylabel:			Label for y-axis.
* @file_name:		Filename-string for png output file.
* Returns: 			Nothing */
void create_png_plot(double *plotx, double *ploty, int output_size, char *ylabel, char *file_name){
	gnuplot_ctrl *plot;
	plot = gnuplot_init();
	gnuplot_cmd(plot, "set terminal png");

	if (strlen(file_name) > 128){
		fprintf(stderr, "filename %s exceeds length of 128 characters.", file_name);
		exit(EXIT_FAILURE);
	}
	char filename_buf[128];
	sprintf(filename_buf, "set output \"%s\"", file_name);
	gnuplot_cmd(plot, filename_buf);

	gnuplot_resetplot(plot);
	gnuplot_set_xlabel(plot, "x");
	gnuplot_set_ylabel(plot, ylabel);
	// gnuplot_setstyle(plot, "linespoints");
	gnuplot_setstyle(plot, "lines");
	gnuplot_plot_xy(plot, plotx, ploty, output_size, ylabel);
	gnuplot_close(plot);
}


/* print_usage() - Prints usage instructions.
* @progname:		Program name.
* @opt: 			Program option.
* Returns: 			Nothing */
void print_usage(char *progname){
    fprintf(stderr, USAGE_FMT, progname?progname:DEFAULT_PROG_NAME);
    fprintf(stderr, USAGE_EXAMPLE);
    exit(EXIT_FAILURE);
}


/* exit_function() - Cleanup and exit function.
* @status: 			Exit status.
* Returns: 			Nothing */
void exit_function(int status){
	if (status == FORMAT_ERROR){
		fprintf(stderr, "usage: clutmaker <function> <x=start:stepsize:end>\n");
	}else{
		fprintf(stderr, "%s\n", strerror(status));
	}
	exit(status);
}
