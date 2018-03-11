#ifndef COMMANDS_H
#define COMMANDS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "var.h"
#include "typedef.h"

#include "util.h"
#include "built-in.h"

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
void execute_command(char** argv, int argc);

/**
 * Execute a buitl-in command using argv and argc
 * @param argv array of arguments
 * @param argc arguments count
 */
int execute_builtin(char** args, int argc);

/**
 * Clean result of fgets, remove "\n" and flush stdin
 * @param buffer string to clean
 */ 
void clean(const char*);

/**
 * Indicates wether or not the command is a built-in command
 * @param command
 */
int isbuiltin(char* command);

#endif