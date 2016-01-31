#include <string.h>

#include "utils.h"
#include "vector.h"


struct Vector
{
    size_t count;
    VectorItem* elements;
    size_t element_size;
    size_t block_size;
    size_t alloc_size;
};

static const int ITEMS_PER_BLOCK = 16;


/* -------------------------------------------------------------------------- */

static bool vector_is_full(const Vector* self);
static bool vector_increase_size(Vector* self, unsigned int blocks);
static bool vector_check_bounds(const Vector* self, int index);


/* -------------------------------------------------------------------------- */

Vector* vector_create()
{
    Vector* self       = memalloc(sizeof(Vector));
    self->count        = 0;
    self->element_size = sizeof(VectorItem);
    self->block_size   = self->element_size * ITEMS_PER_BLOCK;
    self->elements     = memalloc(self->block_size);
    self->alloc_size   = self->block_size;

    return self;
}

void vector_destroy(Vector* self)
{
    memfree(self->elements);
    memfree(self);
}

int vector_append(Vector* self, VectorItem item)
{
    int index;

    if (vector_is_full(self))
        vector_increase_size(self, 1);

    self->count++;
    index = self->count - 1;      /* index is the last item */
    self->elements[index] = NULL; /* clear allocated memory (TODO: remove!) */

    if (vector_set(self, index, item))
        return index;
    else
        return -1;
}

VectorItem vector_remove(Vector* self, int index)
{
    VectorItem removed_item;
    VectorItem source;
    VectorItem destination;
    size_t size;

    /*
     * TODO: Check bounds, when accessing elements via index.
     */
    removed_item = self->elements[index];
    if (!vector_set(self, index, NULL))
        return NULL;

    source      = self->elements + index + 1;
    destination = self->elements + index;
    size        = (self->count - (index + 1)) * self->element_size;
    memmove(destination, source, size);

    self->count--;

    return removed_item;
}

void vector_clear(Vector* self)
{
    self->count = 0;
    memclr(self->elements, self->alloc_size);
}

bool vector_get(const Vector* self, int index, VectorItem* destination)
{
    if (!vector_check_bounds(self, index))
        return false;

    *destination = self->elements[index];
    return true;
}

bool vector_set(Vector* self, int index, const VectorItem item)
{
    if (!vector_check_bounds(self, index))
        return false;

    self->elements[index] = item;
    return true;
}

size_t vector_get_count(const Vector* self)
{
    return self->count;
}


/* -------------------------------------------------------------------------- */

static bool vector_is_full(const Vector* self)
{
    return ((self->count * self->element_size) >= self->alloc_size);
}

static bool vector_increase_size(Vector* self, unsigned int blocks)
{
    size_t new_size  = self->alloc_size + (self->block_size * blocks);
    VectorItem* temp = memrealloc(self->elements, new_size);
    if (temp == NULL)
        return false;

    self->elements   = temp;
    self->alloc_size = new_size;

    return true;
}

static bool vector_check_bounds(const Vector* self, int index)
{
    return (self->count > 0) && (index < self->count) && (index >= 0);
}
