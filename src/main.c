#include "../include/main.h"

void prompt()
{
    printf("my_sh > ");
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
        printf("Your entered command: %s\n", command);
    }
}

int main(int argc, char** argv)
{
    char commandLine[255];

    printf("=== PROJET : MiniShell my_sh ===\n");

    while(strcmp(stringToLower(commandLine),EXIT_STRING) != 0)
    {
        prompt();
        scanf("%s", commandLine);
        readCommand(commandLine);
        break;
    }

    return EXIT_SUCCESS;
}