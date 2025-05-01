#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "quotes.h"
#include "../../utils.h"
#include "../../logger.h"

// with the help of stack overflow
// https://stackoverflow.com/questions/5459868/concatenate-int-to-string-using-c-preprocessor
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

const char* availability_strings[] = {
    [IN_STOCK] = "Out of Stock",
    [OUT_OF_STOCK] = "In Stock"
};

void FreeQuote(void *pQuote)
{
    Quote *quote = (Quote *)pQuote;
    if (quote == NULL) 
    {
        return;
    }
    FreeMemory((void **)&quote->retailer);
}

void *GetQuoteAt(GenericWrapper *pw, size_t index)
{
    if (pw == NULL || pw->data == NULL || index >= pw->used) 
    {
        return NULL;
    }
    Quote *pQuote = (Quote *)pw->data + index;
    return (void *)pQuote;
}

/**
 * @brief Reads quote data from the csv file
 *
 * @param pw Wrapper to hold the quotes data and allocation information
 * @param filename The name of the file to read the quote data from
 *
 * @return Number of quotes read, or negative value on error
 */
void ReadQuotes(GenericWrapper *pw, char *filename)
{
    LogInfo("Reading quotes from file");
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) 
    {
        LogError("Failed to open quotes file");
        perror("Failed to open quotes file");
        exit(EXIT_FAILURE);
    }

    size_t count = 0;
    size_t allocated = 0;

    // Main pointer for allocated memory
    Quote *pQuotes = NULL;

    // Temporary pointer for reading
    Quote *tempQuote = NULL;

    // buffers for reading
    char bufCode[QUOTE_ID_LEN + 1], bufProductCode[PRODUCT_CODE_LEN + 1], bufRetailer[MAX_RETAILER_LEN + 1];
    float bufPrice;
    char bufAvailability[QUOTE_AVAILABILITY_LEN + 1];

    // skip csv header
    fscanf(fp, "%*[^\n]\n"); // Reads and discards everything until the first newline

    // Limit the maximum characters read by having the MACRO value after '%' to avoid buffer overflow
    while(count < MAX_QUOTES && fscanf(fp, " %" STR(QUOTE_ID_LEN) "[^,] ," // quote ID
        " %" STR(PRODUCT_CODE_LEN) "[^,] ," // product code
        " %" STR(MAX_RETAILER_LEN) "[^,] ," // retailer name
        "%f," // price
        " %" STR(QUOTE_AVAILABILITY_LEN) "[^\n]\n", // availability status 
        bufCode, bufProductCode, bufRetailer, &bufPrice, bufAvailability) == 5) 
    {
        printf("Quote %zu: %s, %s, %s, %.2f, %s\n", count + 1, bufCode, bufProductCode, bufRetailer, bufPrice, bufAvailability);
        if(count >= allocated)
        {
            allocated += 10; // enough for this task
            tempQuote = realloc(pQuotes, allocated * sizeof(Quote));
            if (tempQuote == NULL) 
            {
                for (size_t i = 0; i < count; i++) {
                    FreeQuote(&pQuotes[i]);
                }
                FreeMemory((void **)&pQuotes);

                fclose(fp);
                LogError("Failed to allocate memory for quotes");
                perror("Failed to allocate memory for quotes");
                exit(EXIT_FAILURE);
            }

            pQuotes = tempQuote;
        }

        (pQuotes + count)->retailer = strdup(bufRetailer);
        (pQuotes + count)->price = bufPrice;

        strcpy((pQuotes + count)->quote_id, bufCode);
        strcpy((pQuotes + count)->product_code, bufProductCode);

        // Determine availability status
        // Check if the availability status is valid
        if(strcmp(bufAvailability, availability_strings[OUT_OF_STOCK]) == 0)
        {
            (pQuotes + count)->availability = OUT_OF_STOCK;
        }
        else if(strcmp(bufAvailability, availability_strings[IN_STOCK]) == 0)
        {
            (pQuotes + count)->availability = IN_STOCK;
        }
        else
        {
            char error_message[BUF_LEN];
            sprintf(error_message, "Invalid availability status: %s", bufAvailability);
            fprintf(stderr, "%s\n", error_message);
            LogError(error_message);
            FreeMemory((void **)&pQuotes);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        
        count++;
    }

    pw->data = pQuotes;
    pw->size = sizeof(Quote);
    pw->used = count;
    pw->limit = allocated;

    fclose(fp);
}

void DisplayQuote(Quote *quote)
{
    if (quote == NULL) 
    {
        return;
    }
    printf("Quote ID: %s\n", quote->quote_id);
    printf("Product Code: %s\n", quote->product_code);
    printf("Retailer: %s\n", quote->retailer);
    printf("Price: %.2f\n", quote->price);
    printf("Availability: %s\n", availability_strings[quote->availability]);
}
void DisplayQuotes(GenericWrapper *pw)
{
    if (pw == NULL || pw->data == NULL) 
    {
        return;
    }
    printf("%d quotes found:\n", (int)pw->used);
    // not safe? maybe add a 'type' property to the wrapper
    Quote *quotes = (Quote *)pw->data;
    for (size_t i = 0; i < pw->used; i++) 
    {
        DisplayQuote(&quotes[i]);
        printf("\n");
    }
}