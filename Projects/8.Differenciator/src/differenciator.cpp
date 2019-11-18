#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "differenciator.hpp"

BinaryTree<Token> * DiffExpression (const BinaryTree<Token> * expression, const char var)
{
    assert (expression != nullptr);

    BinaryTree<Token> * diff = (BinaryTree<Token> *) calloc (1, sizeof (*diff));

    assert (diff != nullptr);

    diff->init ();

    diff->root = Diff (expression->root, var);

    return (diff);
}

Node<Token> * Diff (Node<Token> * node, const char var)
{
    assert (node != nullptr);

    if (TYPE(N) == NUM_TYPE)
    {
        return (n(0));
    }
    else if (TYPE(N) == VAR_TYPE && node->data.var == var)
    {
        return (n(1));
    }
    else if (TYPE(N) == VAR_TYPE)
    {
        return (DIFF (v(node->data.var), v(var)));
    }
    else
    {
        switch (OPCODE(N))
        {
            case ADD: // dL + dR
            {
                return (ADD ( d(L), d(R) )); 
            }
            break;

            case SUB: // dL - dR
            {
                return (SUB ( d(L), d(R) )); 
            }
            break;

            case MUL: // dL * R + L * dR
            {
                return (ADD ( MUL( d(L), c(R) ), 
                              MUL( c(L), d(R) ))); 
            }
            break;

            case DIV: // (dL * R - L * dR) / (R ^ 2)
            {
                return (DIV ( SUB ( MUL ( d(L), c(R) ), 
                                    MUL ( c(L), d(R) )), 
                              POW ( c(R), n(2) ))); 
            }
            break;

            case POW: // (L ^ R) * (dR * ln(L) + (dL / L) * R)
            {
                return (MUL ( c(N), 
                              ADD ( MUL ( d(R), LN( c(L) ) ),
                                    MUL ( DIV ( d(L), c(L) ), c(R) )))); 
            }
            break;

            case LN: // (1 / R) * dR
            {
                return (MUL ( DIV ( n(1), c(R) ), d(R) )); 
            }
            break;

            case EXP: // (e ^ R) * dR
            {
                return (MUL ( EXP ( c(R) ), d(R) ));
            }
            break;

            case SIN: // cos(R) * dR
            {
                return (MUL ( COS ( c(R) ), d(R) ));
            }
            break;

            case COS: // -1 * sin(R) * dR
            {
                return (MUL ( MUL( n(-1), SIN ( c(R) )), 
                              d(R) ));
            }
            break;

            case TG: // (1 / cos(R)^2) * dR
            {
                return (MUL ( DIV ( n(1), POW ( COS ( c(R) ), n(2) ) ), d(R) ));
            }
            break;

            case CTG: // (-1 / sin(R)^2) * dR
            {
                return (MUL ( DIV ( n(-1), POW ( SIN ( c(R) ), n(2) ) ) , d(R)));
            }
            break;

            case ARCSIN: // (1 / sqrt ( 1 - R^2 )) * dR
            {
                return (MUL ( DIV ( n(1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) ) );
            }
            break;

            case ARCCOS: // (-1 / sqrt ( 1 - R^2 )) * dR
            {
                return (MUL ( DIV ( n(-1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) ) );
            }
            break;

            case ARCTG: // (1 / ( 1 + R^2 )) * dR
            {
                return (MUL ( DIV ( n(1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) ) );
            }
            break;

            case ARCCTG: // (-1 / ( 1 + R^2 )) * dR
            {
                return (MUL ( DIV ( n(-1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) ) );
            }
            break;

            case SH: // ch(R) * dR
            {
                return (MUL ( CH ( c(R) ) , d(R) ));
            }
            break;

            case CH: // sh(R) * dR
            {
                return (MUL ( SH ( c(R) ) , d(R) ));
            }
            break;

            case TH: // (1 / ( ch(R) ^ 2 )) * dR 
            {
                return (MUL ( DIV ( n(1), POW ( CH ( c(R) ), n(2) ) ) , d(R) ));
            }
            break;

            case CTH: // (-1 / ( sh(R) ^ 2 )) * dR 
            {
                return (MUL ( DIV ( n(-1), POW ( SH ( c(R) ), n(2) ) ) , d(R) ));
            }
            break;

            case ABS: // sign(R) * dR
            {
                return (MUL ( SIGN ( c(R) ), d(R) ));
            }
            break;

            case SIGN: // 0
            {
                return (n(0));
            }
            break;
        
            default:
            {
                assert (0);
            }
            break;
        }
    }
}