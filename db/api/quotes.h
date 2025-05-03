#ifndef QUOTES_H
#define QUOTES_H

#include "../data_api.h"
#include "products.h"

#define QUOTE_ID_LEN 4   // quote IDs "Q001"
#define MAX_RETAILER_LEN 20  // retailer names
#define QUOTE_AVAILABILITY_LEN 20 // availability status

typedef enum {
    OUT_OF_STOCK = 0,
    IN_STOCK = 1
} AvailabilityStatus;

// data from quotes.csv
typedef struct {
    char quote_id[QUOTE_ID_LEN + 1];
    char product_code[PRODUCT_CODE_LEN + 1];  // Foreign key to Product
    char *retailer;
    float price;
    AvailabilityStatus availability;
} Quote;

void FreeQuote(void *pQuote);

/**
 * @brief Reads quote data from the csv file
 *
 * @param pw Wrapper to hold the quotes data and allocation information
 * @param filename The name of the file to read the quote data from
 *
 * @return Number of quotes read, or negative value on error
 */
void ReadQuotes(GenericWrapper *pw, char *filename);
void DisplayQuote(Quote *quote);
void DisplayQuotes(GenericWrapper *pw);
void *GetQuoteAt(GenericWrapper *pw, size_t index);

/**
 * @brief Get the string representation of the availability status
 *
 * @param status The availability status
 * @return Pointer to the string representation of the status
 */
char *GetAvailabilityString(AvailabilityStatus status);

/**
 * @brief Initializes a GenericWrapper for Quote data
 *
 * @param qw Pointer to the GenericWrapper to be initialized
 */
void InitQuoteWrapper(GenericWrapper *qw);

/**
 * @brief Get a Quote by its quote code
 *
 * @param pw Pointer to the GenericWrapper containing quotes
 * @param quoteCode The quote code to search for
 * @return Pointer to the Quote if found, NULL otherwise
 */
Quote *GetQuoteByCode(GenericWrapper *pw, const char *quoteCode);

#endif // QUOTES_H