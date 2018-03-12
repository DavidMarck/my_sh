#include "commandTree.h"

commandNode* new_node(char* value)
{
	commandNode* cmdNode;
	cmdNode = malloc(sizeof(commandNode));
	//printf("Création du noeud %s\n", value);
	cmdNode->value = malloc(sizeof(char) * (strlen(value)+1));
	//printf("Valeur du pointeur : %p\n", cmdNode->value);
	cmdNode->mainRoot = cmdNode;
	strcpy(cmdNode->value, value);
	cmdNode->left = NULL;
	cmdNode->right = NULL;
	//printf("Création effectuée \n");
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
	char* command = malloc(sizeof(char));
	
	
	commandNode* cmdNode = NULL;
	commandNode* operatorNode = NULL;
	commandNode* redirectionNode = NULL; 
	
	while (index > -1)
	{
		//printf("Noeud actuel : %s\n", arguments[index]);
		if(is_special_string(arguments[index]) == TRUE || index == 0) 
		{
			if(index != 0) 
				string_index = index+1;
			else
				string_index = index;
			command = strcpy(command,"");
			
			for(i = string_index; i < last_string_limit; i++)
			{
				//printf("taille de la commande avant : %d\n", (strlen(command)+1));
				//printf("Espace qui va être ajouté : %d\n", ((strlen(arguments[i]) + 1) * sizeof(char)) + 1);
				int spaceNeeded = ((strlen(command)) * sizeof(char)) + ((strlen(arguments[i]) + 1) * sizeof(char));
				// if we're don't at the last string, we had 1 space for the whitespace
				if(i != string_index)
					spaceNeeded++;
					
				command = realloc(command,spaceNeeded);
			
				if(i != string_index)
					command = strcat(command, " ");
					
				command = strcat(command, arguments[i]);
				//printf("Espace actuel de la commande après concatenation : %d\n", spaceNeeded);
				//printf("Value : %s\n", command);
			}
			
			//printf("Fin de la concaténation\n");
			if(index == 0)
			{
				cmdNode = new_node(command);
				if(redirectionNode != NULL)
				{	
					add_left(redirectionNode, cmdNode);
					add_left(operatorNode, redirectionNode);
				}
				else
					cmdNode = add_left(operatorNode, cmdNode);
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
			
			last_string_limit = index;
		} 
		index--;
	}
	//printf("Fin de l'engraissage, dernière commande : %s\n", command);
	free(command);
	//printf("Free effectué\n");
	return cmdNode->mainRoot;
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

void execute_tree(commandNode* root)
{
  int status;
  int pid;
  if((pid = fork()) < 0)
  {
	  perror("Error");
	  exit(EXIT_FAILURE);
  }
  
  
  if(pid == 0) 
  {
	  interpret_node(root);
  }
  else
  {
	  wait(&status);
  }
	
}

void interpret_node(commandNode* node)
{
	if(is_special_string(node->value) == FALSE)
	{
		int args_count = 0;
		char** commandArgs = parse_command(node->value,&args_count);
		execute_command(commandArgs, args_count);
	}
	
	else if(is_fork(node->value) == TRUE) 
	{
		execute_fork_node(node);
	}
	
	else if(is_redirection_without_fork(node->value) == TRUE)
	{
		execute_redirection_without_fork(node);
	}
}

void execute_fork_node(commandNode* node)
{
	int status;
	int pid;
	
	// If we have a pipe
	if(strcmp(node->value, "|") == 0)
	{
		int pipeDescs[2];         
		if(pipe(pipeDescs) == -1)
		{
		   perror("Pipe error");
		   exit(EXIT_FAILURE);
		}
		
		if((pid = fork()) < 0)
		{
			perror("Fork error");
			exit(EXIT_FAILURE);
		}
		
		if (pid ==0) 
		{			 
			close(pipeDescs[0]);  	
			dup2(pipeDescs[1], STDOUT);  
			
			interpret_node(node->left);
		}
		else 
		{
			close(pipeDescs[1]); // On ferme l'entrée du pipe
			dup2(pipeDescs[0], STDIN); // On redirige l'entrée standarde sur la sortie du pipe
				
			interpret_node(node->right);
	  }

		
		
		
	}
	
	
	else
	{
		if((pid = fork()) < 0)
		{
			perror("Fork error");
			exit(EXIT_FAILURE);
		}
		
		if(pid == 0)
		{
			interpret_node(node->left);
		}
		else 
		{
			wait(&status);
			if(strcmp(node->value, "&&") == 0)
			{
				if(WIFEXITED(status) && WEXITSTATUS(status) == 0) 
				{		
					interpret_node(node->right);
					exit(EXIT_SUCCESS);
				}
				
			}
			else
			{
				if(WIFEXITED(status) && WEXITSTATUS(status) != 0) 
				{		
					interpret_node(node->right);
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
}

void execute_redirection_without_fork(commandNode* node)
{
	if(strcmp(node->value, ">") == 0)
	{
		int fileDescriptor;
		if ((fileDescriptor = open(node->right->value, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("File error");
			exit(EXIT_FAILURE);
		}
		dup2(fileDescriptor, STDOUT);
	}
	interpret_node(node->left);
}

void print_tree(commandNode* root)
{
	//printf("Affichage\n");
    if (root == NULL) return; 
	
	printf("tree : %s\n", root->value);
    print_tree(root->left);
    print_tree(root->right);
      
}

void free_tree(commandNode* root){         
	if( root ){
     free_tree(root->left);
     free_tree(root->right);
     //printf("Nettoyage de l'arbre : %s\n", root->value);
     free(root->value); 
     free(root);
     //printf("Fin du nettoyage de l'arbre\n");
   }
}



