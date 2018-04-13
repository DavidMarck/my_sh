#include "history.h"

char* get_history_path()
{
    char* history_path = malloc(1024 * sizeof(char));
    *history_path = '\0';
    strcat(history_path,getenv("HOME"));
    strcat(history_path,"/tmp/commands_history.txt");

    return history_path;
}

int write_to_history(char* commandLine)
{
    FILE * fp;

    char* history_path= get_history_path();

    if((fp = fopen(history_path, "a")) == NULL)
    {
        perror("history file open");
        return -1;
    }

    fprintf(fp, "%s\n", commandLine);
    
    fclose(fp);

    free(history_path);
    
    return 0;
}

void clear_history()
{
    FILE * fp;

    char* history_path= get_history_path();

    if((fp = fopen (history_path, "w")) == NULL)
    {
        perror("history file open");
        return;
    }

    fclose(fp);

    free(history_path);
}