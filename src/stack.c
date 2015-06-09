/*******************************************************************************
 * Implementation of CbStack
 ******************************************************************************/

#include <stdlib.h>

#include "stack.h"


/* -------------------------------------------------------------------------- */

typedef struct CbStackItem CbStackItem;
struct CbStackItem
{
    CbStackItem* prior;
    const void* data;
};

struct CbStack
{
    CbStackItem* top;
    size_t count;
};


/* -------------------------------------------------------------------------- */

CbStack* cb_stack_create()
{
    CbStack* self = (CbStack*) malloc(sizeof(CbStack));
    self->top     = NULL;
    self->count   = 0;
    
    return self;
}

void cb_stack_destroy(CbStack* self)
{
    free(self);
}

void cb_stack_push(CbStack* self, const void* item)
{
    CbStackItem* stack_item = (CbStackItem*) malloc(sizeof(CbStackItem));
    stack_item->data        = item;
    stack_item->prior       = self->top;
    
    self->top = stack_item;
    self->count++;
}

bool cb_stack_pop(CbStack* self, void** dest)
{
    CbStackItem* temp;
    
    /* check for stack-underflow */
    if (cb_stack_is_empty(self))
        return false;
    
    /* only set destinaion, if it's a valid memory-address */
    if (dest != NULL)
        *dest = (void*) self->top->data;
    
    temp      = self->top;
    self->top = self->top->prior; /* move top to prior item */
    free(temp);                   /* free former top item */
    
    self->count--;
    
    return true;
}

const void* cb_stack_get_top_item(const CbStack* self)
{
    const void* result = NULL;
    if (self->top)
        result = self->top->data;
    
    return result;
}

size_t cb_stack_count(const CbStack* self)
{
    return self->count;
}

bool cb_stack_is_empty(const CbStack* self)
{
    return self->count == 0;
}
