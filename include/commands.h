#ifndef COMMANDS_H
#define COMMANDS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "var.h"
#include "typedef.h"

#include "util.h"
#include "built-in.h"

/**
 * Parse the text of the commandLine
 * @param command command line to parse
 * @param argc argument count
 */ 
char** parse_to_argv(char*, int*);

/**
 * Execute a command using argv and argc
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return the exit code of process
 */
int execute_command(char** argv, int argc);

/**
 * Execute a buitl-in command using argv and argc
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return 0 on success, -1 on failure
 */
int execute_builtin(char** args, int argc);

/**
 * Indicates wether or not the command is a built-in command
 * @param commandLine
 * 
 * @return true or false
 */
int isbuiltin(char* commandLine);

/**
 * Indicates wether or not a command line (parsed into argv) 
 * includes command(s) that have to be ran as background processes.
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return true if argv contains at least one & operator and false otherwise
 */
//int includes_background(char** argv, int argc);

/**
 * Gets the next sequence of commands that has to be ran as a background process
 * @param argv array of arguments
 * @param argc arguments count (input arguments array)
 * @param bg_argc arguments count (output arguments array)
 * @param nxtCmdLineIndex index of the next command line (after & operator)
 * 
 * @return array of arguments to be used in a background process
 */
//char** get_bg_command_args(char** argv, int argc, int* bg_argc, int* nxtCmdLineIndex);

/**
 * Gets the next sequence of commands that has to be ran as a foreground process
 * @param argv array of arguments
 * @param argc arguments count (input arguments array)
 * @param fg_argc arguments count (output arguments array)
 * @param startIndex 
 * 
 * @return sub-array of arguments to be used in a foreground process
 */
//char** get_fg_command_args(char** argv, int argc, int* fg_argc, int startIndex);

#endif
