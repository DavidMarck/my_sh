#include "commands.h"

const char* built_in_commands[] = {"cd", "pwd", "echo", "exit"};
int n_built_in = sizeof(built_in_commands) / sizeof(const char*);

void print_prompt()
{
    char buffer[1024];
    char* dir = getcwd(buffer,sizeof(buffer));

    char* prompt = strcat(dir," > ");
    printf(prompt);
    //printf("my_sh > ");
}

char** parse_command(char* command, int* argc)
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
            realloc(tmp,spaceNeeded);
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
        execute_builtin(argv,argc);
        return;
    }

    char* bin = "/bin/";
    char* path = malloc(strlen(bin) * sizeof(char)) + ((strlen(argv[0]) + 1) * sizeof(char));
    strcat(path,bin);
    strcat(path,argv[0]);

    if (execv(path, argv) == -1) {
        perror("execv");
        return;
    }

    free(path);
}

void clean(const char *buffer)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(stdin)) != '\n' && c != EOF);
    }
}

void read_command(char* command)
{
    int commandLength = strlen(command);

    if(commandLength > 255)
    {
        printf("Your command exceeds 255 characters. Please enter a shorter command.");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Your entered command: %s\n", string_to_lower(command));
    }
}

int execute_builtin(char** argv, int argc)
{
    int code;

    if(!isbuiltin(argv[0]))
    {
        code = -1;
    }
    else if(strcmp(argv[0],"cd") == 0)
    {
        code = cd(argv,argc);
    }
    else if(strcmp(argv[0],"pwd") == 0)
    {
        //code = pwd(argv,argc);
    }
    else if(strcmp(argv[0],"echo") == 0)
    {
        //code = echo(argv,argc);
    }
    else if(strcmp(argv[0],"exit") == 0)
    {
        //code = exit();
    }

    return code;
}

int isbuiltin(char* command)
{
    for(int i = 0; i < n_built_in; i++)
    {
        if(strcmp(command,built_in_commands[i]) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
