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