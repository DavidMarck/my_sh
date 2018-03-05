#include "typedef.h"
#include <stdlib.h>


commandNode* newNode(char* value)
{
	commandNode* cmdNode;
	cmdNode = malloc(sizeof(struct commandNode));
	cmdNode->value = value;
	cmdNode->left = NULL;
	cmdNode->right = NULL;
	
	return cmdNode;
}

commandNode* insertLeft(commandNode* root, commandNode* nodeToInsert) {
	if(root == NULL)
	{
		return nodeToInsert;
	}
	
	if(root->left != NULL)
	{
		root->left = insertLeft(root->left, nodeToInsert);
	}
	else
	{
		root->left = nodeToInsert;
	}
	
	return root;
}

commandNode* insertRight(commandNode* root, commandNode* nodeToInsert) {
	if(root == NULL)
	{
		return nodeToInsert;
	}
	
	if(root->right != NULL)
	{
		root->right = insertLeft(root->right, nodeToInsert);
	}
	else
	{
		root->right = nodeToInsert;
	}
	return root;
}

void print_tree(commandNode* root)
{
      if (root == NULL) return; 

      print_tree(root->right);
      printf("%s\n", root->value);
      print_tree(root->left);
}
