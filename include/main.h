#ifndef MAIN_H
#define MAIN_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>

#include "commands.h"
#include "util.h"
#include "commandTree.h"
#include "ui.h"

#define USAGE_SYNTAX "[OPTIONS]"
#define USAGE_PARAMS "OPTIONS:\n\
  -c, --command  COMMAND  : command line\n\
  -h, --help    : display this help\n\
"

/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * @see man 3 getopt_long or getopt
 * @see struct option definition
 */
static struct option binary_opts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "command",   required_argument, 0, 'c' },
  { 0,         0,                 0,  0  } 
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * @see man 3 getopt_long or getopt
 */ 
const char* binary_optstr = "hc:";

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * @return void
 */
void print_usage(char* bin_name);

/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * @param void* to_free pointer to an allocated mem
 * @see man 3 free
 * @return void
 */
void free_if_needed(void* to_free);

/**
 *
 * @see man 3 strndup
 * @see man 3 perror
 * @return
 */
char* dup_optarg_str();

/**
 * Exits the program
 */
void exitProg(char* bin_command_param, int code);

#endif
