#include "headers.hpp"

#include "operations.hpp"

void PrintMonomial (FILE * f, const void * ptr)
{
    switch (((Node<Token> *) ptr)->data.type)
    {
        case OP_TYPE:
        {
            fprintf (f, "%s", op_names[((Node<Token> *) ptr)->data.opcode]);
        }
        break;

        case NUM_TYPE:
        {
            fprintf (f, "%.0lf", ((Node<Token> *) ptr)->data.data);
        }
        break;

        case VAR_TYPE:
        {
            fprintf (f, "%c", ((Node<Token> *) ptr)->data.opcode);
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