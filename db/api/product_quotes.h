#ifndef PRODUCT_QUOTES_H
#define PRODUCT_QUOTES_H

#include "../data_api.h"
#include "products.h"
#include "quotes.h"

typedef struct {
    Product *product;
    // Array of pointers to original Quote objects
    Quote **quotes;
    size_t quote_count; // Number of quotes in the array
} ProductQuote;

/**
 * @brief Returns a pointer to the ProductQuote at the specified index.
*/
void *GetProductQuoteAt(GenericWrapper *pw, size_t index);

/**
 * @brief Frees the memory allocated for a ProductQuote.
 * 
 * @param pq Pointer to the ProductQuote to be freed
 */
void FreeProductQuote(void *pq);

/**
 * Display all products with their associated quotes grouped together.
 * Each product is displayed once, followed by all quotes associated with it.
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 */
void DisplayProductsWithQuotes(GenericWrapper *productQuoteWrapper);

/**
 * Edit a product's screen size by its product code.
 * Updates both the in-memory structure and writes changes to the file.
 * 
 * @param products The GenericWrapper containing all products
 * @param productCode The product code to identify the product to edit
 * @param newScreenSize The new screen size value
 * @param filename The name of the file to save changes to
 * @return 1 if successful, 0 if product not found or other error
 */
int EditProductScreenSize(GenericWrapper *products, const char *productCode, 
                          float newScreenSize, const char *filename);

/**
 * Edit a quote's availability status by its quote ID.
 * Updates both the in-memory structure and writes changes to the file.
 * 
 * @param quotes The GenericWrapper containing all quotes
 * @param quoteId The quote ID to identify the quote to edit
 * @param newAvailability The new availability status
 * @param filename The name of the file to save changes to
 * @return 1 if successful, 0 if quote not found or other error
 */
int EditQuoteAvailability(GenericWrapper *quotes, const char *quoteId, 
                          AvailabilityStatus newAvailability, const char *filename);

/**
 * Search for products by store name.
 * Returns all products available in stock at the specified store, 
 * ordered by RAM amount.
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 * @param storeName The name of the store to search for
 * @param results A GenericWrapper to store the search results
 * @return The number of products found
 */
int SearchProductsByStore(GenericWrapper *products, GenericWrapper *quotes, 
                          const char *storeName, GenericWrapper *results);

/**
 * Write all products to the specified CSV file.
 * 
 * @param products The GenericWrapper containing all products
 * @param filename The name of the file to save to
 * @return 1 if successful, 0 if error occurred
 */
int WriteProductsToFile(GenericWrapper *products, const char *filename);

/**
 * Write all quotes to the specified CSV file.
 * 
 * @param quotes The GenericWrapper containing all quotes
 * @param filename The name of the file to save to
 * @return 1 if successful, 0 if error occurred
 */
int WriteQuotesToFile(GenericWrapper *quotes, const char *filename);

/**
 * Group products by their associated quotes.
 * 
 * @param products The GenericWrapper containing all products
 * @param quotes The GenericWrapper containing all quotes
 * @return A new GenericWrapper containing grouped ProductQuote objects
 */
GenericWrapper *GroupByProduct(GenericWrapper *products, GenericWrapper *quotes);

#endif // PRODUCT_QUOTES_H