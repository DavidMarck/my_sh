#ifndef BUILT_IN_H
#define BUILT_IN_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/**
 * Sets new current working directory
 * @param argv array of arguments
 * @param argc arguments count
 */
int cd(char** argv, int argc);

#endif