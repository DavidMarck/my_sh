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


commandNode* parse_to_tree(char** arguments, int args_count)
{
	// initialisation of the cursors (we'll start from the last index to the first(0))
	int index = args_count-1;            // current index
	int last_string_limit = args_count;  // this cursor go to the last "special index" (it concerned all operators and redirection), by default this is the "null" value at the end
	int string_index;                    // this cursor will be used in order to concatenate some strings
	int i;
	
	// this variable will concatenate all strings between 2 specials indexes
	char* command = malloc(sizeof(char));
	
	// these variables concerned the tree building
	commandNode* cmdNode = NULL;
	commandNode* operatorOrPipeNode = NULL;
	commandNode* redirectionNode = NULL; 
	
	
	while (index > -1)
	{
		// when we have a special string (or the begin of the commmand), we concatenate all strings until we get the "special" index (or the end of the command)
		if(is_special_string(arguments[index]) == TRUE || index == 0) 
		{
			// if we are not at the first element, we don't take the "special" string into the concatenation
			if(index != 0) 
				string_index = index+1;
			else
				string_index = index;
			command = strcpy(command,"");
			
			// these loop will concatenate all string until the "special" index is reached
			for(i = string_index; i < last_string_limit; i++)
			{
				char* argument = strdup(arguments[i]);

				// if any space in the argument, then we add quotes
				if(contains(arguments[i]," "))
				{
					insert_substring(argument,"\"",1);
					insert_substring(argument,"\"",strlen(argument) + 1);
				}

				// we realloc the space and we make the concatenation
				int spaceNeeded = ((strlen(command)) * sizeof(char)) + ((strlen(argument) + 1) * sizeof(char));
				// if we're don't at the last string, we had 1 space for the whitespace
				if(i != string_index)
					spaceNeeded++;
					
				command = realloc(command,spaceNeeded);
			
				if(i != string_index)
					command = strcat(command, " ");				
				
				command = strcat(command, argument);
			}
			
			// if we reach the start of the array...
			if(index == 0)
			{
				// ... we create a new "cmdNode" which contains the value of the last concatenate string
				cmdNode = new_node(command);
				
				// ... if there is a redirection Node without fork ('>', '>>', '<', '<<'), whe add the cmdNode at the left of this one (and we had the redirection node at the left of the operator)
				if(redirectionNode != NULL)
				{	
					add_left(operatorOrPipeNode, redirectionNode);
					add_left(redirectionNode, cmdNode);
				}
				// ... else we add the cmdNode at the left of the last "operatorOrPipeNode", if there is no operator or pipe, the cmd node become the root of the tree 
				else
					cmdNode = add_left(operatorOrPipeNode, cmdNode);
			}
			// if our index is an operator or a pipe
			else if(is_logical_operator(arguments[index]) == TRUE || is_pipe(arguments[index]) == TRUE)
			{
				// ... we initiate an operatorOrPipeNode and we add it at the left of the last operatorOrPipeNode (or it becomes the main root)
				operatorOrPipeNode = add_left(operatorOrPipeNode, new_node(arguments[index]));
				
				// if there was a redirectionNode,  we add it to this node and we add a node which contains the last concatenante string and we turn the redirectionNode variable to null 
				if(redirectionNode != NULL) 
				{
					add_right(operatorOrPipeNode, redirectionNode);
					add_left(redirectionNode, new_node(command));
					redirectionNode = NULL;
				}
				// else we just add the node with the concatenate string at the right of this node
				else
					add_right(operatorOrPipeNode, new_node(command));
			}
			// if we have a redirection without fork, we declare it, and we add a node which contains the last concatenate string at its right
			else if(is_redirection_without_fork(arguments[index]) == TRUE)
			{
				redirectionNode = new_node(arguments[index]);
				add_right(redirectionNode, new_node(command));
			}
			
			last_string_limit = index;
		} 
		index--;
	}
	free(command);
	return cmdNode->mainRoot;
}

int is_special_string(char* argument)
{	
	int index = 0;
	char* operators[] = {"|", "||", "&&", ">", "<", ">>", "<<","&"};
	
	
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

int is_logical_operator(char* argument) 
{
	int index = 0;
	char* operators[] = {"||", "&&"};
	
	while (index < 2)
	{
		if(strcmp(operators[index], argument) == 0)
		{
			return TRUE;
		}
		index++;
	}
	return FALSE;
}

int is_pipe(char* argument)
{
	return (strcmp(argument, "|") == 0);
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
	// we get the last left child, i.e. the leftmost child in the tree
	commandNode* last_left_child = get_last_left_child(root);
	// we interpret that last left child node, which will recursively interpret the entire tree
	interpret_node(last_left_child);
}

void interpret_node(commandNode* node)
{
	int argc = 0;
	char** argv;
	
	// if the current node is root
	if(node->parentNode == NULL && node->left == NULL && node->right == NULL)
	{
		argv = parse_to_argv(node->value,&argc);
		execute_command(argv, argc);
	}
	else 
	{
		// current node is a left child
		if(is_left_child(node))
		{
			// value of left child is not && , || or |
			if(is_logical_operator(node->value) == FALSE && is_pipe(node->value) == FALSE)
			{
				// we check current node's parent value to determine which logic should be applied

				// Logical and : &&
				if(strcmp(node->parentNode->value, "&&") == 0)
				{
					argv = parse_to_argv(node->value,&argc);
					if(execute_command(argv, argc) >= 0)
					{
						interpret_node(node->parentNode->right);
					}
				}
				
				// Logical or : ||
				if(strcmp(node->parentNode->value, "||") == 0)
				{
					argv = parse_to_argv(node->value,&argc);
					if(execute_command(argv, argc) < 0)
					{
						interpret_node(node->parentNode->right);
					}	
				}
				
				// Redirection that is not implying a fork : <, <<, >, >>
				if(is_redirection_without_fork(node->parentNode->value))
				{
					interpret_node(node->parentNode->right);
				}
				
				// Pipeline : |
				if(is_pipe(node->parentNode->value))
				{
					execute_pipe_left(node);
				}
			}	
		}
		// current node is a right child
		else 
		{
			// case node is first right child
			if((node->parentNode->parentNode == NULL))
			{
				// parent (i.e. root) is a redirection without fork
				if(is_redirection_without_fork(node->parentNode->value))
				{
					execute_redirection_without_fork(node->parentNode); 
				}
				// node itself is a redirection without fork
				else if(is_redirection_without_fork(node->value))
				{
					execute_redirection_without_fork(node);
				}
				// no redirection, execution of node's command
				else
				{
					argv = parse_to_argv(node->value,&argc);
					execute_command(argv, argc);
				}
			}
			// all the other right childs
			else 
			{
				// check if right child's grand parent is && or || operator
				// to determine logic to be applied
				if(is_logical_operator(node->parentNode->parentNode->value))
				{
					argv = parse_to_argv(node->value,&argc);
					int return_code;
					
					// node is a redirection without fork
					if(is_redirection_without_fork(node->value))
					{
						return_code = execute_redirection_without_fork(node);
					}
					// parent is a redirection without fork
					else if(is_redirection_without_fork(node->parentNode->value))
					{
						return_code = execute_redirection_without_fork(node->parentNode);
					}
					// no redirection, execution of node's command
					else
					{
						return_code = execute_command(argv, argc);
					}
					
					// Logical and : &&
					if(strcmp(node->parentNode->parentNode->value, "&&") == 0)
					{
						if(return_code >= 0)
						{
							interpret_node(node->parentNode->parentNode->right);
						}
					}
					
					// Logical or : ||
					if(strcmp(node->parentNode->parentNode->value, "||") == 0)
					{
						if(return_code < 0)
						{
							interpret_node(node->parentNode->parentNode->right);
						}
					}
				}
				// Grand parent is a pipeline operator, meaning a pipeline should be initiated
				else if(is_pipe(node->parentNode->parentNode->value))
				{
					execute_pipe_right(node);					
				}	
			}			
		}
	}
}

void execute_pipe_left(commandNode* node)
{
	if(!is_pipe(node->parentNode->value))
	{
		printf("pipe : Tried to pipeline non pipelined nodes!");
		return;
	}

	int argc = 0;
	char** argv;

	// pipe's descriptors
	int pipeDescs[2];         
	if(pipe(pipeDescs) == -1)
	{
		perror("pipe");
	}
	else 
	{
		int pid;
		// fork
		if((pid = fork()) < 0)
		{
			perror("fork");
		}
		else 
		{
			// save current STDIN
			int saved_stdin = dup(STDIN);
			// child
			if (pid ==0) 
			{		
				// close the output descriptor of the pipe 	 
				close(pipeDescs[0]);
				// redirection of STDOUT to pipe's input descriptor							 	
				dup2(pipeDescs[1], STDOUT);				
				// then, execution of the command. Its output, if any, will be redirected into the pipe				
				argv = parse_to_argv(node->value,&argc);
				if(execute_command(argv, argc) < 0)
				{
					exit(EXIT_FAILURE);
				}
				exit(EXIT_SUCCESS);
				
			}
			// parent
			else 
			{
				wait(NULL);
				
				close(pipeDescs[1]); // we close pipe's input descriptor
				dup2(pipeDescs[0], STDIN); // redirection of STDIN on pipe's output descriptor
					
				// then, we intepret the pipelined node
				// it will receive previous command's output as its input
				interpret_node(node->parentNode->right);
				
				// here we reset STDIN to its default state
				dup2(saved_stdin, STDIN);
				close(saved_stdin);
			}
		}
	}
}

void execute_pipe_right(commandNode* node)
{
	if(!is_pipe(node->parentNode->parentNode->value))
	{
		printf("pipe : Tried to pipeline non pipelined nodes!");
		return;
	}

	int argc = 0;
	char** argv;

	// pipe's descriptors
	int pipeDescs[2];         
	if(pipe(pipeDescs) == -1)
	{
		perror("pipe");
	}
	else 
	{
		int pid;
		// fork
		if((pid = fork()) < 0)
		{
			perror("fork");
		}
		else 
		{
			// save current STDIN
			int saved_stdin = dup(STDIN);
			// child
			if (pid ==0) 
			{		
				// close the output descriptor of the pipe 
				close(pipeDescs[0]);
				// redirection of STDOUT to pipe's input descriptor							 	
				dup2(pipeDescs[1], STDOUT);	
				int return_code;
				
				if(is_redirection_without_fork(node->parentNode->value))
				{
					return_code = execute_redirection_without_fork(node->parentNode);
				}
				else							
				{
					// then, execution of the command. Its output, if any, will be redirected into the pipe	
					argv = parse_to_argv(node->value,&argc);
					return_code = execute_command(argv, argc);
				}
				if(return_code < 0)
				{
					exit(EXIT_FAILURE);
				}
				exit(EXIT_SUCCESS);
				
			}
			// parent
			else 
			{
				wait(NULL);
				
				close(pipeDescs[1]); // we close pipe's input descriptor
				dup2(pipeDescs[0], STDIN); // redirection of STDIN on pipe's output descriptor
					
				// then, we intepret the pipelined node
				// it will receive previous command's output as its input
				interpret_node(node->parentNode->parentNode->right);
				
				// here we reset STDIN to its default state
				dup2(saved_stdin, STDIN);
				close(saved_stdin);
			}
		}
	}	
}

int execute_redirection_without_fork(commandNode* node)
{
	int saved_stdout = dup(STDOUT);
	int saved_stdin = dup(STDIN);
	
	// case > : We open the file (with The Truncate flag) and we redirect STDOUT to it
	if(strcmp(node->value, ">") == 0)
	{
		int fileDescriptor;
		if ((fileDescriptor = open(node->right->value, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
		{
			fprintf(stderr,"file open: %s: %s\n",node->right->value,strerror(errno));
			return -1;
		}
		dup2(fileDescriptor, STDOUT);
	}
	
	// case >> : Same as > but with the append flag
	if(strcmp(node->value, ">>") == 0)
	{
		int fileDescriptor;
		if ((fileDescriptor = open(node->right->value, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR)) == -1)
		{
			fprintf(stderr,"file open: %s: %s\n",node->right->value,strerror(errno));
			return -1;
		}
		dup2(fileDescriptor, STDOUT);
	}
	
	// case < or << we open the file (readonly) and we redirect stdin to it (it works with << because we replaced the delimiter by the tmp file path.
	if((strcmp(node->value, "<") == 0) || (strcmp(node->value, "<<") == 0))
	{
		int fileDescriptor;
		
		if ((fileDescriptor = open(node->right->value, O_RDONLY)) == -1)
		{
			fprintf(stderr,"file open: %s: %s\n",node->right->value,strerror(errno));
			return -1;
		}
		
		dup2(fileDescriptor, STDIN);
	}
	
	// Executing interpret the left child node of the redirection before to restore all streams
	int argc = 0;
	char** argv = parse_to_argv(node->left->value,&argc);
	int return_code = execute_command(argv, argc);
	
	dup2(saved_stdout, STDOUT);
	dup2(saved_stdin, STDIN);
	close(saved_stdin);
	close(saved_stdout);
	
	// if we used a tmp file, we remove it
	if((strcmp(node->value, "<<") == 0))
	{
		remove(node->right->value);
	}
	
	// return code
	if(return_code < 0)
	{
		return -1;
	}
	return 0;
}

void print_tree(commandNode* root)
{
    if (root == NULL) return; 
	
	printf("tree : %s\n", root->value);
    print_tree(root->left);
    print_tree(root->right);
      
}

void free_tree(commandNode* root)
{         
	if( root ){
     free_tree(root->left);
     free_tree(root->right);
     free(root->value); 
     free(root);
   }
}



