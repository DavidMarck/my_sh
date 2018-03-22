#include "main.h"

void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}

void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);  
}

char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_SIZE);
    
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}

void exitProg(char* bin_command_param, int code) 
{
   // Freeing allocated data
    free_if_needed(bin_command_param);
    // Exiting with a failure ERROR CODE (== 1)
    exit(code);
}

/**
 * Progam
 * @param argc arguments count
 * @param argv array of arguments
 */
int main(int argc, char** argv)
{

    /**
     * Binary variables
     * (could be defined in a structure)
     */
    char* bin_command_param = NULL;

    // Parsing options
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
            case 'c':
                //input param
                if (optarg)
                {
                    bin_command_param = dup_optarg_str();         
                }
                break;
            case 'h':
                print_usage(argv[0]);

                free_if_needed(bin_command_param);

                exit(EXIT_SUCCESS);
            default :
                break;
        }
    }
    
    /**
     * Checking binary requirements
     * (could be defined in a separate function)
     */
    // if (bin_command_param == NULL)
    // {
    //     dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");
    //     exitProg(bin_command_param, EXIT_FAILURE);
    // }

    // Printing params
    dprintf(1, "** PARAMS **\n%-8s: %s\n", 
        "command",   bin_command_param);

    if(bin_command_param != NULL)
    {
        strcat(bin_command_param,"\n");
        read_command_line(bin_command_param);
        int args_count = 0;
       
        char** arguments = parse_command(bin_command_param,&args_count);

/*	
        for (int i = 0; i < (args_count + 1); i++) 
        {
            printf ("arguments[%d] = %s\n", i, arguments[i]);
        }
*/		
        commandNode* tree_arguments = parse_to_tree(arguments, args_count);

	    execute_tree(tree_arguments);

        free_tree(tree_arguments);
        free(arguments);
    }
    else
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
                printf("Parsing de la commande en tableau\n");
                char** arguments = parse_command(commandLine,&args_count);
                printf("Fin...\n");
/*
                for (int i = 0; i < (args_count + 1); i++) 
                {
                    printf ("arguments[%d] = %s\n", i, arguments[i]);
                }
*/

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
                printf("Parsing de la commande en arbre\n");
                commandNode* tree_arguments = parse_to_tree(arguments, args_count);
                printf("Fin...\n");
				printf("Exécution de l'arbre\n");
                execute_tree(tree_arguments);
                printf("Fin...\n");
                //print_tree(tree_arguments);                
                
                //execute_command(arguments, args_count);

                free_tree(tree_arguments);

                // TO-DO free elements of arguments array
                free(arguments);
            }
        }
    }
    
    exitProg(bin_command_param,EXIT_SUCCESS);
}
