#include "commands.h"

/**
 * Prints the command lines starter
 */
void print_prompt()
{
    printf("my_sh > ");
}

/**
 * Parse the text of the commandLine
 * @param command command line to parse
 */ 
void parseCommand(char* command)
{   

    // Returns first token 
    char *token = strtok(command, " ");
   
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
    //Début d'amélioration pour le parser
    /*
    char* word;
    int len = 0;
    printf("Caractère actuel");
    
    while(*command != '\0')
    {
        printf("Caractère actuel : %c", *command);
        if(*command != ' ') 
        {
            word[len] = *command;
            word[len+1] = '\0';
            len = strlen(word);
        }
        else{
            word[len+1] = '\0';
            printf("%s\n", word);
            len = 0;
            word = "";
        }
        
        *command++; 
    } 
    */
}

/**
 * Function parsing words by spaces
 * @param str string to parse in words
 */
void parseSpace(char* str)
{
    
}

/**
 * Clean result of fgets, remove "\n" and flush stdin
 * @param buffer string to clean
 */ 
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

/**
 * Reads a string as a command
 * @param command string to be read as a command
 */
void readCommand(char* command)
{
    int commandLength = strlen(command);

    if(commandLength > 255)
    {
        printf("Your command exceeds 255 characters. Please enter a shorter command.");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Your entered command: %s\n", stringToLower(command));
    }
}