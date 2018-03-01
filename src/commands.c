#include "commands.h"

void print_prompt()
{
    printf("my_sh > ");
}

void parseCommand(char* command)
{
    //char** argv = NULL;
    int argc = 0; // arguments count
    int i = 0; // will serve as an index in the command string
    int awaitingClosingQuote = FALSE;

    if(command[i] == '\0') return; // no arguments
    //else if(command[i] != ' ') argc++; //no space as first argument

    // if there are any spaces before first argument we increment the index
    while(command[i] == ' ')
    {
        i++;
        //if(command[i] != ' ') argc++;
    }

    // if NULL here, means the user pressed enter after only white spaces
    if(command[i] == '\0') return;
    // otherwise at least one argument
    argc++;

    while(command[i] != '\0')
    {
        if(command[i] == ' ')
        {
            if(awaitingClosingQuote == TRUE) 
            {
                i++;
                continue;
            }

            // if next char after white space is not a white space as well
            // we increment the arg count
            if(command[i+1] != ' ') argc++;
        }
        else if(command[i] == '"')
        {
            if(awaitingClosingQuote == FALSE) awaitingClosingQuote = TRUE;
            else 
            {
                awaitingClosingQuote = FALSE;
            }
        }
        i++;
    }
    //argc++;

    printf("arg count : %d\n",argc);

	// char ** argv  = NULL;
	// char *  p    = strtok (command, " ");
	// int n_spaces = 0;


	// /* split string and append tokens to 'argv' */

	// while (p) {
    //     argv = realloc (argv, sizeof (char*) * ++n_spaces);

    //     if (argv == NULL) exit (-1); /* memory allocation failed */

    //     if(p[0] == '"')
    //     {            
    //         p = strtok(NULL, "\"");
    //         argv[n_spaces-1] = p;
    //         /*for(int i = 0; i < n_spaces; i++)
    //         {
    //             strtok(command," ");
    //         }*/
    //     }
    //     else
    //     {
    //         argv[n_spaces-1] = p;
    //     }
        
    //     //if(p[0] == '"') printf("%s",strtok(argv[n_spaces-1],"\""));

    //     p = strtok (NULL, " ");
	// }

	// /* realloc one extra element for the last NULL */
	// argv = realloc (argv, sizeof (char*) * (n_spaces+1));
	// argv[n_spaces] = 0;

	// /* print the result */

	// for (int i = 0; i < (n_spaces+1); ++i) {
	//   printf ("argv[%d] = %s\n", i, argv[i]);
	// }    
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
