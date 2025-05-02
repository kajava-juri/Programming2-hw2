#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "data_api.h"
#include "../utils.h"



/**
 * @brief Frees resources associated with a wrapper object.
 *
 * This function deallocates memory and releases any resources that were
 * allocated by the wrapper.
 *
 * @param wrapper Pointer to the wrapper object to be freed.
 * @return void
 */
void FreeWrapper(GenericWrapper *wrapper)
{
    if (wrapper == NULL)
    {
        return;
    }
    if (wrapper->data != NULL)
    {
        if (wrapper->freeData != NULL)
        {
            for (size_t i = 0; i < wrapper->used; i++)
            {
                // get the data pointer at index i for the unknown type
                void *pData = wrapper->getElementAt(wrapper, i);
                // continue if pData is NULL
                if (pData == NULL)
                {
                    continue;
                }

                // call the struct specific memory deallocation function
                wrapper->freeData(pData);
            }
        }
        // free the data pointer itself
        FreeMemory(&wrapper->data);
    }
    wrapper->size = 0;
    wrapper->used = 0;
    wrapper->limit = 0;

}