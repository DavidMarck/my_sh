#include "main.h"

/**
 * Progam
 * @param args_count arguments count
 * @param argv array of arguments
 */
int main(int args_count, char** argv)
{

	printf("########################################TEST Arbre############################################\n");
	commandNode* cmdNode = new_node("&&");
	cmdNode = insert_left(cmdNode, new_node("|"));
	cmdNode = insert_left(cmdNode, new_node("cat toto.txt"));
	cmdNode = insert_right(cmdNode, new_node("grep ^root"));
	cmdNode = cmdNode->mainRoot;
	cmdNode = insert_right(cmdNode, new_node("echo 'bitch'"));
	print_tree(cmdNode->mainRoot);
	freeTree(cmdNode->mainRoot);
    char commandLine[255];

    int pid = fork();

    if(pid == 0) {
        execlp("/bin/clear","clear", NULL);
    }
    else {
        wait(NULL);

        printf("=== PROJET : MiniShell my_sh ===\n");
        
        while(strcmp(string_to_lower(commandLine),EXIT_STRING) != 0)
        {
            print_prompt();
            fgets(commandLine, sizeof(commandLine), stdin);
            clean(commandLine);

            int args_count = 0;
            char** arguments = parse_command(commandLine,&args_count);

            // case no arguments (i.e. empty command line)
            if(args_count == 0)
            {
                continue;
            }
            
            // 
            if(isbuiltin(arguments[0]))
            {
                printf("youhou");
            }

            for (int i = 0; i < (args_count + 1); i++) {
                printf ("arguments[%d] = %s\n", i, arguments[i]);
            }

            if(strcmp(arguments[0],EXIT_STRING) == 0)
            {
                return EXIT_SUCCESS;
            }
            
            execute_command(arguments, args_count);

            // TO-DO free elements of arguments array
            free(arguments);
        }
    }
    return EXIT_SUCCESS;
}
