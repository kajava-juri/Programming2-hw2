#include "product_quotes.h"
#include "../../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GenericWrapper *GroupByProduct(GenericWrapper *products, GenericWrapper *quotes)
{
    if (products == NULL || quotes == NULL)
    {
        return NULL;
    }
    ProductQuote *productQuotes = malloc(products->used * sizeof(ProductQuote));
    if (productQuotes == NULL)
    {
        LogError("Failed to allocate memory for product quotes");
        exit(EXIT_FAILURE);
    }

    GenericWrapper *productQuoteWrapper = malloc(sizeof(GenericWrapper));
    if (productQuoteWrapper == NULL)
    {
        FreeMemory((void **)&productQuotes);
        LogError("Failed to allocate memory for product quote wrapper");
        exit(EXIT_FAILURE);
    }

    // Initialize the productQuoteWrapper
    productQuoteWrapper->data = productQuotes;
    productQuoteWrapper->size = sizeof(ProductQuote);
    productQuoteWrapper->used = products->used;
    productQuoteWrapper->limit = products->used;
    productQuoteWrapper->freeData = (void (*)(void *))FreeProductQuote;
    productQuoteWrapper->getElementAt = (void *(*)(void *, size_t))GetProductQuoteAt;

    for (size_t i = 0; i < products->used; i++)
    {
        Product *product = (Product *)products->getElementAt(products, i);
        if (product == NULL)
        {
            continue;
        }

        // search for quotes associated with the product id
        char *productCode = product->product_code;
        GenericWrapper *associatedQuotes = malloc(sizeof(GenericWrapper));
        InitQuoteWrapper(associatedQuotes);

        int count = 0;
        int allocated = 0;
        for (size_t j = 0; j < quotes->used; j++)
        {
            Quote *quote = (Quote *)quotes->getElementAt(quotes, j);
            if (quote == NULL || strcmp(quote->product_code, productCode) != 0)
            {
                continue;
            }
            // add the quote to the associated quotes

            // realloc n + 10
            if (count >= allocated)
            {
                allocated += 10; // enough for this task
                Quote *qTemp = realloc(associatedQuotes->data, (unsigned)allocated * sizeof(Quote));
                if (qTemp == NULL)
                {
                    FreeWrapper(associatedQuotes);
                    FreeMemory((void **)&associatedQuotes);
                    FreeWrapper(productQuoteWrapper);
                    FreeMemory((void **)&productQuoteWrapper);

                    FreeMemory((void **)&productQuotes);

                    LogError("Failed to allocate memory for quotes");
                    exit(EXIT_FAILURE);
                }

                // reassign the data pointer to new bigger memory
                associatedQuotes->data = qTemp;
            }

            // copy the quote to the associated quotes array
            Quote *dest = (Quote *)associatedQuotes->data + count;
            // deep copy the quote data
            // alternatively could use an array of pointers in ProductQuote and data copying wont be needed
            memcpy(dest, quote, sizeof(Quote));
            dest->retailer = strdup(quote->retailer);
            strcpy(dest->quote_id, quote->quote_id);
            strcpy(dest->product_code, quote->product_code);
            dest->availability = quote->availability;
            dest->price = quote->price;
            
            count++;
        } // END INNER QUOTES LOOP
        associatedQuotes->used = count;
        associatedQuotes->limit = allocated;
        // POSSIBLE ISSUE?: not a deep copy
        productQuotes[i].product = product;
        productQuotes[i].quotes = associatedQuotes;

    } // END PRODUCT LOOP

    return productQuoteWrapper;
}

void FreeProductQuote(void *pq)
{
    if (pq == NULL)
    {
        return;
    }
    ProductQuote *pProductQuote = (ProductQuote *)pq;
    
    // Don't free the product pointer as it points to the original product
    // that will be freed elsewhere
    
    // Free the quotes wrapper and its contents
    if (pProductQuote->quotes != NULL)
    {
        FreeWrapper(pProductQuote->quotes);
        FreeMemory((void **)&pProductQuote->quotes);
    }
}

void *GetProductQuoteAt(GenericWrapper *pw, size_t index)
{
    if (pw == NULL || pw->data == NULL)
    {
        return NULL;
    }
    if (index >= pw->used)
    {
        return NULL;
    }

    ProductQuote *pProductQuote = (ProductQuote *)pw->data + index;
    return (void *)pProductQuote;
}

void DisplayProductsWithQuotes(GenericWrapper *productQuoteWrapper)
{
    ProductQuote *productQuotes = (ProductQuote *)productQuoteWrapper->data;
    for (size_t i = 0; i < productQuoteWrapper->used; i++)
    {
        ProductQuote *pq = &productQuotes[i];
        if (pq == NULL)
        {
            continue;
        }
        DisplayProduct(pq->product);
        printf("Associated Quotes:\n");
        for (size_t j = 0; j < pq->quotes->used; j++)
        {
            Quote *quote = (Quote *)pq->quotes->getElementAt(pq->quotes, j);
            if (quote == NULL)
            {
                continue;
            }
            printf("    Quote ID: %s\n", quote->quote_id);
            printf("    Product Code: %s\n", quote->product_code);
            printf("    Store: %s\n", quote->retailer);
            printf("    Price: %.2f\n", quote->price);
            printf("    Availability: %d\n", GetAvailabilityString(quote->availability));
            printf("    ---------------\n");
        }
        printf("===============\n");
    }
}

int EditProductScreenSize(GenericWrapper *products, const char *productCode,
                          float newScreenSize, const char *filename)
{
    // Implementation to be filled in
    return 0;
}

int EditQuoteAvailability(GenericWrapper *quotes, const char *quoteId,
                          AvailabilityStatus newAvailability, const char *filename)
{
    // Implementation to be filled in
    return 0;
}

int SearchProductsByStore(GenericWrapper *products, GenericWrapper *quotes,
                          const char *storeName, GenericWrapper *results)
{
    // Implementation to be filled in
    return 0;
}

int WriteProductsToFile(GenericWrapper *products, const char *filename)
{
    // Implementation to be filled in
    return 0;
}

int WriteQuotesToFile(GenericWrapper *quotes, const char *filename)
{
    // Implementation to be filled in
    return 0;
}