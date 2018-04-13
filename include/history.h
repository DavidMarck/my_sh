#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Gets the history file path, usually under /home/user/...
 * (for this project we chose : /home/user/tmp/my_sh/commands_history.txt)
 * 
 * @return command history file path
 */
char* get_history_path();

/**
 * Writes a command line to the history file
 * @param commandLine command line to write into the history
 * 
 * @return -1 on error, 0 on success
 */
int write_to_history(char* commandLine);

/**
 * Clears commands history file
 */
void clear_history();

#endif