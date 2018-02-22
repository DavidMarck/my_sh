#include "../include/util.h"

/**
 * Returns a string in lower case
 * @param string string to be returned in lower case
 */
char* stringToLower(char* string)
{
    for(int i = 0; i = strlen(string); i++)
    {
        string[i] = tolower(string[i]);
    }

    return string;
}