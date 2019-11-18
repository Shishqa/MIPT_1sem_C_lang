#include "headers.hpp"

#include "operations.hpp"

void PrintToken (FILE * f, const void * ptr)
{
    switch (((Node<Token> *) ptr)->data.type)
    {
        case OP_TYPE:
        {
            fprintf (f, "%s", operations[((Node<Token> *) ptr)->data.op_id].name);
        }
        break;

        case NUM_TYPE:
        {
            fprintf (f, "%.3lf", ((Node<Token> *) ptr)->data.data);
        }
        break;

        case VAR_TYPE:
        {
            fprintf (f, "%c", ((Node<Token> *) ptr)->data.var);
        }
        break;
    
        default:
        {
            printf ("Wrong type of Token!\n");
            fprintf (f, "$$$");
        }
        break;
    }
}