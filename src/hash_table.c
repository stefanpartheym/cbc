#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "hash_table.h"


/* -------------------------------------------------------------------------- */

typedef struct CbHashNode CbHashNode;
struct CbHashNode
{
    char* key;
    void* data;
    CbHashNode* next;
};

struct CbHashTable
{
    CbHashSize size;
    CbHashNode** nodes;
    CbHashFunc hash_func;
    bool destroy_items;
    CbHashItemDestructor item_destructor;
};

static CbHashSize cb_hash_table_default_hash_function(const char* key);


/* -------------------------------------------------------------------------- */

CbHashTable* cb_hash_table_create(CbHashSize size,
                                  CbHashFunc hash_func,
                                  CbHashItemDestructor item_destructor)
{
    CbHashTable* self = memalloc(sizeof(CbHashTable));
    self->nodes       = calloc(size, sizeof(CbHashNode*));
    self->size        = size;
    
    if (hash_func)
        self->hash_func = hash_func;
    else
        self->hash_func = cb_hash_table_default_hash_function;
    
    if (item_destructor)
    {
        self->destroy_items   = true;
        self->item_destructor = item_destructor;
    }
    else
    {
        self->destroy_items   = false;
        self->item_destructor = NULL;
    }
    
    return self;
}

void cb_hash_table_destroy(CbHashTable* self)
{
    CbHashNode* node;
    CbHashNode* old_node;
    CbHashSize n = 0;
    
    for (; n < self->size; n++)
    {
        node = self->nodes[n];
        while (node)
        {
            memfree(node->key);
            old_node = node;
            node     = node->next;
            
            if (self->destroy_items && old_node->data)
                self->item_destructor(old_node->data);
            
            memfree(old_node);
        }
    }
    
    memfree(self->nodes);
    memfree(self);
}

void cb_hash_table_insert(CbHashTable* self, const char* key, void* data)
{
    CbHashNode* node;
    CbHashSize hash = self->hash_func(key) % self->size;
    
    node = self->nodes[hash];
    while (node)
    {
        if (!strcmp(node->key, key))
            node->data = data;
        
        node = node->next;
    }
    
    node       = (CbHashNode*) malloc(sizeof(CbHashNode));
    node->key  = strdup(key);
    node->data = data;
    node->next = self->nodes[hash];
    
    self->nodes[hash] = node;
}

bool cb_hash_table_remove(CbHashTable* self, const char* key)
{
    CbHashNode* node;
    CbHashNode* prev = NULL;
    CbHashSize hash  = self->hash_func(key) % self->size;
    
    node = self->nodes[hash];
    while (node)
    {
        if (!strcmp(node->key, key))
        {
            memfree(node->key);
            if(prev)
                prev->next = node->next;
            else
                self->nodes[hash]=node->next;
            
            if (self->destroy_items && node->data)
                self->item_destructor(node->data);
            
            memfree(node);
            return true;
        }
        
        prev = node;
        node = node->next;
    }
    
    return false;
}

void* cb_hash_table_get(const CbHashTable* self, const char* key)
{
    CbHashSize hash  = self->hash_func(key) % self->size;
    CbHashNode* node = self->nodes[hash];
    
    while (node)
    {
        if (!strcmp(node->key, key))
            return node->data;
        
        node = node->next;
    }
    
    return NULL;
}

void cb_hash_table_resize(CbHashTable* self, CbHashSize size)
{
    CbHashTable newtbl;
    CbHashSize n;
    CbHashNode* node;
    CbHashNode* next;
    
    newtbl.size      = size;
    newtbl.hash_func = self->hash_func;
    newtbl.nodes     = calloc(size, sizeof(CbHashNode*));
    
    for (n = 0; n < self->size; n++)
    {
        for (node = self->nodes[n]; node; node = next)
        {
            next = node->next;
            cb_hash_table_insert(&newtbl, node->key, node->data);
            cb_hash_table_remove(self, node->key);
        }
    }
    
    memfree(self->nodes);
    self->size  = newtbl.size;
    self->nodes = newtbl.nodes;
}


/* -------------------------------------------------------------------------- */

static CbHashSize cb_hash_table_default_hash_function(const char* key)
{
    CbHashSize hash = 0;
    
    while (*key)
    {
        hash += (unsigned char) *key;
        key++;
    }
    
    return hash;
}
