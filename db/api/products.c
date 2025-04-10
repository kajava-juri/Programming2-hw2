#include <stdio.h>
#include <stdlib.h>
#include "products.h"
#include "utils.h"

void FreeProduct(Product *product)
{
    if (product == NULL) {
        return;
    }
    FreeMemory((void **)&product->name);
    FreeMemory((void **)&product->operating_system);
    FreeMemory((void **)&product);
}