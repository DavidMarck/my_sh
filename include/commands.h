#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedef.h"

#include "util.h"

/**
 * Prints the command lines starter
 */
void print_prompt();

/**
 * Parse the text of the commandLine
 * @param command command line to parse
 * @param argc argument count
 */ 
char** parse_command(char*, int*);

/**
 * Execute a command using argv and argc
 * @param argv array of arguments
 * @param argc arguments count
 */
int execute_command(char**, int);

/**
 * Clean result of fgets, remove "\n" and flush stdin
 * @param buffer string to clean
 */ 
void clean(const char*);

/**
 * Reads a string as a command
 * @param command string to be read as a command
 */
void read_command(char*);

#endif