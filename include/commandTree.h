#ifndef COMMANDTREE_H
#define COMMANDTREE_H

#include "typedef.h"

/**
 * Declare and allocate a new node
 * @param value the String value of the node
 * @return the node created
 */
commandNode* newNode(char* value);

/**
 * Insert a node to the left
 * @param root the node target by the attach
 * @param nodeToInsert the node to insert
 * @return the node attached
 */
commandNode* insertLeft(commandNode* root, commandNode* nodeToInsert);

/**
 * Insert a node to the right
 * @param root the node target by the attach
 * @param nodeToInsert the node to insert
 * @return the node attached
 */
commandNode* insertRight(commandNode* root, commandNode* nodeToInsert);

/**
 * Print the entire tree
 * @param root the root of the tree to print
 */
void print_tree(commandNode* root);

#endif
