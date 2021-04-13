#include "headers.hpp"

#include "operations.hpp"

void PrintToken (FILE * f, const void * ptr)
{
    const double eps = 0.01;
    double mod = 0;

    switch (((Node<Token> *) ptr)->data.type)
    {
        case OP_TYPE:
        {
            fprintf (f, "%s", operations[((Node<Token> *) ptr)->data.op_id].name);
        }
        break;

        case NUM_TYPE:
        {
            if (((Node<Token> *) ptr)->data.data < 0)
            {
                modf (((Node<Token> *) ptr)->data.data - 0.5, &mod);
            }
            else
            {
                modf (((Node<Token> *) ptr)->data.data + 0.5, &mod);
            }

            if (fabs (mod - ((Node<Token> *) ptr)->data.data) < eps)
            {
                fprintf (f, "%.0lf", mod);
            }
            else
            {
                fprintf (f, "%.2lf", ((Node<Token> *) ptr)->data.data);
            }
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