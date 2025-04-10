#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "data_api.h"
#include "utils.h"

void FreeWrapper(GenericWrapper *wrapper)
{
    if (wrapper == NULL) {
        return;
    }
    if (wrapper->data != NULL) {
        if (wrapper->freeData != NULL) {
            for (size_t i = 0; i < wrapper->used; i++) {
                wrapper->freeData(((void **)wrapper->data)[i]);
            }
        }
        FreeMemory(wrapper->data);
    }
    wrapper->size = 0;
    wrapper->used = 0;
    wrapper->limit = 0;
}