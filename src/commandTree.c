#include "commandTree.h"
#include "string.h"

commandNode* new_node(char* value)
{
	commandNode* cmdNode;
	cmdNode = malloc(sizeof(struct commandNode));
	cmdNode->value = realloc(cmdNode->value, sizeof(char) * (strlen(value)+1));
	cmdNode->mainRoot = cmdNode;
	strcpy(cmdNode->value, value);
	cmdNode->left = NULL;
	cmdNode->right = NULL;
	return cmdNode;
}

commandNode* insert_left(commandNode* root, commandNode* nodeToInsert) {
	if(root == NULL)
	{
		return nodeToInsert;
	}
	nodeToInsert->mainRoot = root->mainRoot;
	
	if(root->left != NULL)
	{
		root->left = insert_left(root->left, nodeToInsert);
	}
	else
	{
		root->left = nodeToInsert;
	}
	return nodeToInsert;
}

commandNode* insert_right(commandNode* root, commandNode* nodeToInsert) {
	
	if(root == NULL)
	{
		return nodeToInsert;
	}
	nodeToInsert->mainRoot = root->mainRoot;
	if(root->right != NULL)
	{
		root->right = insert_right(root->right, nodeToInsert);
	}
	else
	{
		root->right = nodeToInsert;
	}
	return nodeToInsert;
}



void print_tree(commandNode* root)
{
    if (root == NULL) return; 
	
	printf("tree : %s\n", root->value);
    print_tree(root->left);
    print_tree(root->right);
      
}

void free_tree(commandNode* root){          
	if( root ){
     freeTree(root->left);
     freeTree(root->right);
     free(root->value); 
     free(root);
   }

}

