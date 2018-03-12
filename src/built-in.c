#include "built-in.h"

int cd(char** argv, int argc)
{
    int returnCode;

    // case cd with no argument
    if(argc == 1)
    {
        returnCode = chdir(getenv("HOME"));
    }
    else if(strcmp(argv[1],"--help") == 0)
    {
        returnCode = 0;
        printf("cd: cd [dir]\n\tChange the shell working directory.\n\n\tChange the current directory to DIR.  The default DIR is the value of the HOME shell variable.\n\n\tThe variable CDPATH defines the search path for the directory containing DIR.\n\tAlternative directory names in CDPATH are separated by a colon (:).\n\tA null directory name is the same as the current directory.\n\tIf DIR begins with a slash (/), then CDPATH is not used.\n\n\tIf the directory is not found, and the shell option `cdable_vars' is set, the word is assumed to be  a variable name.\n\tIf that variable has a value, its value is used for DIR.\n\n\tReturns 0 if the directory is changed; non-zero otherwise.\n\n\n");
    }
    // case more than one argument for cd
    // (first argument in array is command)
    else if(argc > 2)
    {
        returnCode = -1;
        fprintf(stderr,"cd: Too many arguments\n");
        return returnCode;
    }
    else
    {
        returnCode = chdir(argv[1]);
    }

    if(returnCode != 0)
    {
        perror("cd");
    }

    return returnCode;
}

int pwd(char** argv, int argc)
{
    int returnCode;

    if((argv[1] != NULL) && (strcmp(argv[1],"--help") == 0))
    {
        returnCode = 0;
        printf("pwd: pwd\n\tPrint the name of the current working directory.\n\n\tExit Status:\n\tReturns 0 unless an invalid option is given or the current directory\n\tcannot be read.\n\n\n");
    }
    else
    {
        char buffer[1024];
        char* dir = getcwd(buffer,sizeof(buffer));

        printf("%s\n", dir);

        if(dir != NULL) returnCode = 0;
        else returnCode = -1;    
    }

    if(returnCode != 0)
    {
        perror("pwd");
    }

    return returnCode;
}

int echo(char** argv, int argc)
{
    int returnCode = 0;

    for(int i = 1; i < argc; i++)
    {
        //if((argv[i] != NULL) && (contains(argv[i],"\\"))) argv[i] = str_replace("\\","",argv[i]);
        for(int j = 0; j < strlen(argv[i]); j++)
        {
            if(argv[i][j] == '\\')
            {
                printf("%c",argv[i][j+1]);
                j++;
                continue;
            }
            printf("%c",argv[i][j]);
        }
        printf(" ");
    }

    printf("\n");
    
    return returnCode;
}