/*******************************************************************************
 * Implementation of a simple dynamic vector data structure.
 ******************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>


typedef struct Vector Vector;
typedef void*         VectorItem;


/* -------------------------------------------------------------------------- */

/*
 * Create a Vector object.
 */
Vector* vector_create();

/*
 * Destroy a Vector object.
 */
void vector_destroy(Vector* self);

/*
 * Append an item to the vector.
 * @result Returns index of the appended item. Returns -1 if an error occurred.
 */
int vector_append(Vector* self, VectorItem item);

/*
 * Remove an item from the vector.
 * @result Returns the removed item.
 */
VectorItem vector_remove(Vector* self, int index);

/*
 * Remove all items from the vector.
 */
void vector_clear(Vector* self);

/*
 * Get an item from a specific index in the vector.
 */
bool vector_get(const Vector* self, int index, VectorItem* destination);

/*
 * Set an item at a specific index in the vector.
 */
bool vector_set(Vector* self, int index, VectorItem item);

/*
 * Get item count.
 */
size_t vector_get_count(const Vector* self);


#endif /* VECTOR_H */
