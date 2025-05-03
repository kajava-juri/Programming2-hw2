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

int GetMenuSelection()
{
    
    DisplayMainMenu();
    int menuOption;
    do
    {
        printf("Select an option: ");
        scanf("%d", &menuOption);
        if (menuOption < 1 || menuOption > 4)
        {
            printf("Invalid option. Please try again.\n");
            // Clear the input buffer
            while(getchar() != '\n' && getchar() != EOF);
        }
    }
    while (menuOption < 1 || menuOption > 4);
    if(menuOption == 4)
    {
        return 0;
    }

    // Clear the input buffer
    while (getchar() != '\n');

    return menuOption;
}

void MenuDisplayProductsWithQuotes(GenericWrapper *productQuotes)
{
    if (productQuotes == NULL || productQuotes->data == NULL)
    {
        printf("No products or quotes available.\n");
        return;
    }
    DisplayProductsWithQuotes(productQuotes);
}

void MenuEditProductOrQuote(GenericWrapper *products, GenericWrapper *quotes)
{
    printf("Edit Product or Quote:\n");
    char userInput[BUF_LEN];
    do 
    {
        scanf("%s", userInput);
        //printf("You entered: %c\n", userInput[0]);
        printf("%c == Q: %d\n", userInput[0], userInput[0] == 'Q');
        if(strlen(userInput) < 2)
        {
            printf("Invalid input. Please try again.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) // Clear the input buffer
            continue;
        }
        if (userInput[0] == 'Q')
        {
            printf("Searching for quote with ID: %s\n", userInput);
            // Check if input matches format Q000
            if (strlen(userInput) != 4 || 
                userInput[1] < '0' || userInput[1] > '9' || 
                userInput[2] < '0' || userInput[2] > '9' || 
                userInput[3] < '0' || userInput[3] > '9') {
                printf("Invalid quote format. Should be Q followed by 3 digits.\n");
                continue;
            }

            Quote *pQuote = GetQuoteByCode(quotes, userInput);
            if(pQuote == NULL)
            {
                printf("Quote not found.\n");
                continue;
            }
            printf("Quote found:\n");
            DisplayQuote(pQuote);
            // Edit Quote availability 
            char *newAvailability;
            printf("Enter new availability: \n"
            "1. In Stock    2. Out of Stock\n");
            int availabilityOption;
            scanf("%d", &availabilityOption);
            // Keep asking until a valid option is entered
            while (availabilityOption != 1 && availabilityOption != 2)
            {
                printf("Invalid option. Please enter 1 for In Stock or 2 for Out of Stock: ");
                scanf("%d", &availabilityOption);
                // Clear the input buffer
                while(getchar() != '\n' && getchar() != EOF);
            }

            // Update the quote's availability based on selection
            if (availabilityOption == 1)
            {
                pQuote->availability = IN_STOCK;
            }
            else if (availabilityOption == 2)
            {
                pQuote->availability = OUT_OF_STOCK;
            }

            // Apply the changes to the quote
            if (ApplyQuoteEdits(pQuote, userInput, "data/quotes.csv") == 1)
            {
                printf("Quote availability updated successfully.\n");
            }
            else
            {
                printf("Failed to update quote availability.\n");
            }
        }
        else if(userInput[0] == 'S' && userInput[1] == 'P')
        {
            printf("Searching for product with code: %s\n", userInput);
            // Check if input matches format SP000
            if (strlen(userInput) != 5 ||
                userInput[2] < '0' || userInput[2] > '9' ||
                userInput[3] < '0' || userInput[3] > '9' || 
                userInput[4] < '0' || userInput[4] > '9') {
                printf("Invalid product format. Should be SP followed by 3 digits.\n");
                continue;
            }

            Product *pProduct = GetProductByCode(products, userInput);
            if (pProduct == NULL)
            {
                printf("Product not found.\n");
                continue;
            }
            DisplayProduct(pProduct);
            
            // Edit Product
            float newScreenSize;
            printf("Enter new screen size: ");
            scanf("%f", &pProduct->screen_size_inches);
            // Edit the product screen size
            if (ApplyProductEdits(pProduct, userInput, "data/products.csv") == 0)
            {
                printf("Product screen size updated successfully.\n");
            }
            else
            {
                printf("Failed to update product screen size.\n");
            }
        }
    } while (strlen(userInput) < 2);
}

void MenuSearchByStore(GenericWrapper *products, GenericWrapper *quotes)
{
    // Implementation to be filled in
}