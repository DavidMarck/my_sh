#include "commands.h"

void print_prompt()
{
    printf("my_sh > ");
}

void parseCommand(char* command)
{   

	char ** res  = NULL;
	char *  p    = strtok (command, " ");
	int n_spaces = 0, i;


	/* split string and append tokens to 'res' */

	while (p) {
	  res = realloc (res, sizeof (char*) * ++n_spaces);

	  if (res == NULL)
		exit (-1); /* memory allocation failed */

	  res[n_spaces-1] = p;

	  p = strtok (NULL, " ");
	}

	/* realloc one extra element for the last NULL */

	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = 0;

	/* print the result */

	for (i = 0; i < (n_spaces+1); ++i) {
	  printf ("res[%d] = %s\n", i, res[i]);
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
