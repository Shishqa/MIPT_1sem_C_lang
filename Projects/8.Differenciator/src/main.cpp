#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "get_pic.hpp"
#include "get_latex.hpp"
#include "simplify.hpp"

void initExpression (BinaryTree<Monomial> * expression, const char * path);
void initMonomial   (Node<Monomial> ** node, char ** cur);

void InitNum (Node<Monomial> * node, char ** cur);
void InitVar (Node<Monomial> * node, char ** cur);
void InitOp  (Node<Monomial> * node, char ** cur);

int ParseOperation (const char * op);

BinaryTree<Monomial> * DiffExpression (const BinaryTree<Monomial> * expression, const char var);
Node<Monomial> * Diff (Node<Monomial> * node, const char var);

int main ()
{
    BinaryTree<Monomial> expression = {};

    initExpression (&expression, "data/exp1");

    BinaryTree<Monomial> * diff_expression = DiffExpression (&expression, 'x');

    getPic (diff_expression, "not_simple_diff_pic");

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    getLaTeX (&expression, diff_expression, "hello");

    return (0);
}

void initExpression (BinaryTree<Monomial> * expression, const char * path)
{
    FILE * f = fopen (path, "r");

    if (!f)
    {
        printf ("Can't open %s\n", path);
        return;
    }

    char * buff = nullptr;
    Read (&buff, f);

    fclose (f);

    char * cur = buff;

    printf ("%s\n", cur);

    initMonomial (&expression->root, &cur);
}

void initMonomial (Node<Monomial> ** node, char ** cur)
{
    assert (node != nullptr && *node == nullptr);
    assert (cur != nullptr && *cur != nullptr);

    if (**cur == '(')
    {
        printf ("(\n");

        (*cur)++;
        *node = (Node<Monomial> *) calloc (1, sizeof (**node));

        (*node)->init ();

        if (**cur == '(')
        {
            printf ("init left\n");
            initMonomial (&(*node)->left, cur);
            (*node)->left->parent = *node;
        }
        
        if (isdigit (**cur) || (**cur == '-' && isdigit (*(*cur + 1))))
        {
            InitNum (*node, cur);
            printf ("num %d\n", (*node)->data.data);
        }
        else if (isalpha (**cur) && *(*cur + 1) == ')')  // КОСТЫЛЬ
        {
            InitVar (*node, cur);
            printf ("var %c\n", (*node)->data.data);
        }
        else
        {
            InitOp (*node, cur);
            printf ("op %d\n", (*node)->data.data);
        }

        if (**cur == '(')
        {
            printf ("init right\n");
            initMonomial (&(*node)->right, cur);
            (*node)->right->parent = *node;
        }

        if (**cur != ')')
        {
            printf ("wrong format!\n");
            return;
        }
        
        (*cur)++;

        printf (")\n");
    }
    else
    {
        printf ("wrong format!\n");
        return;
    }
}

void InitNum (Node<Monomial> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);

    size_t skip = 0;

    TYPE(N) = NUM_TYPE;
    sscanf (*cur, "%d%n", &(DATA(N)), &skip);

    *cur += skip;
}

void InitVar (Node<Monomial> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);

    TYPE(N) = VAR_TYPE;
    sscanf (*cur, "%c", &(DATA(N)));

    (*cur)++;
}

void InitOp (Node<Monomial> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);
    
    size_t skip = 0;

    TYPE(N) = OP_TYPE;

    char * op = (char *) calloc (10, sizeof (*op));

    sscanf (*cur, "%[^()]%n", op, &skip);

    DATA(N) = ParseOperation (op);

    assert (DATA(N) != UNDEF);

    *cur += skip;

    free (op);
}

int ParseOperation (const char * op)
{
    assert (op != nullptr);

    size_t op_len = strlen (op);

    switch (op_len)
    {
        CASE (1, {
                    PARSE (MUL, 1)
                    PARSE (DIV, 1)
                    PARSE (ADD, 1)
                    PARSE (SUB, 1)
                    PARSE (POW, 1)
                 })

        CASE (2, {
                    PARSE (LN, 2)
                    PARSE (TG, 2)
                    PARSE (SH, 2)
                    PARSE (CH, 2)
                    PARSE (TH, 2)
                 })

        CASE (3, {
                    PARSE (SIN, 3)
                    PARSE (COS, 3)
                    PARSE (CTG, 3)
                    PARSE (ABS, 3)
                    PARSE (EXP, 3)
                 })

        CASE (4, {
                    PARSE (SIGN, 4)
                 })

        CASE (5, {
                    PARSE (ARCTG, 5)
                 })

        CASE (6, {
                    PARSE (ARCSIN, 6)
                    PARSE (ARCCOS, 6)
                    PARSE (ARCCTG, 6)
                 })
    
        default:
        {
            return (UNDEF);
        }
        break;
    }

    assert (0);
}

BinaryTree<Monomial> * DiffExpression (const BinaryTree<Monomial> * expression, const char var)
{
    assert (expression != nullptr);

    BinaryTree<Monomial> * diff = (BinaryTree<Monomial> *) calloc (1, sizeof (*diff));

    assert (diff != nullptr);

    diff->init ();

    diff->root = Diff (expression->root, var);

    return (diff);
}

Node<Monomial> * Diff (Node<Monomial> * node, const char var)
{
    assert (node != nullptr);

    if (TYPE(N) == NUM_TYPE)
    {
        return (n(0));
    }
    else if (TYPE(N) == VAR_TYPE && DATA(N) == var)
    {
        return (n(1));
    }
    else if (TYPE(N) == VAR_TYPE)
    {
        return (DIFF (v(DATA(N)), v(var)));
    }
    else
    {
        switch (DATA(N))
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

            case EXP:
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

            case TG:
            {
                return (MUL ( DIV ( n(1), POW ( COS ( c(R) ), n(2) ) ), d(R) ));
            }
            break;

            case CTG:
            {
                return (MUL ( DIV ( n(-1), POW ( SIN ( c(R) ), n(2) ) ) , d(R)));
            }
            break;

            case ARCSIN:
            {
                return (MUL ( DIV ( n(1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) ) );
            }
            break;

            case ARCCOS:
            {
                return (MUL ( DIV ( n(-1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) ) );
            }
            break;

            case ARCTG:
            {
                return (MUL ( DIV ( n(1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) ) );
            }
            break;

            case ARCCTG:
            {
                return (MUL ( DIV ( n(-1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) ) );
            }
            break;

            case SH:
            {
                return (MUL ( CH ( c(R) ) , d(R) ));
            }
            break;

            case CH:
            {
                return (MUL ( SH ( c(R) ) , d(R) ));
            }
            break;

            case TH:
            {
                return (MUL ( DIV ( n(1), POW ( CH ( c(R) ), n(2) ) ) , d(R) ));
            }
            break;

            case CTH:
            {
                return (MUL ( DIV ( n(-1), POW ( SH ( c(R) ), n(2) ) ) , d(R) ));
            }
            break;

            case ABS:
            {
                return (MUL ( SIGN ( c(R) ), d(R) ));
            }
            break;

            case SIGN:
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

