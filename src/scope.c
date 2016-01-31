#include <stdlib.h>

#include "utils.h"
#include "symbol.h"
#include "scope.h"


/* -------------------------------------------------------------------------- */

struct CbScope
{
    const CbScope* parent;
    CbHashTable* symbols;
};


/* -------------------------------------------------------------------------- */

CbScope* cb_scope_create(const CbScope* parent)
{
    CbScope* self = (CbScope*) memalloc(sizeof(CbScope));
    self->parent  = parent;
    self->symbols =
        cb_hash_table_create(16, NULL, (CbHashItemDestructor) cb_symbol_destroy);

    return self;
}

void cb_scope_destroy(CbScope* self)
{
    cb_hash_table_destroy(self->symbols);
    memfree(self);
}

const CbScope* cb_scope_get_parent(const CbScope* self)
{
    return self->parent;
}

CbHashTable* cb_scope_get_symbols(const CbScope* self)
{
    return self->symbols;
}
