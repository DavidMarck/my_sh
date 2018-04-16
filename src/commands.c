#include "commands.h"

const char* built_in_commands[] = {"cd", "pwd", "echo", "exit"};
int n_built_in = sizeof(built_in_commands) / sizeof(const char*);

char** parse_to_argv(char* command, int* argc)
{
	char** argv  = NULL; // args array
	char *  p    = strtok (command, " "); // token
	int n_spaces = 0; // args counter

    int awaitingClosingQuote = FALSE; // indicates wether or not quote is opened and not closed
    char* tmp = ""; // hold between quotes string until closing quote reached


	/* split string and append tokens to 'argv' */

	while (p) {
        // reallocating memory as the arg count augments
        // (only reallocating once if arg between quotes, i.e. spaces ignored)
        if(awaitingClosingQuote != TRUE) argv = realloc (argv, sizeof (char*) * ++n_spaces);

        if (argv == NULL) exit (-1); /* memory allocation failed */

        // if still waiting for closing quote
        if(awaitingClosingQuote == TRUE)
        {
            int spaceNeeded = (strlen(tmp) * sizeof(char)) + ((strlen(p) + 1) * sizeof(char)) + 1;
            tmp = realloc(tmp,spaceNeeded);
            strcat(tmp," ");
            strcat(tmp,p);
        }

        // if current argument starts with quotes
        if((p[0] == '"') && (awaitingClosingQuote != TRUE))
        {
            // only one word between quotes = normal token
            if(p[strlen(p) - 1] == '"')
            {
                p = str_replace("\"","",p);
                argv[n_spaces-1] = p;
                p = strtok (NULL, " ");
                continue;
            }

            // initiates tmp string to hold string between quotes
            awaitingClosingQuote = TRUE;
            tmp = strdup(p);
            p = strtok(NULL, " ");
            continue;
        }
        // if closing quote is reached
        else if((p[strlen(p) - 1] == '"') && (awaitingClosingQuote == TRUE))
        {
            awaitingClosingQuote = FALSE;
            tmp = str_replace("\"","",tmp);
            argv[n_spaces-1] = tmp;
            p = strtok(NULL, " ");
            tmp = "";
            continue;
        }
        // end of command string is reached while still waiting for closing quote
        else if((awaitingClosingQuote == TRUE) && (p[strlen(p)] == '\0'))
        {
            argv[n_spaces-1] = tmp;
            p = strtok(NULL, " ");
            continue;
        }   
        
        argv[n_spaces-1] = p;
        p = strtok (NULL, " ");
	}

	/* realloc one extra element for the last NULL 
     (necessary for exec fucntion)*/
	argv = realloc (argv, sizeof (char*) * (n_spaces+1));
	argv[n_spaces] = 0;

    *argc = n_spaces;
    
    return argv;
}

char** interpret_heard_file(char** argv, int args_count)
{
	int index = 0;
	
	// brownsing the entire command
	while(index < args_count)
	{
		// case a "<<" is find
		if(strcmp("<<", argv[index]) == 0)
		{
			
			char line_input[MAX_SIZE];
			char* stdin_text = malloc(sizeof(char));

            if(argv[index+1] == NULL)
            {
                fprintf(stderr,"bash: syntax error near unexpected token '<<'\n");
                return argv;
            }
			char* delimiter = malloc(strlen(argv[index+1])*sizeof((char) + 1));
			
			delimiter = strcpy(delimiter, argv[index+1]);
			delimiter = strcat(delimiter,"\n");  // adding a \n to the delimiter  (in order to be able to compare it with fget)
			
			// letting the user write some lines until he writes the delimiter after << 
			do
			{
				printf(" > ");
				fgets(line_input, sizeof(line_input), stdin);
				
				if(strcmp(delimiter, line_input) != 0)
				{
					int spaceNeeded = ((strlen(stdin_text)) * sizeof(char)) + ((strlen(line_input) + 1) * sizeof(char));
					stdin_text = realloc(stdin_text,spaceNeeded);
					strcat(stdin_text, line_input);
				}	
			} while(strcmp(delimiter, line_input) != 0);
			
			free(delimiter);
			
			// creating a temporary files (which will be readed during the execution of the tree  	
			int descTemp;
			static char template[] = "/tmp/tmpShellXXXXXX.tmp";
			char fileName[23];
			
			strcpy(fileName, template);
			
			descTemp = mkstemps(fileName,4);
			if(descTemp == -1) {
				perror("error !");
				dprintf(STDERR, "Error in mkstemp.\n");
				exit(EXIT_FAILURE);
			}
			
			// writing what the user wrote before into the tmp file
			FILE *fp = fdopen(descTemp, "w");
			if (fp == NULL)
			{
				perror("Error when opening temp file");
				exit(EXIT_FAILURE);
			}
			
			fputs(stdin_text, fp);
			
			// replacing  the delimiter by the tmp file path
			char* tmp = malloc(sizeof(char) * strlen(fileName));
			strcpy(tmp, fileName);
			argv = str_array_replace(argv, args_count,(index+1), tmp);
			
			fclose(fp);
			close(descTemp);
			free(stdin_text);
		}
		index++;
	}
	return argv;
}

int execute_command(char** argv, int argc)
{
    // case built-in command
	if(isbuiltin(argv[0]))
	{
		return execute_builtin(argv, argc);
	}
    // other commands
	else
	{
        // fork
		int pid;
		int status;
		if ((pid = fork()) < 0)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}

        // child
		if(pid == 0)
		{
            // buidling the path
			char* bin = "/bin/";
			char* path = malloc(strlen(bin) * sizeof(char)) + ((strlen(argv[0]) + 1) * sizeof(char));
			*path = '\0';
			path = strcat(path,bin);
			path = strcat(path,argv[0]);
			
            // execution
			if (execvp(path, argv) == -1) {
				if(errno == ENOENT)
				{
					fprintf(stderr,"%s: Command not found\n",argv[0]);
				}
				else
				{
					perror(argv[0]);
				}
				exit(-1);
			}
		}
        // parent
		else
		{
			wait(&status);
			if(WIFEXITED(status) && WEXITSTATUS(status) == 0) 
			{
				return status;
			}
			return -1;
		}
		
	}
	return -1;
	
    /*if(argv[0] == NULL)
    {
        return;
    }
    
    else if(isbuiltin(argv[0]))
    {
        if(execute_builtin(argv,argc) == 0) exit(EXIT_SUCCESS);
        else exit(EXIT_FAILURE);
    }

    char* bin = "/bin/";
    char* path = malloc(strlen(bin) * sizeof(char)) + ((strlen(argv[0]) + 1) * sizeof(char));
    strcat(path,bin);
    strcat(path,argv[0]);

    if (execvp(path, argv) == -1) {
        if(errno == ENOENT)
        {
            fprintf(stderr,"%s: Command not found\n",argv[0]);
        }
        else
        {
            perror(argv[0]);
        }
        free(path);
        exit(EXIT_FAILURE);
    }

    printf("\n\n");

    free(path);
    */

}

int execute_builtin(char** argv, int argc)
{
    int returnCode;

    if(!isbuiltin(argv[0]))
    {
        returnCode = -1;
    }
    else if(strcmp(argv[0],"cd") == 0)
    {
        returnCode = cd(argv,argc);
    }
    else if(strcmp(argv[0],"pwd") == 0)
    {
        returnCode = pwd(argv,argc);
    }
    else if(strcmp(argv[0],"echo") == 0)
    {
        returnCode = echo(argv,argc);
    }
    else if(strcmp(argv[0],"exit") == 0)
    {
        builtin_exit(argv,argc);
    }

    printf("\n");

    return returnCode;
}

int isbuiltin(char* commandLine)
{
    for(int i = 0; i < n_built_in; i++)
    {
        if(strcmp(commandLine,built_in_commands[i]) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int includes_multplie_commands(char** argv, int argc)
{
    for(int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i],";") == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}

char** get_next_command_args(char** argv, int argc, int* nxt_argc, int* nxtCmdLineIndex)
{
    char** nxt_argv = NULL;

    int n_args = 0;
    int i = *nxtCmdLineIndex;

    // We copy all the arguments to get only the ones concerned by the next ; operator
    while(strcmp(argv[i],";") != 0)
    {
        // reallocating memory
        nxt_argv = realloc(nxt_argv, sizeof (char*) * ++n_args);
        if (nxt_argv == NULL) exit (-1); /* memory allocation failed */

        nxt_argv[n_args - 1] = argv[i];
        i++;
    }
    // Sets ";" to "" in argv
    argv[i] = "";
    *nxtCmdLineIndex = i + 1;

    /* realloc one extra element for the last NULL 
     (necessary for exec fucntion)*/
	nxt_argv = realloc (nxt_argv, sizeof (char*) * (n_args+1));
	nxt_argv[n_args] = 0;

    *nxt_argc = n_args;

    return nxt_argv;
}

char** get_last_command_args(char** argv, int argc, int* last_argc, int startIndex)
{
    char** last_argv = NULL;

    int n_args = 0;

    // We copy all the arguments at the end (after all the ; operators)
    for(int i = startIndex; i < argc; i++)
    {
        // reallocating memory
        last_argv = realloc(last_argv, sizeof (char*) * ++n_args);
        if (last_argv == NULL) exit (-1); /* memory allocation failed */

        last_argv[n_args - 1] = argv[i];
    }

    /* realloc one extra element for the last NULL 
     (necessary for exec fucntion)*/
	last_argv = realloc (last_argv, sizeof (char*) * (n_args+1));
	last_argv[n_args] = 0;

    *last_argc = n_args;

    return last_argv;
}

// int includes_background(char** argv, int argc)
// {
//     for(int i = 0; i < argc; i++)
//     {
//         if(strcmp(argv[i],"&") == 0)
//         {
//             return TRUE;
//         }
//     }

//     return FALSE;
// }

// char** get_bg_command_args(char** argv, int argc, int* bg_argc, int* nxtCmdLineIndex)
// {
//     char** bg_argv = NULL;

//     int n_args = 0;
//     int i = *nxtCmdLineIndex;

//     // We copy all the arguments to get only the ones concerned by the next & operator
//     while(strcmp(argv[i],"&") != 0)
//     {
//         // reallocating memory
//         bg_argv = realloc(bg_argv, sizeof (char*) * ++n_args);
//         if (bg_argv == NULL) exit (-1); /* memory allocation failed */

//         bg_argv[n_args - 1] = argv[i];
//         i++;
//     }
//     // Sets "&" to "" in argv
//     argv[i] = "";
//     *nxtCmdLineIndex = i + 1;

//     /* realloc one extra element for the last NULL 
//      (necessary for exec fucntion)*/
// 	bg_argv = realloc (bg_argv, sizeof (char*) * (n_args+1));
// 	bg_argv[n_args] = 0;

//     *bg_argc = n_args;

//     return bg_argv;
// }

// char** get_fg_command_args(char** argv, int argc, int* fg_argc, int startIndex)
// {
//     char** fg_argv = NULL;

//     int n_args = 0;

//     // We copy all the arguments at the end (after all the & operators)
//     for(int i = startIndex; i < argc; i++)
//     {
//         // reallocating memory
//         fg_argv = realloc(fg_argv, sizeof (char*) * ++n_args);
//         if (fg_argv == NULL) exit (-1); /* memory allocation failed */

//         fg_argv[n_args - 1] = argv[i];
//     }

//     /* realloc one extra element for the last NULL 
//      (necessary for exec fucntion)*/
// 	fg_argv = realloc (fg_argv, sizeof (char*) * (n_args+1));
// 	fg_argv[n_args] = 0;

//     *fg_argc = n_args;

//     return fg_argv;
// }
