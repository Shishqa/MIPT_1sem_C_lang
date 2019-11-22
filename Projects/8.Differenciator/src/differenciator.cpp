#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "differenciator.hpp"

#include "get_latex.hpp"

#include "simplify.hpp"

BinaryTree<Token> * DiffWithLaTeX  (const BinaryTree<Token> * expression, const char var, const char * name)
{
    FILE * latex = initLaTeX (name);

    fprintf (latex, "\\section*{Крутейшая-моднейшая производная}\\\\\n");

    BinaryTree<Token> * diff_expression = DiffExpression (expression, var, latex);

    fprintf (latex, "Now a little bit попроще:\\\\\n");

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    fprintf (latex, "\\begin{math}\n\t");

    getNodeLaTeX (diff_expression->root, latex);

    fprintf (latex, "\\end{math}\n");

    closeLaTeX (latex);
    compileLaTeX (name);

    return (diff_expression);
}

BinaryTree<Token> * DiffExpression (const BinaryTree<Token> * expression, const char var, FILE * latex)
{
    assert (expression != nullptr);

    BinaryTree<Token> * diff = (BinaryTree<Token> *) calloc (1, sizeof (*diff));

    assert (diff != nullptr);

    diff->init ();

    diff->root = Diff (expression->root, var, latex);

    return (diff);
}

Node<Token> * Diff (Node<Token> * node, const char var, FILE * latex)
{
    assert (node  != nullptr);

    Node<Token> * res = nullptr;

    fprintf (latex, "\\begin{math}\n\t(");

    getNodeLaTeX (node, latex);

    fprintf (latex, ")'\n\\end{math}\\\\\n");

    if (TYPE(N) == NUM_TYPE)
    {
        res = n(0);
    }
    else if (TYPE(N) == VAR_TYPE && node->data.var == var)
    {
        res = n(1);
    }
    else if (TYPE(N) == VAR_TYPE)
    {
        res = DIFF (v(node->data.var), v(var));
    }
    else
    {
        switch (OPCODE(N))
        {
            case ADD: // dL + dR
            {
                res = ADD ( d(L), d(R) ); 
            }
            break;

            case SUB: // dL - dR
            {
                res = SUB ( d(L), d(R) ); 
            }
            break;

            case MUL: // dL * R + L * dR
            {
                res = ADD ( MUL( d(L), c(R) ), 
                              MUL( c(L), d(R) )); 
            }
            break;

            case DIV: // (dL * R - L * dR) / (R ^ 2)
            {
                res = DIV ( SUB ( MUL ( d(L), c(R) ), 
                                    MUL ( c(L), d(R) )), 
                              POW ( c(R), n(2) )); 
            }
            break;

            case POW: // (L ^ R) * (dR * ln(L) + (dL / L) * R)
            {
                res = MUL ( c(N), 
                              ADD ( MUL ( d(R), LN( c(L) ) ),
                                    MUL ( DIV ( d(L), c(L) ), c(R) ))); 
            }
            break;

            case LN: // (1 / R) * dR
            {
                res = MUL ( DIV ( n(1), c(R) ), d(R) ); 
            }
            break;

            case EXP: // (e ^ R) * dR
            {
                res = MUL ( EXP ( c(R) ), d(R) );
            }
            break;

            case SIN: // cos(R) * dR
            {
                res = MUL ( COS ( c(R) ), d(R) );
            }
            break;

            case COS: // -1 * sin(R) * dR
            {
                res = MUL ( MUL( n(-1), SIN ( c(R) )), 
                              d(R) );
            }
            break;

            case TG: // (1 / cos(R)^2) * dR
            {
                res = MUL ( DIV ( n(1), POW ( COS ( c(R) ), n(2) ) ), d(R) );
            }
            break;

            case CTG: // (-1 / sin(R)^2) * dR
            {
                res = MUL ( DIV ( n(-1), POW ( SIN ( c(R) ), n(2) ) ) , d(R));
            }
            break;

            case ARCSIN: // (1 / sqrt ( 1 - R^2 )) * dR
            {
                res = MUL ( DIV ( n(1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) );
            }
            break;

            case ARCCOS: // (-1 / sqrt ( 1 - R^2 )) * dR
            {
                res = MUL ( DIV ( n(-1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) );
            }
            break;

            case ARCTG: // (1 / ( 1 + R^2 )) * dR
            {
                res = MUL ( DIV ( n(1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) );
            }
            break;

            case ARCCTG: // (-1 / ( 1 + R^2 )) * dR
            {
                res = MUL ( DIV ( n(-1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) );
            }
            break;

            case SH: // ch(R) * dR
            {
                res = MUL ( CH ( c(R) ) , d(R) );
            }
            break;

            case CH: // sh(R) * dR
            {
                res = MUL ( SH ( c(R) ) , d(R) );
            }
            break;

            case TH: // (1 / ( ch(R) ^ 2 )) * dR 
            {
                res = MUL ( DIV ( n(1), POW ( CH ( c(R) ), n(2) ) ) , d(R) );
            }
            break;

            case CTH: // (-1 / ( sh(R) ^ 2 )) * dR 
            {
                res = MUL ( DIV ( n(-1), POW ( SH ( c(R) ), n(2) ) ) , d(R) );
            }
            break;

            case ABS: // sign(R) * dR
            {
                res = MUL ( SIGN ( c(R) ), d(R) );
            }
            break;

            case SIGN: // 0
            {
                res = n(0);
            }
            break;
        
            default:
            {
                assert (0);
            }
            break;
        }
    }

    // fprintf (latex, "\\begin{math}\n\t(");

    // getNodeLaTeX (node, latex);

    // fprintf (latex, ")' = ");

    // getNodeLaTeX (res, latex);

    // fprintf (latex, "\n\\end{math}\\\\\n");

    return (res);
}

Node<Token> * getRoot (Node<Token> * node)
{
    if (node->parent)
    {
        return (getRoot(node->parent));
    }

    return (node);
}