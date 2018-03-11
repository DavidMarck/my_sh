#include "ui.h"

int print_prompt()
{
    char buffer[1024];
    char* dir = getcwd(buffer,sizeof(buffer));
    char* prompt = strcat(dir," > ");
    printf(prompt);

    return TRUE;
}

void clean(const char *buffer)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
        *p = 0;
    else
    {
        int c;
        while ((c = fgetc(stdin)) != '\n' && c != EOF);
    }
}

void read_command_line(char* buffer)
{
    fgets(buffer, sizeof(buffer), stdin);
    clean(buffer);
    
    // if(contains(buffer,"\""))
    // {
    //     while((strcount(buffer,"\"") % 2) != 0)
    //     {
    //         printf("yolo");
    //     }
    // }

    
}

int wait_for_delim(char* delim)
{
    return TRUE;
}