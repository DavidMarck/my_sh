#ifndef UI_H
#define UI_H

/**
 * Prints the shell prompt
 */
void print_prompt();

/**
 * Reads a command line
 */
void read_command_line();

/**
 * Make the shell wait for closing quote
 */
int wait_for_delim(char* delim);

#endif