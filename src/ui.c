#include "ui.h"

int print_prompt()
{
    char buffer[1024];
    char* dir = getcwd(buffer,sizeof(buffer));
    char* prompt = strcat(dir," > ");
    printf("\n");
    printf("%s",prompt);

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

void read_command_line(char* commandLine)
{
    if(contains(commandLine,"\""))
    {
        while((strcount(commandLine,"\"") % 2) != 0)
        {
            printf(" > ");
            char c[MAX_SIZE];
            fgets(c,sizeof(c),stdin);
            strcat(commandLine,c);
        }
    }

    commandLine[strlen(commandLine) - 1] = '\0';
}
