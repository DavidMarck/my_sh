#include "built-in.h"

int cd(char* target)
{
    return chdir(target);
}