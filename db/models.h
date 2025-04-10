#ifndef MODELS_H
#define MODELS_H

#define CODE_LEN 5       // product codes "SP001"
#define MAX_NAME_LEN 50      // product names
#define MAX_OS_LEN 10        // "Android", "iOS" ...
#define QUOTE_ID_LEN 4   // quote IDs "Q001"
#define MAX_RETAILER_LEN 20  // retailer names

typedef enum {
    OUT_OF_STOCK = 0,
    IN_STOCK = 1
} AvailabilityStatus;

// data from products.csv
typedef struct {
    char product_code[CODE_LEN + 1];
    char *name;
    int ram_mb;
    float screen_size_inches;
    char *operating_system;
} Product;

// data from quotes.csv
typedef struct {
    char quote_id[QUOTE_ID_LEN + 1];
    char product_code[CODE_LEN + 1];  // Foreign key to Product
    char *retailer;
    float price;
    AvailabilityStatus availability;
} Quote;

#endif // MODELS_H