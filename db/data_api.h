#ifndef DATA_API_H
#define DATA_API_H
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "models.h"

#define MAX_PRODUCTS 100
#define MAX_QUOTES 100

typedef struct {
    void *data;
    void (*freeData)(void *); // a function provided that frees the data
    size_t size;
    size_t used;
    size_t limit;
} GenericWrapper;

void FreeWrapper(GenericWrapper *wrapper);

#endif // DATA_API_H