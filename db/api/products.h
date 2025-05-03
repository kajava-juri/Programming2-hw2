#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "../data_api.h"

// Define length constants first, before any includes
#define PRODUCT_CODE_LEN 5       // product codes "SP001"
#define MAX_NAME_LEN 50      // product names
#define MAX_OS_LEN 10        // "Android", "iOS" ...

// data from products.csv
typedef struct {
    char product_code[PRODUCT_CODE_LEN + 1];
    char *name;
    int ram_mb;
    float screen_size_inches;
    char *operating_system;
} Product;

void FreeProduct(void *pProduct);

/**
 * @brief Reads product data from the csv file
 *
 * @param pw Wrapper to hold the product data and allocation information
 * @param filename The name of the file to read the product data from
 *
 * @return Number of products read, or negative value on error
 */
void ReadProducts(GenericWrapper *pw, char *filename);

/**
 * @brief Displays the product information
 *
 * @param product Pointer to the Product to be displayed
 */
void DisplayProduct(Product *product);

/**
 * @brief Displays all products in the GenericWrapper
 *
 * @param pw Pointer to the GenericWrapper containing products
 */
void DisplayProducts(GenericWrapper *pw);

/**
 * @brief Gets a Product at given index
 *
 * @param pw Pointer to the GenericWrapper containing products
 * @param productCode The product code to search for
 * @return Pointer to the Product if found, NULL otherwise
 */
void *GetProductAt(GenericWrapper *pw, size_t index);

/**
 * @brief Gets a Product by its product code
 *
 * @param pw Pointer to the GenericWrapper containing products
 * @param productCode The product code to search for
 * @return Pointer to the Product if found, NULL otherwise
 */
Product *GetProductByCode(GenericWrapper *pw, const char *productCode);

/**
 * @brief Edits the screen size of a product
 *
 * @param product Pointer to the Product to be edited
 * @param productCode The product code to search for
 * @param newScreenSize The new screen size to set
 * @param filename The name of the file to save the changes to
 * @return 1 on success, 0 on failure
 */
int ApplyProductEdits(Product *product, const char *productCode, const char *filename);

#endif // PRODUCTS_H