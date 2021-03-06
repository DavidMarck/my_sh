#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "var.h"
#include "typedef.h"

/**
 * Returns a string in lower case
 * 
 * @param string string to be returned in lower case
 * 
 * @return string in lower-case
 */
char* string_to_lower(char*);

/**
 * Indicates wether or not a string contains a certain sequence
 * 
 * @ param str string in which the sequence has to be searched
 * @ param seq sequence to look for in string
 * 
 * @return true or false
 */
int contains(char* str, char* seq);

/**
 * Indicates wether or not a string represents a signed or unsigner numeric
 * 
 * @param s string to test
 * 
 * @return true if string is numeric, false otherwise
 */
int isNumeric (const char * s);

/**
 * Converts a string to an integer
 * 
 * @param str string to convert
 * 
 * @return resulting integer
 */
int strtonum(const char * str);

/**
 * Count occurences of a certain character in a string
 * 
 * @param str string to look into
 * @param search char seq to find and count
 * 
 * @return count
 */
int strcount(char* str, char* search);

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

/**
 * Inserts a string into another string at a given position
 * 
 * @param str
 * @param insert
 * @param position
 */
void insert_substring(char *str, char *insert, int position);

/**
 * Gets substring
 * 
 * @param string string from which we want a substring
 * @param position start index
 * @param length end index from position
 * 
 * @return substring
 */
char *substring(char *string, int position, int length);

/**
 * Replaces the value of an index into a char**
 * 
 * @param array the array to change
 * @param size the size of the array
 * @param index_to_replace the index of the array to change
 * @param string the new string to put into the index
 * 
 * @return a new array with the modifications
 */
char** str_array_replace(char** array, int size, int index_to_replace, char* string);

/**
 * Free a char**
 * 
 * @param array the array to free
 * @param size the size of the array
 */
void free_array(char** array, int array_size);

#endif
