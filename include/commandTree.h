#ifndef COMMANDTREE_H
#define COMMANDTREE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
       
#include "string.h"

#include "var.h"
#include "typedef.h"
#include "util.h"
#include "commands.h"

/**
 * Declare and allocate a new node
 * @param value the String value of the node
 * 
 * @return the node created
 */
commandNode* new_node(char* value);

/**
 * Insert a node to the left
 * @param root the node target by the attach
 * @param nodeToInsert the node to insert
 * 
 * @return the node attached
 */
commandNode* insert_left(commandNode* root, commandNode* nodeToInsert);

/**
 * Insert a node to the right
 * @param root the node target by the attach
 * @param nodeToInsert the node to insert
 * 
 * @return the node attached
 */
commandNode* insert_right(commandNode* root, commandNode* nodeToInsert);

/**
 * Parse an array of Strings into a tree
 * @param arguments the array of Strings
 * @param args_count the number of arguments
 * @return the parsed string in tree format
 */
commandNode* parse_to_tree(char** arguments, int args_count);

/**
 * Check if a string is a special argument
 * @param argument the string to check
 * @return true/false depending if this is a special argument
 */
int is_special_string(char* argument);

/**
 * Check if a string is a && or ||
 * @param argument the string to check
 * @return true/false depending if this is a && or ||
 */
int is_operator(char* argument);

/**
 * Check if a string is a |
 * @param argument the string to check
 * @return true/false depending if this is a |
 */
int is_pipe(char* argument);

/**
 * Check if a string is a special argument without fork
 * @param argument the string to check
 * @return true/false 
 */
int is_redirection_without_fork(char* argument); 

/**
 * Execute a command line based on a tree
 * @param root the tree to execute
 * @param isBackground wether or not the process has to be ran in background (O or 1)
 */
void execute_tree(commandNode* root, int isBackground);

/**
 * Intertpret a Node to execute in a tree (recursive call)
 * @param node the node to interpret
 */
void interpret_node(commandNode* node);

/**
 * Execute a node which include a fork
 * @param node the node to execute
 */
void execute_fork_node(commandNode* node);

/**
 * Execute a node with a redirection which don't requires a fork
 * @param node the node to execute
 * @return the return code
 */
int execute_redirection_without_fork(commandNode* node);

/**
 * Print the entire tree
 * @param root the root of the tree to print
 */
void print_tree(commandNode* root);

/**
 * Free the memory allocation of a tree
 * @param root the root of the tree to free
 */
void free_tree(commandNode* root);

#endif
