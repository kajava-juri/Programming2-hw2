#ifndef DATA_API_H
#define DATA_API_H
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define BUF_LEN 128
#define MAX_PRODUCTS 100
#define MAX_QUOTES 100

typedef enum {
    TYPE_PRODUCT,
    TYPE_QUOTE,
} DataType;

typedef struct {
    void *data;

    // These function must be implemented in the respective data types
    void (*freeData)(void *); // a function provided that frees the data
    void *(*getElementAt)(void *, size_t); // a function provided that gets the element at index

    size_t size;
    size_t used;
    size_t limit;
} GenericWrapper;

void FreeWrapper(GenericWrapper *wrapper);

#endif // DATA_API_H