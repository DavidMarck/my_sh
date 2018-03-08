#include "commandTree.h"

commandNode* new_node(char* value)
{
	commandNode* cmdNode;
	cmdNode = malloc(sizeof(commandNode));
	cmdNode->value = realloc(cmdNode->value, sizeof(char) * (strlen(value)+1));
	cmdNode->mainRoot = cmdNode;
	strcpy(cmdNode->value, value);
	cmdNode->left = NULL;
	cmdNode->right = NULL;
	
	return cmdNode;
}

commandNode* add_left(commandNode* root, commandNode* nodeToInsert) {
	if(root == NULL)
	{
		return nodeToInsert;
	}
	nodeToInsert->mainRoot = root->mainRoot;
	
	if(root->left != NULL)
	{
		root->left = add_left(root->left, nodeToInsert);
	}
	else
	{
		root->left = nodeToInsert;
	}
	return nodeToInsert;
}

commandNode* add_right(commandNode* root, commandNode* nodeToInsert) {
	
	if(root == NULL)
	{
		return nodeToInsert;
	}
	nodeToInsert->mainRoot = root->mainRoot;
	if(root->right != NULL)
	{
		root->right = add_right(root->right, nodeToInsert);
	}
	else
	{
		root->right = nodeToInsert;
	}
	return nodeToInsert;
}


commandNode* parse_to_tree(char** arguments, int args_count)   // pas oublier le "&" !
{
	int index = args_count-1;
	int last_string_limit = args_count;
	int string_index;
	int i;
	
	
	commandNode* cmdNode = NULL;
	commandNode* operatorNode = NULL;
	commandNode* redirectionNode = NULL; 
	
	while (index > -1)
	{
		if(is_special_string(arguments[index]) == TRUE || index == 0) 
		{
			if(index != 0) 
				string_index = index+1;
			else
				string_index = index;
			
			
			char* command = malloc(sizeof(char));
			//command = "\0";
			printf("Malloc : valeur du malloc = %s\n", command);
			for(i = string_index; i < last_string_limit; i++)
			{
				printf("Value : %s\n", command);
				int spaceNeeded = (strlen(command) * sizeof(char)) + ((strlen(arguments[i]) + 1) * sizeof(char)) + 1;
				realloc(command,spaceNeeded);
				if(i != string_index)
					strcat(command, " ");
					
				strcat(command, arguments[i]);
				
			}
			
			/*
			if(index == 0)
			{
				cmdNode = add_left(operatorNode, new_node(command));
			}
			else if(is_fork(arguments[index]) == TRUE)
			{
				operatorNode = add_left(operatorNode, new_node(arguments[index]));
				
				if(redirectionNode != NULL) 
				{
					add_left(redirectionNode, new_node(command));
					add_right(operatorNode, redirectionNode);
					redirectionNode = NULL;
				}
				else
					add_right(operatorNode, new_node(command));
			}
			
			else if(is_redirection_without_fork(arguments[index]) == TRUE)
			{
				redirectionNode = new_node(arguments[index]);
				add_right(redirectionNode, new_node(command));
			}
			*/
			//free(command);
			last_string_limit = index;
		}
		index--;
	}
	//return cmdNode->mainRoot;
}

int is_special_string(char* argument)
{	
	int index = 0;
	char* operators[] = {"|", "||", "&&", ">", "<", ">>", "<<", "&"};
	
	
	while (index < 8)
	{
		if(strcmp(operators[index], argument) == 0)
		{
			return TRUE;
		}
		index++;
	}
	return FALSE;
}

int is_fork(char* argument) 
{
	int index = 0;
	char* operators[] = {"|", "||", "&&"};
	
	while (index < 3)
	{
		if(strcmp(operators[index], argument) == 0)
		{
			return TRUE;
		}
		index++;
	}
	return FALSE;
}

int is_redirection_without_fork(char* argument) 
{
	int index = 0;
	char* operators[] = {">", "<", ">>", "<<"};
	
	while (index < 4)
	{

		if(strcmp(operators[index], argument) == 0)
		{
			return TRUE;
		}
		index++;
	}
	return FALSE;
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
     free_tree(root->left);
     free_tree(root->right);
     free(root->value); 
     free(root);
   }
}



