#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "../models.h"
#include "../data_api.h"

void FreeProduct(void *pProduct);
void ReadProducts(GenericWrapper *pw, char *filename);
void DisplayProduct(Product *product);
void DisplayProducts(GenericWrapper *pw);
void *GetProductAt(GenericWrapper *pw, size_t index);

#endif // PRODUCTS_H