#ifndef UI_H
#define UI_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"
#include "var.h"

/**
 * Prints the shell prompt
 * 
 * @return true
 */
int print_prompt();

/**
 * Clean result of fgets, remove "\n" and flush stdin
 * @param buffer string to clean
 */ 
void clean(const char*);

/**
 * Reads a command line from the shell prompt
 * @param buffer to put the line into
 */
void read_command_line(char* buffer);

/**
 * Make the shell wait for specified character sequence
 * 
 * @param delim char sequence waited
 */
int wait_for_delim(char* delim);

#endif