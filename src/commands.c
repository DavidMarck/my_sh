#include "commands.h"

void print_prompt()
{
    printf("my_sh > ");
}

void parseCommand(char* command)
{
    // char** argv = NULL; // array of arguments
    // int argc = 0; // arguments count
    // char* nbChar = ""; // char count for memory allocation
    // int i = 0; // will serve as an index in the command string
    // int awaitingClosingQuote = FALSE; // wheter or not a string between quotes is being read

    // if(command[i] == '\0') return; // no arguments

    // // if there are any spaces before first argument we increment the index
    // while(command[i] == ' ')
    // {
    //     i++;
    // }

    // // if NULL here, means the user pressed enter after a bunch white spaces
    // if(command[i] == '\0') return;
    // // otherwise at least one argument in command line
    // argc++;
    // argv = realloc(argv,argc * sizeof(char*));

    // while(command[i] != '\0')
    // {
    //     if(command[i] == ' ')
    //     {
    //         // Ignoring space if between quotes
    //         if(awaitingClosingQuote == TRUE) 
    //         {
    //             //TO DO add space to current arg string
    //             //arg = strcat(arg,&command[i]);
    //             i++;
    //             continue;
    //         }

    //         // if next char after white space is not a white space as well,
    //         // new argument found
    //         if(command[i+1] != ' ') 
    //         {
    //             argv[argc - 1] = arg;
    //             arg = "";
    //             argc++;
    //             argv = realloc(argv,argc * sizeof(char*));
    //             i++;
    //             continue;
    //         }
    //     }
    //     else if(command[i] == '"')
    //     {
    //         if(awaitingClosingQuote == FALSE) awaitingClosingQuote = TRUE;
    //         else 
    //         {
    //             awaitingClosingQuote = FALSE;
    //         }
    //     }
    //     else
    //     {
    //         //TO DO add current char to arg str
    //         //arg = strcat(arg,&command[i]);
    //     }
    //     i++;
    // }

    // printf("arg count : %d\n",argc);

	char ** argv  = NULL;
	char *  p    = strtok (command, " ");
	int n_spaces = 0;

    int awaitingClosingQuote = FALSE;
    char* tmp = "";


	/* split string and append tokens to 'argv' */

	while (p) {
        if(awaitingClosingQuote != TRUE) argv = realloc (argv, sizeof (char*) * ++n_spaces);

        if (argv == NULL) exit (-1); /* memory allocation failed */

        if(awaitingClosingQuote == TRUE)
        {
            int spaceNeeded = (strlen(tmp) * sizeof(char)) + ((strlen(p) + 1) * sizeof(char)) + 1;
            realloc(tmp,spaceNeeded);
            strcat(tmp," ");
            strcat(tmp,p);
        }

        if((p[0] == '"') && (awaitingClosingQuote != TRUE))
        {
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
        else if((p[strlen(p) - 1] == '"') && (awaitingClosingQuote == TRUE))
        {
            awaitingClosingQuote = FALSE;
            argv[n_spaces-1] = tmp;
            p = strtok(NULL, " ");
            tmp = "";
            continue;
        }        
        
        argv[n_spaces-1] = p;
        p = strtok (NULL, " ");
	}

	/* realloc one extra element for the last NULL */
	argv = realloc (argv, sizeof (char*) * (n_spaces+1));
	argv[n_spaces] = 0;

	/* print the result */

	for (int i = 0; i < (n_spaces+1); ++i) {
	  printf ("argv[%d] = %s\n", i, argv[i]);
	}    
}

void parseSpace(char* str)
{
    
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

void readCommand(char* command)
{
    int commandLength = strlen(command);

    if(commandLength > 255)
    {
        printf("Your command exceeds 255 characters. Please enter a shorter command.");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Your entered command: %s\n", stringToLower(command));
    }
}
