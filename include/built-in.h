#ifndef BUILT_IN_H
#define BUILT_IN_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/**
 * Sets new current/working directory
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return 0 on success and -1 on error
 */
int cd(char** argv, int argc);

/**
 * Print name of current/working directory
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return 0 on success and -1 on error
 */
int pwd(char** argv, int argc);

/**
 * Write arguments to the standard output
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return 0 on success and -1 on error
 */
int echo(char** argv, int argc);

/**
 * Cause normal process termination
 * @param argv array of arguments
 * @param argc arguments count
 * 
 * @return 0 on success and -1 on error
 */
int builtin_exit(char** argv, int argc);

#endif