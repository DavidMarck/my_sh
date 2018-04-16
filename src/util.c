#include "util.h"
#include <stdio.h>

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
    return FALSE;
}

int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

int strtonum(const char * str)
{
	int  i, len;

	int result = 0;

	len = strlen(str);

	for(i = 0; i < len; i++)
	{
		result = result * 10 + ( str[i] - '0' );
	}

	return result;
}

int strcount(char* str, char* search)
{
	int i, j, found, count;
    int stringLen, searchLen;

    stringLen = strlen(str);      // length of string
    searchLen = strlen(search); // length of word to be searched

    count = 0;

    for(i=0; i <= stringLen-searchLen; i++)
    {
        /* Match word with string */
        found = 1;
        for(j=0; j<searchLen; j++)
        {
            if(str[i + j] != search[j])
            {
                found = 0;
                break;
            }
        }

        if(found == 1)
        {
            count++;
        }
    }

    return count;
}

char* str_replace(char* search, char* replace, char* subject) {
	int i, j, k;
	
	int searchSize = strlen(search);
	int replaceSize = strlen(replace);
	int size = strlen(subject);

	char* ret;

	if (!searchSize) {
		ret = malloc(size + 1);
		for (i = 0; i <= size; i++) {
			ret[i] = subject[i];
		}
		return ret;
	}
	
	int retAllocSize = (strlen(subject) + 1) * 2; // Allocation size of the return string.
    // let the allocation size be twice as that of the subject initially
	ret = malloc(retAllocSize);

	int bufferSize = 0; // Found characters buffer counter
	char* foundBuffer = malloc(searchSize); // Found character bugger
	
	for (i = 0, j = 0; i <= size; i++) {
		/**
         * Double the size of the allocated space if it's possible for us to surpass it
		 **/
		if (retAllocSize <= j + replaceSize) {
			retAllocSize *= 2;
			ret = (char*) realloc(ret, retAllocSize);
		}
		/**
         * If there is a hit in characters of the substring, let's add it to the
         * character buffer
		 **/
		else if (subject[i] == search[bufferSize]) {
			foundBuffer[bufferSize] = subject[i];
			bufferSize++;

			/**
             * If the found character's bugger's counter has reached the searched substring's
             * length, then there's a hit. Let's copy the replace substring's characters
             * onto the return string.
			 **/
			if (bufferSize == searchSize) {
				bufferSize = 0;
				for (k = 0; k < replaceSize; k++) {
					ret[j++] = replace[k];
				}
			}
		}
		/**
         * If the character is a miss, let's put everything back from the buffer
         * to the return string, and set the found character buffer counter to 0.
		 **/
		else {
			for (k = 0; k < bufferSize; k++) {
				ret[j++] = foundBuffer[k];
			}
			bufferSize = 0;
			/**
             * Add the current character in the subject string to the return string.
			 **/
			ret[j++] = subject[i];
		}
	}

	/**
	 * Free memory
	 **/
	free(foundBuffer);
	
	return ret;
}

void insert_substring(char *str, char *insert, int position)
{
   char *f, *e;
   int length;
 
   length = strlen(str);
 
   f = substring(str, 1, position - 1 ); 
   e = substring(str, position, length-position+1);
 
   strcpy(str, "");
   strcat(str, f);
   free(f);
   strcat(str, insert);
   strcat(str, e);
   free(e);
}
 
char *substring(char *string, int position, int length) 
{
   char *pointer;
   int c;
 
   pointer = malloc(length+1);
 
   if( pointer == NULL )
       exit(EXIT_FAILURE);
 
   for( c = 0 ; c < length ; c++ ) 
      *(pointer+c) = *((string+position-1)+c);       
 
   *(pointer+c) = '\0';
 
   return pointer;
}

char** str_array_replace(char** array, int size, int index_to_replace, char* string) 
{
	char** newArray  = NULL; // args array
	int index = 0;
	newArray = malloc (sizeof (char*) * (size+1));
	if (newArray == NULL) exit (-1); /* memory allocation failed */ 
		
	char* tmp = NULL;
	// we browse the old array
	while(index < size)
	{
		// if we aren't at the specified index, we just get the same element
		if(index != index_to_replace)
		{
			tmp = malloc(sizeof(char)*strlen(array[index]));
			*tmp ='\0'; 
			tmp = strcpy(tmp,array[index]);
		}
		// else we get the new string
		else 
		{
			tmp = malloc(sizeof(char)*strlen(string));
			*tmp = '\0';
			tmp = strcpy(tmp,string);
		}
		
		// we add the right element to the new array
		newArray[index] = tmp;
		index++;	
	}	
	
	// we add the null, and we free the old array before to return the new one
	newArray[index] = 0;
	//free(array);

	return newArray;	
}

void free_array(char** array, int array_size)
{
	// Here we only free the array pointer
	// (we tried to free each element but we don't know why, it dumps)
	// that's why function is unsused for the moment
    for(int i = 0; i < array_size; i++)
    {
        free(array[i]);
    }
    free(array);
}
