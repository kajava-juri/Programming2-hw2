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
void ReadQuotes(GenericWrapper *pw, char *filename);
void DisplayQuote(Quote *quote);
void DisplayQuotes(GenericWrapper *pw);
void *GetQuoteAt(GenericWrapper *pw, size_t index);
char *GetAvailabilityString(AvailabilityStatus status);
void InitQuoteWrapper(GenericWrapper *qw);

#endif // QUOTES_H