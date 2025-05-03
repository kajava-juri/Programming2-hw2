#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "products.h"
#include "../../utils.h"
#include "../../logger.h"

// with the help of stack overflow
// https://stackoverflow.com/questions/5459868/concatenate-int-to-string-using-c-preprocessor
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

void FreeProduct(void *pProduct)
{
    Product *product = (Product *)pProduct;
    if (product == NULL)
    {
        return;
    }
    FreeMemory((void **)&product->name);
    FreeMemory((void **)&product->operating_system);
}

void *GetProductAt(GenericWrapper *pw, size_t index)
{
    if (pw == NULL || pw->data == NULL || index >= pw->used)
    {
        return NULL;
    }
    Product *pProduct = (Product *)pw->data + index;
    return (void *)pProduct;
}


void ReadProducts(GenericWrapper *pw, char *filename)
{
    LogInfo("Reading products from file");
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        LogError("Failed to open products file");
        perror("Failed to open products file");
        exit(EXIT_FAILURE);
    }

    size_t count = 0;
    size_t allocated = 0;

    // Main pointer for allocated memory
    Product *pProducts = NULL;

    // Temporary pointer for reading
    Product *tempProduct = NULL;

    // buffers for reading
    char bufCode[PRODUCT_CODE_LEN + 1], bufName[BUF_LEN + 1], bufOS[BUF_LEN + 1];
    int bufRam;
    float bufScreen;

    // skip csv header
    fscanf(fp, "%*[^\n]\n"); // Reads and discards everything until the first newline

    // Limit the maximum characters read by having the MACRO value after '%' to avoid buffer overflow
    while (count < MAX_PRODUCTS && fscanf(fp, " %" STR(PRODUCT_CODE_LEN) "[^,] ,"                                            // product code
                                                                         " %" STR(BUF_LEN) "[^,] ,"                      // product name
                                                                                           "%d,%f,"                      // RAM size, screen size
                                                                                           " %" STR(BUF_LEN) "[^\n] \n", // operating system
                                          bufCode, bufName, &bufRam, &bufScreen, bufOS) == 5)
    {
        printf("Product %zu: %s, %s, %d MB, %.2f inches, %s\n", count + 1, bufCode, bufName, bufRam, bufScreen, bufOS);
        if (count >= allocated)
        {
            allocated += 10; // enough for this task
            tempProduct = realloc(pProducts, allocated * sizeof(Product));
            if (tempProduct == NULL)
            {
                for (size_t i = 0; i < count; i++)
                {
                    FreeProduct(&pProducts[i]);
                }
                FreeMemory((void **)&pProducts);

                fclose(fp);
                LogError("Failed to allocate memory for products");
                perror("Failed to allocate memory for products");
                exit(EXIT_FAILURE);
            }

            pProducts = tempProduct;
        }

        (pProducts + count)->ram_mb = bufRam;
        (pProducts + count)->screen_size_inches = bufScreen;
        (pProducts + count)->operating_system = strdup(bufOS);
        (pProducts + count)->name = strdup(bufName);

        strcpy((pProducts + count)->product_code, bufCode);

        count++;
    }

    pw->data = pProducts;
    pw->size = sizeof(Product);
    pw->used = count;
    pw->limit = allocated;

    fclose(fp);
}

void DisplayProduct(Product *product)
{
    if (product == NULL)
    {
        return;
    }
    printf("Product Code: %s\n", product->product_code);
    printf("Name: %s\n", product->name);
    printf("RAM: %d MB\n", product->ram_mb);
    printf("Screen Size: %.2f inches\n", product->screen_size_inches);
    printf("Operating System: %s\n", product->operating_system);
}
void DisplayProducts(GenericWrapper *pw)
{
    if (pw == NULL || pw->data == NULL)
    {
        return;
    }
    printf("%d products found:\n", (int)pw->used);
    // not safe? maybe add a 'type' property to the wrapper
    Product *products = (Product *)pw->data;
    for (size_t i = 0; i < pw->used; i++)
    {
        DisplayProduct(&products[i]);
        printf("\n");
    }
}

Product *GetProductByCode(GenericWrapper *pw, const char *productCode)
{
    if (pw == NULL || pw->data == NULL || productCode == NULL)
    {
        return NULL;
    }
    Product *products = (Product *)pw->data;
    for (size_t i = 0; i < pw->used; i++)
    {
        if (strcmp(products[i].product_code, productCode) == 0)
        {
            return &products[i];
        }
    }
    return NULL;
}

int ApplyProductEdits(Product *product, const char *productCode, const char *filename)
{
    if (product == NULL || productCode == NULL || filename == NULL)
    {
        return 0;
    }

    // Write everyithing to a temporary file
    // and then replace the original file with the temporary file
    FILE *fp = fopen(filename, "rb+");
    FILE * fTemp;
    if (fp == NULL)
    {
        LogError("Failed to open products file for writing");
        perror("Failed to open products file for writing");
        return 0;
    }
    fTemp = fopen("data/replace.tmp.csv", "w"); 
    if (fTemp == NULL)
    {
        LogError("Failed to open temporary file for writing");
        fclose(fp);
        return 0;
    }

    // Move the file pointer until the product code is found
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // If line contains the searched product code, replace with the updated product
        // else write the line to the temporary file
        if (strncmp(line, productCode, PRODUCT_CODE_LEN) == 0)
        {
            fprintf(fTemp, "%s,%s,%d,%.2f,%s\n", product->product_code, product->name,
                    product->ram_mb, product->screen_size_inches, product->operating_system);
        }
        else
        {
            fputs(line, fTemp);
        }
    }

    // save the changes
    fclose(fTemp);
    fclose(fp);

    // Replace the original file with the temporary file
    remove(filename);
    rename("data/replace.tmp.csv", filename);

    return 1;
}