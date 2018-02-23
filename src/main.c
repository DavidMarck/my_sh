#include "../include/main.h"

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
        print_prompt();
        fgets(commandLine, sizeof(commandLine), stdin);
        clean(commandLine);
        readCommand(commandLine);
    }

    printf("It's done !");
    return EXIT_SUCCESS;
}