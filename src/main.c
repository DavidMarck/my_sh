#include "main.h"

/**
 * Progam
 * @param argc arguments count
 * @param argv array of arguments
 */
int main(int argc, char** argv)
{
    char commandLine[255];

    int pid = fork();

    if(pid == 0) {
        execlp("clear", NULL);
    }
    else {
        wait(NULL);

        printf("=== PROJET : MiniShell my_sh ===\n");
        
        while(strcmp(string_to_lower(commandLine),EXIT_STRING) != 0)
        {
            print_prompt();
            fgets(commandLine, sizeof(commandLine), stdin);
            clean(commandLine);

            char** argv = NULL;
            int argc = 0;
            parse_command(commandLine,argv,&argc);
        }
    }
    return EXIT_SUCCESS;
}