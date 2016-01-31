/*******************************************************************************
 * @file  hash_table.h
 * @brief Contains the CbHashTable structure
 *
 * Implementation of a generic hash table data structure.
 *
 * IMPORTANT NOTE: All source code (concerning hash_table.h and hash_table.c)
 *                 is retrieved from:
 *                    http://en.literateprograms.org/Hash_table_(C)?oldid=19638
 *                 The source code was slightly modified to fit the projects
 *                 source code style.
 ******************************************************************************/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdbool.h>


typedef size_t CbHashSize;

/**
 * @struct CbHashTable
 * @brief  A generic hash table data structure
 */
typedef struct CbHashTable CbHashTable;

typedef CbHashSize (*CbHashFunc)(const char*);
typedef void (*CbHashItemDestructor)(void*);


/**
 * @memberof CbHashTable
 * @brief    Constructor
 *
 * @param size      The initial size of the hash table
 * @param hash_func       Pointer to a function, that transforms a char array
 *                        into a hash. If NULL is passed, the default hash
 *                        function will be used.
 * @param item_destructor Pointer to a function, is will be used as a destructor
 *                        for items being removed from the hash table
 */
CbHashTable* cb_hash_table_create(CbHashSize size,
                                  CbHashFunc hash_func,
                                  CbHashItemDestructor item_destructor);

/**
 * @memberof CbHashTable
 * @brief    Destructor
 *
 * @param self The hash table instance
 */
void cb_hash_table_destroy(CbHashTable* self);

/**
 * @memberof CbHashTable
 * @brief    Insert an item into the hash table
 *
 * @param self The hash table instance
 * @param key  The key, the value/data will be mapped to
 * @param data The value/data
 */
void cb_hash_table_insert(CbHashTable* self, const char* key, void* data);

/**
 * @memberof CbHashTable
 * @brief    Remove an item from the hash table
 *
 * @param self The hash table instance
 * @param self The key, the value/data is mapped to
 */
bool cb_hash_table_remove(CbHashTable* self, const char* key);

/**
 * @memberof CbHashTable
 * @brief    Get an item from the hash table
 *
 * @param self The hash table instance
 * @param self The key, the value/data is mapped to
 * @return Returns NULL if there is no item, that is mapped to the given key
 */
void* cb_hash_table_get(const CbHashTable* self, const char* key);

/**
 * @memberof CbHashTable
 * @brief    Resize the hash table
 *
 * @param self The hash table instance
 * @param size The new hash size
 */
void cb_hash_table_resize(CbHashTable* self, CbHashSize size);


#endif /* HASH_TABLE_H */
