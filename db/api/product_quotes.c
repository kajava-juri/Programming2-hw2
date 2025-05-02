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
            // Initialize with NULL values for empty products
            productQuotes[i].product = NULL;
            productQuotes[i].quotes = NULL;
            productQuotes[i].quote_count = 0;
            continue;
        }

        // search for quotes associated with the product id
        char *productCode = product->product_code;
        Quote **quotePtrs = malloc(sizeof(Quote **));
        if (quotePtrs == NULL)
        {
            FreeMemory((void **)&productQuotes);
            LogError("Failed to allocate memory for associated quotes wrapper");
            exit(EXIT_FAILURE);
        }

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
                Quote **qTemp = realloc(quotePtrs, (unsigned)allocated * sizeof(Quote *));
                if (qTemp == NULL)
                {
                    // Clean up memory
                    FreeWrapper(productQuoteWrapper);
                    FreeMemory((void **)&quotePtrs);
                    // Rest of cleanup code
                    LogError("Failed to reallocate memory for quotes");
                    exit(EXIT_FAILURE);
                }

                // reassign the data pointer to new bigger memory
                quotePtrs = qTemp;
            }

            // push the quote pointer to the end of the array

            Quote **dest = quotePtrs + count;
            *dest = quote;

            count++;
        } // END INNER QUOTES LOOP

        productQuotes[i].product = product;
        productQuotes[i].quotes = quotePtrs;
        productQuotes[i].quote_count = count;

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

    // Free the quotes array - but not the Quote objects themselves
    // as they point to quotes that will be freed elsewhere
    if (pProductQuote->quotes != NULL)
    {
        free(pProductQuote->quotes);
        pProductQuote->quotes = NULL;
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
        for (size_t j = 0; j < pq->quote_count; j++)
        {
            Quote *quote = pq->quotes[j];
            if (quote == NULL)
            {
                continue;
            }
            printf("    Quote ID: %s\n", quote->quote_id);
            printf("    Product Code: %s\n", quote->product_code);
            printf("    Store: %s\n", quote->retailer);
            printf("    Price: %.2f\n", quote->price);
            printf("    Availability: %s\n", GetAvailabilityString(quote->availability));
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