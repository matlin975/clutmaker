#ifndef _CLUTMAKER_
#define _CLUTMAKER_
/*
 * ============================== clutmaker ===================================
 * main.c
 *
 * Author: Mattias Lindström
 * Mail: hed17mlm@cs.umu.se
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>

/* Constans */

/* Typedef */

/* Global variables */

/* Function Prototypes */

/**
* parse_arguments() -
*
* @argc: Argument count.
* @argv: Argument vector.
*
* Returns: Number of threads to run.
*/
// int parse_arguments(int argc, char *argv[]);
/*
* exit_function() - Cleanup and exit function.
*
* @status: Exit status.
*
* Returns: Nothing
*/
void exit_function(int status);

#endif
