#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <ctype.h>

#include "typedef.h"

/**
 * Returns a string in lower case
 * @param string string to be returned in lower case
 */
char* string_to_lower(char*);

/**
 * Returns true whether a string contains a certain sequence or not
 * @ param str string in which the sequence has to be searched
 * @ param seq sequence to look for in string
 */
int contains(char*, char*);

#endif