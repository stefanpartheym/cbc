/*******************************************************************************
 * @file  stack.h
 * @brief Contains the CbStack structure
 *
 * Implementation of a simple generic stack data structure
 ******************************************************************************/

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>


/**
 * @struct CbStack
 * @brief  A simple and generic stack data structure
 */
typedef struct CbStack CbStack;


/**
 * @memberof CbStack
 * @brief    Constructor
 */
CbStack* cb_stack_create();

/**
 * @memberof CbStack
 * @brief    Destructor
 *
 * @param self The stack instance to destroy
 */
void cb_stack_destroy(CbStack* self);

/**
 * @memberof CbStack
 * @brief    Push an item on the stack
 *
 * @param self The stack instance
 * @param item The item to push on the stack
 */
void cb_stack_push(CbStack* self, const void* item);

/**
 * @memberof CbStack
 * @brief    Pop an item off the stack
 *
 * @param self The stack instance
 * @param dest Destination adress where the popped item should be stored in
 */
bool cb_stack_pop(CbStack* self, void** dest);

/**
 * @memberof CbStack
 * @brief    Get pointer to the data member of the top item
 *
 * @param self The stack instance
 */
const void* cb_stack_get_top_item(const CbStack* self);

/**
 * @memberof CbStack
 * @brief    Get item count
 *
 * @param self The stack instance
 */
size_t cb_stack_count(const CbStack* self);

/**
 * @memberof CbStack
 * @brief    Check whether the stack is empty
 *
 * @param self The stack instance
 */
bool cb_stack_is_empty(const CbStack* self);


#endif /* STACK_H */
