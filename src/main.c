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
            
            read_command_line(commandLine);

            int args_count = 0;
            char** arguments = parse_command(commandLine,&args_count);

            //case no arguments (i.e. empty command line)
            if(args_count == 0)
            {
                continue;
            }

			commandNode* tree_arguments = parse_to_tree(arguments, args_count);
			execute_tree(tree_arguments);
			//print_tree(tree_arguments);
            

            // for (int i = 0; i < (args_count + 1); i++) 
            // {
            //     printf ("arguments[%d] = %s\n", i, arguments[i]);
            // }	
            

            if(strcmp(arguments[0],EXIT_STRING) == 0)
            {
                return EXIT_SUCCESS;
            }
            
            execute_command(arguments, args_count);

			free_tree(tree_arguments);

            // TO-DO free elements of arguments array
            free(arguments);
        }
    }
    return EXIT_SUCCESS;
}
