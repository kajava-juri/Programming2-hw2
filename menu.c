#include "menu.h"
#include "db/api/product_quotes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DisplayMainMenu() {
    printf("Main Menu:\n");
    printf("1. Display Products grouped by Quotes\n");
    printf("2. Edit Product or Quote\n");
    printf("3. Search by Store\n");
    printf("4. Exit\n");
}

int ProcessMenuSelection(GenericWrapper *products, GenericWrapper *quotes, 
                        const char *productsFile, const char *quotesFile)
{
    
    DisplayMainMenu();
    int menuOption;
    printf("Select an option: ");
    do
    {
        scanf("%d", &menuOption);
        if (menuOption < 1 || menuOption > 4)
        {
            printf("Invalid option. Please try again.\n");
        }
    }
    while (menuOption < 1 || menuOption > 4);

    // Clear the input buffer
    while (getchar() != '\n');
    
    switch (menuOption)
    {
        case 1:
            MenuDisplayProductsWithQuotes(products, quotes);
            break;
        case 2:
            MenuEditProductOrQuote(products, quotes, productsFile, quotesFile);
            break;
        case 3:
            MenuSearchByStore(products, quotes);
            break;
        case 4:
            printf("Exiting...\n");
            return 0; // Exit the program
    }
    return 0;
}

void MenuDisplayProductsWithQuotes(GenericWrapper *products, GenericWrapper *quotes)
{
    // Implementation to be filled in
}

void MenuEditProductOrQuote(GenericWrapper *products, GenericWrapper *quotes, 
                          const char *productsFile, const char *quotesFile)
{
    // Implementation to be filled in
}

void MenuSearchByStore(GenericWrapper *products, GenericWrapper *quotes)
{
    // Implementation to be filled in
}