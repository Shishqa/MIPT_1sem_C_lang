#include "headers.hpp"

#include "operations.hpp"

void PrintMonomial (FILE * f, const void * ptr)
{
    switch (((Node<Monomial> *) ptr)->data.type)
    {
        case OP_TYPE:
        {
            fprintf (f, "%s", op_names[((Node<Monomial> *) ptr)->data.data]);
        }
        break;

        case NUM_TYPE:
        {
            fprintf (f, "%d", ((Node<Monomial> *) ptr)->data.data);
        }
        break;

        case VAR_TYPE:
        {
            fprintf (f, "%c", ((Node<Monomial> *) ptr)->data.data);
        }
        break;
    
        default:
        {
            printf ("Wrong type of Monomial!\n");
            fprintf (f, "$$$");
        }
        break;
    }
}