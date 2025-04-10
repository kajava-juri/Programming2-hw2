#include "utils.h"
#include <stdlib.h>

void FreeMemory(void **p)
{
    if (*p) {
        free(*p);
        *p = NULL;
    }
}