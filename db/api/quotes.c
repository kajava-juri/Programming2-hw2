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

const char *availability_strings[] = {
    [IN_STOCK] = "In Stock",
    [OUT_OF_STOCK] = "Out of Stock"};

void InitQuoteWrapper(GenericWrapper *qw)
{
    if (qw == NULL)
    {
        return;
    }
    qw->data = NULL;
    qw->size = sizeof(Quote);
    qw->used = 0;
    qw->limit = 0;
    qw->freeData = FreeQuote;
    qw->getElementAt = GetQuoteAt;
}

char *GetAvailabilityString(AvailabilityStatus status)
{
    if (status < 0 || status >= sizeof(availability_strings) / sizeof(availability_strings[0]))
    {
        return NULL;
    }
    return availability_strings[status];
}

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
    while (count < MAX_QUOTES && fscanf(fp, " %" STR(QUOTE_ID_LEN) "[^,] ,"                                                                                          // quote ID
                                            " %" STR(PRODUCT_CODE_LEN) "[^,] ,"                                                               // product code
                                            " %" STR(MAX_RETAILER_LEN) "[^,] ,"// retailer name
                                            "%f,"                                       // price
                                            " %" STR(QUOTE_AVAILABILITY_LEN) "[^\n]\n", // availability status
                                        bufCode, bufProductCode, bufRetailer, &bufPrice, bufAvailability) == 5)
    {
        printf("Quote %zu: %s, %s, %s, %.2f, %s\n", count + 1, bufCode, bufProductCode, bufRetailer, bufPrice, bufAvailability);
        if (count >= allocated)
        {
            allocated += 10; // enough for this task
            tempQuote = realloc(pQuotes, allocated * sizeof(Quote));
            if (tempQuote == NULL)
            {
                for (size_t i = 0; i < count; i++)
                {
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
        if (strcmp(bufAvailability, availability_strings[OUT_OF_STOCK]) == 0)
        {
            (pQuotes + count)->availability = OUT_OF_STOCK;
        }
        else if (strcmp(bufAvailability, availability_strings[IN_STOCK]) == 0)
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

Quote *GetQuoteByCode(GenericWrapper *pw, const char *quoteCode)
{
    if (pw == NULL || pw->data == NULL)
    {
        return NULL;
    }
    Quote *quotes = (Quote *)pw->data;
    for (size_t i = 0; i < pw->used; i++)
    {
        if (strcmp(quotes[i].quote_id, quoteCode) == 0)
        {
            return &quotes[i];
        }
    }
    return NULL;
}

/*Credit to this article https://codeforwin.org/c-programming/c-program-replace-specific-line-a-text-file*/
int ApplyQuoteEdits(Quote *quote, const char *quoteCode, const char *filename)
{
    if (quote == NULL || quoteCode == NULL || filename == NULL)
    {
        return 0;
    }

    // Write everyithing to a temporary file
    // and then replace the original file with the temporary file
    FILE *fp = fopen(filename, "rb+");
    FILE *fTemp;
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

    char *newAvailability = GetAvailabilityString(quote->availability);

    // Move the file pointer until the product code is found
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // If line contains the searched product code, replace with the updated product
        // else write the line to the temporary file
        if (strncmp(line, quoteCode, QUOTE_ID_LEN) == 0)
        {
            fprintf(fTemp, "%s,%s,%s,%.2f,%s\n", quote->quote_id, quote->product_code,
                    quote->retailer, quote->price, newAvailability);
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