#include "main.h"

/**
 * Progam
 * @param args_count arguments count
 * @param argv array of arguments
 */
int main(int args_count, char** argv)
{

    int pid = fork();

    if(pid == 0) 
    {
        execlp("/bin/clear","clear", NULL);
    }
    else 
    {
        wait(NULL);

        printf("=== PROJET : MiniShell my_sh ===\n");
        
        while(print_prompt())
        {
            char commandLine[MAX_SIZE];

            fgets(commandLine, sizeof(commandLine), stdin);
            //commandLine[strlen(commandLine) - 1] = '\0';
            //clean(commandLine);            
            read_command_line(commandLine);

            int args_count = 0;
            char** arguments = parse_command(commandLine,&args_count);
            
            // case no arguments (i.e. empty command line)
            if(args_count == 0)
            {
                continue;
            }

            // call to exit
            if(strcmp(arguments[0],EXIT_STRING) == 0)
            {
                execute_command(arguments,args_count);
            }
            // call to any other command
			commandNode* tree_arguments = parse_to_tree(arguments, args_count);
			execute_tree(tree_arguments);
            
            // execute_command(arguments, args_count);

			free_tree(tree_arguments);

            // TO-DO free elements of arguments array
            free(arguments);
        }
    }
    return EXIT_SUCCESS;
}
