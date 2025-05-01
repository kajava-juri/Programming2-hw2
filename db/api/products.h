#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "../data_api.h"

// Define length constants first, before any includes
#define PRODUCT_CODE_LEN 5       // product codes "SP001"
#define MAX_NAME_LEN 50      // product names
#define MAX_OS_LEN 10        // "Android", "iOS" ...

// data from products.csv
typedef struct {
    char product_code[PRODUCT_CODE_LEN + 1];
    char *name;
    int ram_mb;
    float screen_size_inches;
    char *operating_system;
} Product;

void FreeProduct(void *pProduct);
void ReadProducts(GenericWrapper *pw, char *filename);
void DisplayProduct(Product *product);
void DisplayProducts(GenericWrapper *pw);
void *GetProductAt(GenericWrapper *pw, size_t index);

#endif // PRODUCTS_H