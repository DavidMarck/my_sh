#include "../include/main.h"

/**
 * Prints the command lines starter
 */
void prompt()
{
    printf("my_sh > ");
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

/**
 * Progam
 * @param argc arguments count
 * @param argv array of arguments
 */
int main(int argc, char** argv)
{
    char commandLine[255];

    printf("=== PROJET : MiniShell my_sh ===\n");

    while(strcmp(stringToLower(commandLine),EXIT_STRING) != 0)
    {
        prompt();
        fgets(commandLine, sizeof(commandLine), stdin);
        clean(commandLine);
        readCommand(commandLine);
    }

    return EXIT_SUCCESS;
}