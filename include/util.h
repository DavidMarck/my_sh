#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
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
int contains(char* str, char* seq);

/**
 * Replaces all found instances of the passed substring in the passed string.
 *
 * @param search The substring to look for
 * @param replace The substring with which to replace the found substrings
 * @param subject The string in which to look
 *
 * @return A new string with the search/replacement performed
 **/
char* str_replace(char* search, char* replace, char* subject);

#endif