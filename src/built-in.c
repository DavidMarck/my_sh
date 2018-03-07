#include "built-in.h"

int cd(char** argv, int argc)
{
    int returnCode;

    // case cd with no argument
    if(argc == 1)
    {
        returnCode = chdir(getenv("HOME"));
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
        if(returnCode == ENOENT)
        {
            fprintf(stderr,"cd: No such file or directory\n");
        }
        else
        {
            perror("cd");
        }
    }

    return returnCode;
}