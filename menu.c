#include "menu.h"
#include "main.h"
#include "db/api/product_quotes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DisplayMainMenu() {
    printf("\n==========================================================\n");
    printf("                  P R I C E    W A T C H                   \n");
    printf("==========================================================\n\n");
    printf("  1. Display Products grouped by Quotes\n");
    printf("  2. Edit Product or Quote\n");
    printf("  3. Search by Store\n");
    printf("  4. Exit\n\n");
}

int GetMenuSelection()
{
    DisplayMainMenu();
    
    int menuOption;
    do
    {
        printf("  Select an option (1-4): ");
        scanf("%d", &menuOption);
        if (menuOption < 1 || menuOption > 4)
        {
            printf("  Invalid option. Please select a number between 1 and 4.\n");
            // Clear the input buffer
            while(getchar() != '\n' && getchar() != EOF);
        }
    }
    while (menuOption < 1 || menuOption > 4);
    
    if(menuOption == 4)
    {
        printf("\n==========================================================\n");
        printf("                        E X I T I N G                     \n");
        printf("==========================================================\n\n");
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
    printf("\n==========================================================\n");
    printf("              E D I T   P R O D U C T / Q U O T E         \n");
    printf("==========================================================\n\n");
    printf("Enter product code (SP###) or quote ID (Q###): ");
    
    char userInput[BUF_LEN];
    do 
    {
        scanf("%s", userInput);
        
        if(strlen(userInput) < 2)
        {
            printf("  Invalid input. Please try again.\n");
            printf("Enter product code (SP###) or quote ID (Q###): ");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) // Clear the input buffer
                continue;
        }
        else if (userInput[0] == 'Q')
        {
            printf("  Searching for quote with ID: %s\n", userInput);
            
            // Check if input matches format Q000
            if (strlen(userInput) != 4 || 
                userInput[1] < '0' || userInput[1] > '9' || 
                userInput[2] < '0' || userInput[2] > '9' || 
                userInput[3] < '0' || userInput[3] > '9') {
                printf("\n  Invalid quote format. Should be Q followed by 3 digits.\n\n");
                printf("Enter product code (SP###) or quote ID (Q###): ");
                continue;
            }

            Quote *pQuote = GetQuoteByCode(quotes, userInput);
            if(pQuote == NULL)
            {
                printf("\n  Quote not found.\n\n");
                printf("Enter product code (SP###) or quote ID (Q###): ");
                continue;
            }
            
            printf("\n  Quote found:\n");
            printf("  ----------------------------------------------------------\n");
            printf("    Quote ID:      %s\n", pQuote->quote_id);
            printf("    Product Code:  %s\n", pQuote->product_code);
            printf("    Retailer:      %s\n", pQuote->retailer);
            printf("    Price:         %.2f\n", pQuote->price);
            printf("    Availability:  %s\n", GetAvailabilityString(pQuote->availability));
            printf("  ----------------------------------------------------------\n\n");
            
            // Edit Quote availability 
            printf("  Enter new availability:\n");
            printf("    1. In Stock\n");
            printf("    2. Out of Stock\n");
            printf("  New availability: ");
            
            int availabilityOption;
            scanf("%d", &availabilityOption);
            
            // Keep asking until a valid option is entered
            while (availabilityOption != 1 && availabilityOption != 2)
            {
                printf("  Invalid option. Please enter 1 for In Stock or 2 for Out of Stock: ");
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
            if (ApplyQuoteEdits(pQuote, userInput, QUOTE_FILE) == 1)
            {
                printf("\n  Quote availability updated successfully.\n\n");
            }
            else
            {
                printf("\n  Failed to update quote availability.\n\n");
            }
        }
        else if(userInput[0] == 'S' && userInput[1] == 'P')
        {
            printf("  Searching for product with code: %s\n", userInput);
            
            // Check if input matches format SP000
            if (strlen(userInput) != 5 ||
                userInput[2] < '0' || userInput[2] > '9' ||
                userInput[3] < '0' || userInput[3] > '9' || 
                userInput[4] < '0' || userInput[4] > '9') {
                printf("\n  Invalid product format. Should be SP followed by 3 digits.\n\n");
                printf("Enter product code (SP###) or quote ID (Q###): ");
                continue;
            }

            Product *pProduct = GetProductByCode(products, userInput);
            if (pProduct == NULL)
            {
                printf("\n  Product not found.\n\n");
                printf("Enter product code (SP###) or quote ID (Q###): ");
                continue;
            }
            
            printf("\n  Product found:\n");
            printf("  ----------------------------------------------------------\n");
            printf("    Product Code:  %s\n", pProduct->product_code);
            printf("    Name:          %s\n", pProduct->name);
            printf("    RAM:           %d MB\n", pProduct->ram_mb);
            printf("    Screen Size:   %.2f inches\n", pProduct->screen_size_inches);
            printf("    OS:            %s\n", pProduct->operating_system);
            printf("  ----------------------------------------------------------\n\n");
            
            // Edit Product
            printf("  Current screen size: %.2f inches\n", pProduct->screen_size_inches);
            printf("  Enter new screen size: ");
            scanf("%f", &pProduct->screen_size_inches);
            
            // Edit the product screen size
            if (ApplyProductEdits(pProduct, userInput, PRODUCT_FILE) == 1)
            {
                printf("\n  Product screen size updated successfully to %.2f inches.\n\n", pProduct->screen_size_inches);
            }
            else
            {
                printf("\n  Failed to update product screen size.\n\n");
            }
        }
        else {
            printf("\n----------------------------------------------------------\n");
            printf("  Invalid input format. Enter SP### for products or Q### for quotes.\n");
            printf("----------------------------------------------------------\n\n");
            printf("Enter product code (SP###) or quote ID (Q###): ");
        }
    } while (strlen(userInput) < 2);
}

void MenuSearchByStore(GenericWrapper *products, GenericWrapper *quotes)
{
    char storeName[MAX_RETAILER_LEN + 1];
    
    printf("\n==========================================================\n");
    printf("           S E A R C H   B Y   S T O R E                  \n");
    printf("==========================================================\n\n");
    
    printf("Enter store name to search: ");
    
    // Use fgets to read the whole line including spaces
    fgets(storeName, MAX_RETAILER_LEN, stdin);
    
    // Remove the trailing newline character if present
    size_t len = strlen(storeName);
    if (len > 0 && storeName[len-1] == '\n') {
        storeName[len-1] = '\0';
    }

    // Create a temporary wrapper to hold search results if needed elsewhere
    GenericWrapper results = {0};
    
    // Search for products by store name and get the count
    int foundCount = SearchProductsByStore(products, quotes, storeName, &results);
    
    if (foundCount <= 0) {
        printf("  No products found in stock at store '%s'.\n", storeName);
        return;
    }
    
    printf("\n==========================================================\n");
    printf("  Found %d products in stock at '%s'\n", foundCount, storeName);
    printf("  Ordered by RAM (highest to lowest)\n");
    printf("==========================================================\n");
    
    // Display each product in the results
    for (size_t i = 0; i < results.used; i++) {
        // Get the pointer to the product
        Product *product = ((Product **)results.data)[i];
        if (product != NULL) {
            printf("\n  Product #%zu:\n", i+1);
            printf("  ----------------------------------------------------------\n");
            printf("    Product Code: %s\n", product->product_code);
            printf("    Name:         %s\n", product->name);
            printf("    RAM:          %d MB\n", product->ram_mb);
            printf("    Screen Size:  %.2f inches\n", product->screen_size_inches);
            printf("    OS:           %s\n", product->operating_system);
            printf("  ----------------------------------------------------------\n");
        }
    }
    printf("\n");
    
    // Free the results data 
    FreeMemory((void **)&results.data);
}