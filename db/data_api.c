#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "data_api.h"
#include "../utils.h"

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
                
                void *pData = wrapper->getElementAt(wrapper, i);
                // continue if pData is NULL
                if (pData == NULL)
                {
                    continue;
                }
                wrapper->freeData(pData);
            }
        }
        FreeMemory(&wrapper->data);
    }
    wrapper->size = 0;
    wrapper->used = 0;
    wrapper->limit = 0;

}

void *GetElementAt(GenericWrapper *wrapper, size_t index)
{
    if(wrapper == NULL || wrapper->data == NULL)
    {
        return NULL;
    }

    char *data = (char *)wrapper->data + (index * wrapper->size);
    return data;
}