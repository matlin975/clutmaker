#ifndef _CLUTMAKER_
#define _CLUTMAKER_
/*
 * ============================== clutmaker ===================================
 * main.h
 *
 * Author: Mattias Lindström
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

// #include "command_parser.h"

/* Constants */
#define FORMAT_ERROR 10
#define MAX_INPUT_FUNCTION_LENGTH 32
#define MAX_NO_ELEMENTS 1024*64
#define PI 3.14159265

/* Typedef */
// typedef struct operation;
typedef struct options_t {
    int verbose;
    int plot;
}options_t;

typedef struct math_operation_t{
	double num_one;
	double num_two;
	char symbol;
}math_operation_t;

/* Global variables */
bool verbose;           //if true, prints function input and outputs to stdout

/* Function Prototypes */
double perform_op(math_operation_t *op);
void str_insert_double(char *dest, char *op_string, double res);
double perform_calculation(char *arg);
void create_png_plot(double *plotx, double *ploty, int output_size, char *ylabel, char *file_name);



/* print_usage() - Prints usage instructions.
* @progname: Program name.
* @opt: Program option.
* Returns: Nothing */
void print_usage(char *progname);


/* exit_function() - Cleanup and exit function.
* @status: Exit status.
* Returns: Nothing */
void exit_function(int status);

#endif /* _CLUTMAKER_ */
