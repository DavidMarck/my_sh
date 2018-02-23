#ifndef COMMANDS_H
#define COMMANDS_H

/**
 * Prints the command lines starter
 */
void print_prompt();

/**
 * Parse the text of the commandLine
 */ 
void parseCommand();

/**
 * Clean result of fgets, remove "\n" and flush stdin
 * @param buffer string to clean
 */ 
void clean(const char*);

/**
 * Reads a string as a command
 * @param command string to be read as a command
 */
void readCommand(char*);

#endif