#include <stdio.h>
#include <stdlib.h>
#include "db/data_api.h"
#include "db/api/products.h"
#include "db/api/quotes.h"
#include "db/api/product_quotes.h"
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
    // void function pointer that takes a void pointer as parameter and returns a pointer to void data...
    pw.getElementAt = (void * (*)(void *, size_t))GetProductAt;

    ReadProducts(&pw, "data/products.csv");
    LogInfo("Products read successfully");
    LogInfo("Displaying products");

    //DisplayProducts(&pw);
    LogInfo("Products displayed successfully");

    GenericWrapper qw;
    Quote *quotes = NULL;
    qw.data = quotes;
    qw.freeData = (void (*)(void *))FreeQuote;

    qw.getElementAt = (void * (*)(void *, size_t))GetQuoteAt;
    ReadQuotes(&qw, "data/quotes.csv");
    LogInfo("Quotes read successfully");

    LogInfo("Displaying quotes");
    //DisplayQuotes(&qw);

    GenericWrapper *productQuoteWrapper = GroupByProduct(&pw, &qw);
    LogInfo("Grouping products by quotes");
    DisplayProductsWithQuotes(productQuoteWrapper);

    LogInfo("Freeing memory");
    FreeWrapper(&qw);
    FreeWrapper(&pw);
    FreeWrapper(productQuoteWrapper);
    FreeMemory((void **)&productQuoteWrapper);
    LogInfo("Memory freed successfully");
    LogInfo("Ending the program");

    return 0;
}