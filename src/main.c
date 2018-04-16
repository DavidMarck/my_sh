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
    char* saved_command_line = strdup(commandLine);

    int argc = 0;
    char** argv = parse_to_argv(commandLine,&argc);

    // if any << in command line
    if(contains(saved_command_line,"<<"))
    {
        char** argv_heard_file = interpret_heard_file(argv, argc);

        // if the two arrays are identical, syntax error in use of << operator exists
        if(argv == argv_heard_file)
        {
            return;
        }
        argv = argv_heard_file;
    }

    int last_argc = 0;
    char** last_argv = NULL;

    // empty command line
    if(argc == 0)
    {
        return;
    }

    // syntax error : operator at the beginning
    if(is_special_string(argv[0]))
    {
        printf("my_sh: syntax error near unexpected token '%s'\n",argv[0]);
        return;
    }

    int nxtCmdLineIndex = 0;
    while(includes_multplie_commands(argv,argc))
    {
        int nxt_argc = 0;
        char** nxt_argv = get_next_command_args(argv,argc,&nxt_argc,&nxtCmdLineIndex);

        execute_command_line(nxt_argv,nxt_argc,TRUE);
    }

    // if there were any ; and if there is still a last command
    if((nxtCmdLineIndex != 0) && (argv[nxtCmdLineIndex]!= NULL))
    {
        // we get only the foreground command's arguments
        last_argv = get_last_command_args(argv,argc,&last_argc,nxtCmdLineIndex);
    }

    // if after all the commands separated by ; there was another command to run...
    if(last_argc > 0)
    {
        execute_command_line(last_argv,last_argc,FALSE);
        return;
    }

    // if there were no ; we use the original argv
    if(nxtCmdLineIndex == 0) execute_command_line(argv,argc,FALSE);
    
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
                {
					if((write_to_history(strdup(commandLine))) == 0)
						process_command_line(commandLine);
					else
						fprintf(stderr, "Error : couldn't write to history. Command not executed");
				}
            }
        }
    }
    
    exit_prog(bin_command_param,EXIT_SUCCESS);
}
