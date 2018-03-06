#include "commands.h"
#include <errno.h>

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

void execute_command(char** args, int argc)
{
    if(args[0] == NULL)
    {
        return;
    }
    else if(!strcmp(args[0],"cd"))
    {
        errno = 0;
        if(cd(args[1]) != 0)
        {
            perror("cd");
            return;
        }
        else
        {
            return;  
        }
    }

    char* bin = "/bin/";
    char* path = malloc(strlen(bin) * sizeof(char)) + ((strlen(args[0]) + 1) * sizeof(char));
    //char* path = malloc((strlen(args[0]) + 1) * sizeof(char));
    strcat(path,bin);
    strcat(path,args[0]);

    if (execv(path, args) == -1) {
        perror("execv");
        return;
    }

    free(path);

    return;
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
