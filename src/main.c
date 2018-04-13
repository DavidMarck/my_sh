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

void process_command_line(char* commandLine)
{
    int argc = 0;
    char** argv = parse_to_argv(commandLine,&argc);
    //interpret_heard_file(argv, argc);

    // int fg_argc = 0;
    // char** fg_argv = NULL;

    for (int i = 0; i < (argc + 1); i++) 
    {
        printf ("argv[%d] = %s\n", i, argv[i]);
    }

    // empty command line
    if(argc == 0)
    {
        return;
    }

    // TO DO handle other syntax errors in the line beginning ?
    if(strcmp(argv[0],"&") == 0)
    {
        printf("my_sh: syntax error near unexpected token `&'");
        return;
    }

    execute_command_line(argv,argc,FALSE);

    // int nxtCmdLineIndex = 0;
    // while(includes_background(argv,argc))
    // {
    //     int bg_argc = 0;
    //     char** bg_argv = get_bg_command_args(argv,argc,&bg_argc,&nxtCmdLineIndex);
    //     for (int i = 0; i < (bg_argc + 1); i++) 
    //     {
    //         printf("bg_argv[%d] = %s\n", i, bg_argv[i]);
    //     }

    //     execute_command_line(bg_argv,bg_argc,TRUE);
    // }

    // // if there were any background processes and if there is still a foreground command
    // if((nxtCmdLineIndex != 0) && (argv[nxtCmdLineIndex]!= NULL))
    // {
    //     // we get only the foreground command's arguments
    //     fg_argv = get_fg_command_args(argv,argc,&fg_argc,nxtCmdLineIndex);

    //     for (int i = 0; i < (fg_argc + 1); i++) 
    //     {
    //         printf("fg_argv[%d] = %s\n", i, fg_argv[i]);
    //     }
    // }

    // // if after backgorund commands there was another command to run in foreground...
    // if(fg_argc > 0)
    // {
    //     execute_command_line(fg_argv,fg_argc,FALSE);
    //     return;
    // }

    // if there were no background commands to run
    // if(nxtCmdLineIndex == 0) execute_command_line(argv,argc,FALSE);
    
}

void execute_command_line(char** argv, int argc, int isBackground)
{
    commandNode* tree_arguments = parse_to_tree(argv, argc);
	execute_tree(tree_arguments,isBackground);

    // int status;
    // pid_t pid;
    // while((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    //     printf("[process %d exited with code %d]\n",
    //     pid, WEXITSTATUS(status));
    //     /* here you can remove the pid from your jobs list */
    //     int i = 0;
    //     while(BG_PIDS_ARRAY[i] != 0)
    //     {
    //         if(BG_PIDS_ARRAY[i] == pid)
    //         {
    //             BG_PIDS_ARRAY[i] = 0;
    //             break;
    //         }
    //         i++;
    //     }
    // }

    free_tree(tree_arguments);
    // TO DO free_array()
    free(argv);
}

void exit_prog(char* bin_command_param, int code) 
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
                //help param
                print_usage(argv[0]);

                free_if_needed(bin_command_param);

                exit(EXIT_SUCCESS);
            default :
                break;
        }
    }
    
    /**
     * Checking binary requirements
     */
    // if (bin_command_param == NULL)
    // {
    //     dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");
    //     exit_prog(bin_command_param, EXIT_FAILURE);
    // }

    // batch mode command
    if(bin_command_param != NULL)
    {
        // Printing params
        // dprintf(1, "** PARAMS **\n%-8s: %s\n", 
        //     "command",   bin_command_param);
        strcat(bin_command_param,"\n");
        read_command_line(bin_command_param);
        process_command_line(bin_command_param);
    }
    // init interactive shell
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

            printf("==================================\n||   PROJET : MiniShell my_sh   ||\n==================================\n");
            
            while(print_prompt())
            {
                char commandLine[MAX_SIZE];
				
                fgets(commandLine, sizeof(commandLine), stdin);         
       
                read_command_line(commandLine);

                if(strlen(commandLine) > 0)
                    write_to_history(strdup(commandLine));

                process_command_line(commandLine);
            }
        }
    }
    
    exit_prog(bin_command_param,EXIT_SUCCESS);
}
