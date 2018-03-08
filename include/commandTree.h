#ifndef COMMANDTREE_H
#define COMMANDTREE_H

#include <stdlib.h>
#include <stdio.h>

#include "typedef.h"
#include "string.h"

/**
 * Declare and allocate a new node
 * @param value the String value of the node
 * @return the node created
 */
commandNode* new_node(char* value);

/**
 * Insert a node to the left
 * @param root the node target by the attach
 * @param nodeToInsert the node to insert
 * @return the node attached
 */
commandNode* insert_left(commandNode* root, commandNode* nodeToInsert);

/**
 * Insert a node to the right
 * @param root the node target by the attach
 * @param nodeToInsert the node to insert
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
 * Check if a string is an operator
 * @param argument the string to check
 * @return true/false(0/1) depending if this is an operator
 */
int is_operator(char* argument);

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
