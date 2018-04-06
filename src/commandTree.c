#include "commandTree.h"

commandNode* new_node(char* value)
{
	commandNode* cmdNode;
	cmdNode = malloc(sizeof(commandNode));
	
	cmdNode->value = malloc(sizeof(char) * (strlen(value)+1));
	cmdNode->mainRoot = cmdNode;
	cmdNode->parentNode = NULL;
	
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
	nodeToInsert->parentNode = root;
	
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
	nodeToInsert->parentNode = root;
	
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

commandNode* get_last_left_child(commandNode* node)
{
	while(node->left != NULL)
	{
		node = node->left;
	}
	return node;
}

int is_left_child(commandNode* node)
{
	if(node == node->parentNode->left)
		return TRUE;
	return FALSE;
}

int is_right_child(commandNode* node)
{
	if(node == node->parentNode->right)
		return TRUE;
	return FALSE;
}


commandNode* parse_to_tree(char** arguments, int args_count)   // pas oublier le "&" !
{
	// initialisation of the cursors (we'll start from the last index to the first(0))
	int index = args_count-1;                          // current index
	int last_string_limit = args_count;                // this cursor go to the last "special index" (it concerned all operators and redirection), by default this is the "null"
	int string_index;                                  // this cursor will be use in order to concatenate some strings
	int i;
	
	// this variable will concatenate all strings between 2 specials indexes
	char* command = malloc(sizeof(char));
	
	// these variables concerned the tree building
	commandNode* cmdNode = NULL;
	commandNode* operatorNode = NULL;
	commandNode* redirectionNode = NULL; 
	
	
	while (index > -1)
	{
		// when we have a special string (or the begin of the commmand), we concatenate all strings until we get the "special" index (or the end of the command)
		//printf("Noeud actuel : %s\n", arguments[index]);
		if(is_special_string(arguments[index]) == TRUE || index == 0) 
		{
			// if we are not at the first element, we don't take the "special" string into the concatenation
			if(index != 0) 
				string_index = index+1;
			else
				string_index = index;
			command = strcpy(command,"");
			
			// these loop will concatenate all string until the "special" index 
			for(i = string_index; i < last_string_limit; i++)
			{
				// we realloc the space and we make the concatenation
				int spaceNeeded = ((strlen(command)) * sizeof(char)) + ((strlen(arguments[i]) + 1) * sizeof(char));
				// if we're don't at the last string, we had 1 space for the whitespace
				if(i != string_index)
					spaceNeeded++;
					
				command = realloc(command,spaceNeeded);
			
				if(i != string_index)
					command = strcat(command, " ");
					
				
				command = strcat(command, arguments[i]);
				//printf("Commande détectée : %s\n", command);
			}
			
			//printf("Fin de la concaténation\n");
			// if we reach the begin of the function...
			if(index == 0)
			{
				// ... we create a new "cmdNode" which contains the value of the last concatenate string
				cmdNode = new_node(command);
				
				// ... if there is a redirection Node without fork ('>', '>>', '<', '<<'), whe add the cmdNode at the left of this one (and we had the redirection node at the left of the operator)
				if(redirectionNode != NULL)
				{	
					add_left(operatorNode, redirectionNode);
					add_left(redirectionNode, cmdNode);
				}
				// ... else we add the cmdNode at the left of the last "operatorNode" which include a fork ('&&', '||', '|'), if there is no operator, the cmd node become the root of the tree 
				else
					cmdNode = add_left(operatorNode, cmdNode);
			}
			// if our index is a future fork...
			else if(is_fork(arguments[index]) == TRUE)
			{
				// ... we initiate an operatorNode and we add it at the left of the last operatorNode (or it becomes the main root)
				//printf("Fork détecté : %s\n", arguments[index]);
				operatorNode = add_left(operatorNode, new_node(arguments[index]));
				
				// if there was a redirectionNode,  we add it to this node and we add a node which contains the last concatenante string and we turn the redirectionNode variable to null 
				if(redirectionNode != NULL) 
				{
					add_right(operatorNode, redirectionNode);
					add_left(redirectionNode, new_node(command));
					redirectionNode = NULL;
				}
				// else we just add the node with the concatenate string at the right of this node
				else
					add_right(operatorNode, new_node(command));
			}
			// if we have a redirection without fork, we declare it, and we add a node which contains the last concatenate string at its right
			else if(is_redirection_without_fork(arguments[index]) == TRUE)
			{
				//printf("Redirections détectée : %s\n", arguments[index]);
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
	//printf("Racine de l'arbre : %s\n", cmdNode->mainRoot->value);
	return cmdNode->mainRoot;
}

int is_special_string(char* argument)
{	
	int index = 0;
	char* operators[] = {"|", "||", "&&", ">", "<", ">>", "<<"};
	
	
	while (index < 7)
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

void execute_tree(commandNode* root, int isBackground)
{
	commandNode* last_left_child = get_last_left_child(root);
	interpret_node(last_left_child);
	/*int status;
	int pid;
	
	if((pid = fork()) < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}  
  
	// child
	if(pid == 0) 
	{
		// if(isBackground)
		// {
		// 	//int fd = open("/dev/null",O_WRONLY | O_CREAT, 0666);   // open the file /dev/null
        // 	//dup2(fd, STDOUT); // replace standard output with output file
		// 	setpgid(0,0);
		// }
		interpret_node(root);
	}	
	// parent process
	else
	{
		// if(isBackground)
		// {
		// 	int i = 0;
		// 	while(BG_PIDS_ARRAY[i] != 0)
		// 	{
		// 		i++;
		// 	}
		// 	BG_PIDS_ARRAY[i] = pid;

		// 	printf("[process %d started]\n", pid);

		// 	int status;
		// 	waitpid(-1, &status, WNOHANG);
		// 	printf("[process %d exited with code %d]\n",
        //     pid, WEXITSTATUS(status));                         
		// }
		// else
		// {
		// 	wait(&status);
		// }
		wait(&status);
	}	
	*/
}

void interpret_node(commandNode* node)
{
	int return_code;
	int argc = 0;
	char** argv;
	
	// If last left child is the root
	if(node->parentNode == NULL && node->left == NULL && node->right == NULL)
	{
		argv = parse_to_argv(node->value,&argc);
		execute_command(argv, argc);
		
	}
	else 
	{
		// left child
		if(is_left_child(node))
		{
			if(strcmp(node->parentNode->value, "&&") == 0)
			{
				argv = parse_to_argv(node->value,&argc);
				if(execute_command(argv, argc) >= 0)
				{
					interpret_node(node->parentNode->right);
				}
			}			
		}
		// right child
		else 
		{
			// redirection
			if(is_redirection_without_fork(node->value))
			{
				//TODO le << 
			}
			// command
			else
			{
				argv = parse_to_argv(node->value,&argc);
				execute_command(argv, argc);
			}
			
		}
		
	}
	/*
	if(node == NULL) 
		return;
	
	interpret_node(node->left);
	

	if(is_special_string(node->value) == FALSE && node == node->mainRoot)
	{
		

		int args_count = 0;
		char** commandArgs = parse_to_argv(node->value,&args_count);
		
		if(isbuiltin(commandArgs[0]))
		{
			execute_builtin(commandArgs, args_count);
		}
		else
		{
			int pid;
			int status;
			pid = fork();
			if(pid == 0)
			{
				execute_command(commandArgs, args_count);
			}
			else
			{
				wait(&status);
			}
			
		}
		
	}
	else 
	{
		if(strcmp(node->value, "&&") == 0)
		{
			int status;
			int args_count = 0;
			char** commandArgs = parse_to_argv(node->left->value,&args_count);
			
			if(isbuiltin(commandArgs[0]))
			{
				execute_builtin(commandArgs, args_count);
			}
			else
			{
				int status;
				int pid;
				
				pid = fork();
				if(pid == 0)
				{
					execute_command(commandArgs, args_count);
				}
				else
				{
					wait(&status);
					
				}
				
			}
			
		}
	}
	*/	
	
	/*
	printf("################ Noeud %s ###########\n", node->value);
	if(node->parentNode != NULL)
		printf(".  parent : %s\n", node->parentNode->value);
	if(node->left != NULL)
		printf(".  fils de gauche : %s\n", node->left->value);
			
	if(node->right != NULL)
		printf(".  fils de droite : %s\n", node->right->value);
	
	*/	
	
	
	//interpret_node(node->right);
	/*
	if(is_special_string(node->value) == FALSE)
	{
		int args_count = 0;
		char** commandArgs = parse_to_argv(node->value,&args_count);
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
	* */
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
		   perror("pipe");
		   exit(EXIT_FAILURE);
		}
		
		if((pid = fork()) < 0)
		{
			perror("fork");
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
			perror("fork");
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
				} 
				exit(EXIT_FAILURE);
			}
			else
			{
				if(WIFEXITED(status) && WEXITSTATUS(status) != 0) 
				{		
					interpret_node(node->right);
				}
				exit(EXIT_FAILURE);
			}
		}
	}
}

void execute_redirection_without_fork(commandNode* node)
{
	if(strcmp(node->value, ">") == 0)
	{
		int fileDescriptor;
		if ((fileDescriptor = open(node->right->value, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("File error");
			exit(EXIT_FAILURE);
		}
		dup2(fileDescriptor, STDOUT);
	}
	
	if(strcmp(node->value, ">>") == 0)
	{
		int fileDescriptor;
		if ((fileDescriptor = open(node->right->value, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR)) == -1)
		{
			perror("File error");
			exit(EXIT_FAILURE);
		}
		dup2(fileDescriptor, STDOUT);
	}
	
	if(strcmp(node->value, "<") == 0)
	{
		int fileDescriptor;
		
		if ((fileDescriptor = open(node->right->value, O_RDONLY)) == -1)
		{
			perror("File error");
			exit(EXIT_FAILURE);
		}
		
		dup2(fileDescriptor, STDIN);
	}
	
	if(strcmp(node->value, "<<") == 0)
	{
		int fileDescriptor;
		
		if ((fileDescriptor = open("/tmp/my_sh.tmp", O_RDONLY)) == -1)
		{
			perror("File error");
			exit(EXIT_FAILURE);
		}
		dup2(fileDescriptor, STDIN);
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



