#include <stdio.h>
#include <stdlib.h>
#include "db/data_api.h"
#include "db/api/products.h"
#include "db/api/quotes.h"
#include "utils.h"
#include "logger.h"

int main(void)
{
    LogInitialize();
    LogInfo("Starting the program");

    GenericWrapper pw;
    Product *products = NULL;
    pw.data = products;
    pw.freeData = (void (*)(void *))FreeProduct;
    // void function pointer that takes a void pointer as parameter and returns a pointer to product data at index... looks weird
    pw.getElementAt = (void * (*)(void *, size_t))GetProductAt;

    ReadProducts(&pw, "data/products.csv");
    LogInfo("Products read successfully");
    LogInfo("Displaying products");

    DisplayProducts(&pw);
    LogInfo("Products displayed successfully");

    GenericWrapper qw;
    Quote *quotes = NULL;
    qw.data = quotes;
    qw.freeData = (void (*)(void *))FreeQuote;
    // void function pointer that takes a void pointer as parameter and returns a pointer to product data at index... looks weird
    qw.getElementAt = (void * (*)(void *, size_t))GetQuoteAt;
    ReadQuotes(&qw, "data/quotes.csv");
    LogInfo("Quotes read successfully");

    LogInfo("Displaying quotes");
    DisplayQuotes(&qw);

    LogInfo("Freeing memory");
    FreeWrapper(&qw);
    FreeWrapper(&pw);
    LogInfo("Memory freed successfully");
    LogInfo("Ending the program");

    return 0;
}