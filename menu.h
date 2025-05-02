#ifndef MENU_H
#define MENU_H

#include "db/data_api.h"

/**
 * Display the main menu options
 */
void DisplayMainMenu();

/**
 * Process the user's menu selection
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 * @param productsFile The filename for products data
 * @param quotesFile The filename for quotes data
 * @return 1 to continue, 0 to exit
 */
int ProcessMenuSelection(GenericWrapper *products, GenericWrapper *quotes, 
                        const char *productsFile, const char *quotesFile);

/**
 * Display and handle the "Display Products with Quotes" menu option
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 */
void MenuDisplayProductsWithQuotes(GenericWrapper *products, GenericWrapper *quotes);

/**
 * Display and handle the "Edit Product/Quote" menu option
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 * @param productsFile The filename for products data
 * @param quotesFile The filename for quotes data
 */
void MenuEditProductOrQuote(GenericWrapper *products, GenericWrapper *quotes, 
                          const char *productsFile, const char *quotesFile);

/**
 * Display and handle the "Search by Store" menu option
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 */
void MenuSearchByStore(GenericWrapper *products, GenericWrapper *quotes);

#endif // MENU_H