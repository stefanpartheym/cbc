#include <ctype.h>
#include <stdlib.h>

#include "test_utils.h"


/* -------------------------------------------------------------------------- */

TestDummy* dummy_create(int id)
{
    TestDummy* self = malloc(sizeof(TestDummy));
    self->id = id;
    return self;
}

void dummy_destroy(TestDummy* self)
{
    free(self);
}


/* -------------------------------------------------------------------------- */

/*
 * Copy stream content to a string
 * 
 * NOTE: Arguemnt string must be allocated before this function is called!
 */
void stream_to_string(FILE* stream, char* string, bool trim)
{
    rewind(stream); /* go to beginning of the stream */
    
    while (!feof(stream))
    {
        *string = fgetc(stream);
        
        if (feof(stream))
            *string = '\0';
        else
            string++;
    }
    
    if (trim) /* trim string */
        while (!isprint((int) *string))
        {
            *string = '\0';
            string--;
        }
}
