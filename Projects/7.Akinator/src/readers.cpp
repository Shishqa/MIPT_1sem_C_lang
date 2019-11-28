#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"
#include "readers.h"

void printStr (FILE * out, const void * data_ptr)
{
    assert (out != nullptr);
    assert (data_ptr != nullptr);

    fprintf (out, "\"%s\"", ((String *) data_ptr)->data);
}

void printStrDot (FILE * out, const void * data_ptr)
{
    assert (out != nullptr);
    assert (data_ptr != nullptr);

    fprintf (out, "%s", ((String *) data_ptr)->data);
}

void readStr (char ** in,  const void * data_ptr)
{
    assert (in != nullptr);
    assert (*in != nullptr);
    assert (data_ptr != nullptr);

    size_t skip = 0;

    sscanf (*in, "\"%[^\"]\"%n", ((String *) data_ptr)->data, &skip);

    *in += skip;
}