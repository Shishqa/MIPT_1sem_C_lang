#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "simplify.hpp"

Node<Token> * Simplify (BinaryTree<Token> * exp, Node<Token> * node)
{
    if (L)
    {
        L = Simplify (exp, L);
        L->P = N;
    }

    if (R)
    {
        R = Simplify (exp, R);
        R->P = N;
    }

    if (TYPE(N) != OP_TYPE)
    {
        return (N);
    }

    Node<Token> * tmp = nullptr;
    char op = 0;
    int l_val = 0;
    int r_val = 0;

    /*
    if ((R && TYPE(R) == NUM_TYPE) && (L && TYPE(L) == NUM_TYPE))
    {
        op    = DATA(N);
        l_val = DATA(L);
        r_val = DATA(R);
        exp->deleteSubtree (N);
        return (n(Count (op, l_val, r_val)));
    }
    */

    //ZERO
    if ((OPCODE(N) == ADD || OPCODE(N) == SUB) && RIGHT (0))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (OPCODE(N) == ADD && LEFT (0))
    {
        tmp = c(R);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (OPCODE(N) == SUB && LEFT (0))
    {
        tmp = c(R);
        exp->deleteSubtree (N);
        return (MUL( n(-1), tmp ));
    }
    if ((OPCODE(N) == MUL || OPCODE(N) == DIV) && (LEFT (0) || RIGHT (0)))
    {
        exp->deleteSubtree (N);
        return (n(0));
    }
    if (OPCODE(N) == POW && LEFT (0))
    {
        exp->deleteSubtree (N);
        return (n(0));
    }
    if (OPCODE(N) == POW && RIGHT (0))
    {
        exp->deleteSubtree (N);
        return (n(1));
    }

    //ONE

    if (OPCODE(N) == MUL && LEFT (1))
    {
        tmp = c(R);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (OPCODE(N) == MUL && RIGHT (1))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (OPCODE(N) == DIV && RIGHT (1))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (OPCODE(N) == POW && LEFT (1))
    {
        exp->deleteSubtree (N);
        return (n(1));
    }
    if (OPCODE(N) == POW && RIGHT (1))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }

    //

    // if (((DATA(N) == ADD || DATA(N) == MUL) && 
    //       L && R && TYPE(L) == OP_TYPE && TYPE(R) == OP_TYPE &&
    //       DATA(L) < DATA(R)) ||
    //     (DATA(N) == MUL && TYPE(R) == NUM_TYPE))
    // {
    //     tmp = L;
    //     L = R;
    //     R = tmp;

    //     return (N);
    // }

    return (N);
}

int Count (const char op, int r)
{
    return 0;
}

int Count (const char op, int l, int r)
{
    switch (op)
    {
        case ADD:
        {
            return (l + r);
        }
        break;

        case SUB:
        {
            return (l - r);
        }
        break;

        case MUL:
        {
            return (l * r);
        }
        break;

        case DIV:
        {
            return (l / r);
        }
        break;

        case POW:
        {
            return (BinaryPow (l, r));
        }
        break;
    
        default:
        break;
    }
}

int BinaryPow (int x, int a)
{
    if (a < 0)
    {
    	return 0;
    }
    if (a == 0)
    {
    	return 1;
    }

    int tmp = BinaryPow (x, a / 2);

    return ((a % 2 == 0) ? (tmp * tmp) : (tmp * tmp * x));
}