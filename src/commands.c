#include "../include/commands.h"

/**
 * Prints the command lines starter
 */
void print_prompt()
{
    printf("my_sh > ");
}

/**
 * Parse the text of the commandLine
 */ 
void parseCommand()
{

}

/**
 * Clean result of fgets, remove "\n" and flush stdin
 * @param buffer string to clean
 */ 
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

/**
 * Reads a string as a command
 * @param command string to be read as a command
 */
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