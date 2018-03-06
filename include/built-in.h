#ifndef BUILT_IN_H
#define BUILT_IN_H

#include <unistd.h>

/**
 * Sets new current working directory
 * @param target destination directory
 */
int cd(char* target);

#endif