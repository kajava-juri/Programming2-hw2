#include <stdio.h>
#include <stdlib.h>
#include "db/data_api.h"
#include "db/api/products.h"
#include "db/api/quotes.h"
#include "db/api/product_quotes.h"
#include "utils.h"
#include "logger.h"
#include "menu.h"
#include "main.h"

int main(void)
{
    LogInitialize(LOG_INFO, "logs.txt");
    LogInfo("Starting the program");

    // Initialize the GenericWrapper for products
    GenericWrapper pw;
    Product *products = NULL;
    pw.data = products;
    pw.freeData = (void (*)(void *))FreeProduct;
    // void function pointer that takes a void pointer as parameter and returns a pointer to void data...
    pw.getElementAt = (void * (*)(void *, size_t))GetProductAt;

    // Initialize the GenericWrapper for quotes
    GenericWrapper qw;
    Quote *quotes = NULL;
    qw.data = quotes;
    qw.freeData = (void (*)(void *))FreeQuote;

    qw.getElementAt = (void * (*)(void *, size_t))GetQuoteAt;

    // Start reading products and quotes from the CSV files
    ReadProducts(&pw, PRODUCT_FILE);
    LogInfo("Products read successfully");
    LogInfo("Displaying products");

    //DisplayProducts(&pw);
    LogInfo("Products displayed successfully");

    ReadQuotes(&qw, QUOTE_FILE);
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
            MenuSearchByStore(&pw, &qw);
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