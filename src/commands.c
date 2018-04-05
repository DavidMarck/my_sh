#include "commands.h"

const char* built_in_commands[] = {"cd", "pwd", "echo", "exit"};
int n_built_in = sizeof(built_in_commands) / sizeof(const char*);

char** parse_to_argv(char* command, int* argc)
{
	char** argv  = NULL;
	char *  p    = strtok (command, " ");
	int n_spaces = 0;

    int awaitingClosingQuote = FALSE;
    char* tmp = "";


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

        // if current argument starts quotes
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

            awaitingClosingQuote = TRUE;
            tmp = strdup(p);
            p = strtok(NULL, " ");
            continue;
        }
        // if ending quote reached
        else if((p[strlen(p) - 1] == '"') && (awaitingClosingQuote == TRUE))
        {
            awaitingClosingQuote = FALSE;
            tmp = str_replace("\"","",tmp);
            argv[n_spaces-1] = tmp;
            p = strtok(NULL, " ");
            tmp = "";
            continue;
        }
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

void execute_command(char** argv, int argc)
{
    if(argv[0] == NULL)
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
        exit(EXIT_FAILURE);
    }

    printf("\n\n");

    free(path);
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
