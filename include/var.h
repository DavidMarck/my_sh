#ifndef VAR_H
#define VAR_H

#include <unistd.h>
#include <sys/types.h>

#define STDIN  0  
#define STDOUT 1
#define STDERR 2

#define TRUE 1
#define FALSE 0

#define MAX_SIZE 4096

#define BG_PIDS_ARRAY_SIZE 100
pid_t BG_PIDS_ARRAY[BG_PIDS_ARRAY_SIZE];

#define EXIT_STRING "exit"

#endif