#include <stdio.h>
#include <stdlib.h>
#include "db/data_api.h"
#include "db/api/products.h"
#include "db/api/quotes.h"
#include "db/api/product_quotes.h"
#include "utils.h"
#include "logger.h"
#include "menu.h"

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
    //DisplayProductsWithQuotes(productQuoteWrapper);

    // Declare option variable before the loop
    int option;
    // Get menu selection and check if it's not 0
    while ((option = GetMenuSelection()) != 0)
    {
        switch (option) // Use option as the switch expression
        {
        case 1:
            DisplayProductsWithQuotes(productQuoteWrapper);
            break;
        
        case 2:
            MenuEditProductOrQuote(&pw, &qw);
            break;
            
        case 3:
            
            break;
            
        default:
            printf("Invalid option: %d\n", option);
            break;
        }
    }

    LogInfo("Freeing memory");
    FreeWrapper(&qw);
    FreeWrapper(&pw);
    FreeWrapper(productQuoteWrapper);
    FreeMemory((void **)&productQuoteWrapper);
    LogInfo("Memory freed successfully");
    LogInfo("Ending the program");

    return 0;
}