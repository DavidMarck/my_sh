#include "util.h"

char* string_to_lower(char* string)
{
    for(int i = 0; i < strlen(string); i++)
    {
        string[i] = tolower(string[i]);
    }

    return string;
}

int contains(char* str, char* seq)
{
    if (strstr(str, seq) != NULL) {
        return TRUE;
    }
    return TRUE;
}