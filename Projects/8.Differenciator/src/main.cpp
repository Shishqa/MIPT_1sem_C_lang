#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "get_pic.hpp"
#include "get_latex.hpp"
#include "simplify.hpp"

void initExpression (BinaryTree<Token> * expression, const char * path);
void initToken      (Node<Token> ** node, char ** cur);

void InitNum (Node<Token> * node, char ** cur);
void InitVar (Node<Token> * node, char ** cur);
void InitOp  (Node<Token> * node, char ** cur);

int ParseOperation (const char * op);

BinaryTree<Token> * DiffExpression (const BinaryTree<Token> * expression, const char var);
Node<Token> * Diff (Node<Token> * node, const char var);

int main ()
{
    BinaryTree<Token> expression = {};

    initExpression (&expression, "data/exp6");

    expression.dotDump (PrintToken, 1);

    BinaryTree<Token> * diff_expression = DiffExpression (&expression, 'x');

    expression.dotDump (PrintToken, 2);
    diff_expression->dotDump (PrintToken, 3);

    getPic (diff_expression, "not_simple_diff_pic");

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    diff_expression->dotDump (PrintToken, 4);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    getLaTeX (&expression, diff_expression, "hello");

    return (0);
}

void initExpression (BinaryTree<Token> * expression, const char * path)
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

    initToken (&expression->root, &cur);
}

void initToken (Node<Token> ** node, char ** cur)
{
    assert (node != nullptr && *node == nullptr);
    assert (cur != nullptr && *cur != nullptr);

    if (**cur == '(')
    {
        printf ("(\n");

        (*cur)++;
        *node = (Node<Token> *) calloc (1, sizeof (**node));

        (*node)->init ();

        if (**cur == '(')
        {
            printf ("init left\n");
            initToken (&(*node)->left, cur);
            (*node)->left->parent = *node;
        }
        
        if (isdigit (**cur) || (**cur == '-' && isdigit (*(*cur + 1))))
        {
            InitNum (*node, cur);
        }
        else if (isalpha (**cur) && *(*cur + 1) == ')') 
        {
            InitVar (*node, cur);
        }
        else
        {
            InitOp (*node, cur);
        }

        printf ("New token:\n");
        printf ("\tdata: %.2lf\n", (*node)->data.data);
        printf ("\tvar: %c\n", (*node)->data.var);
        printf ("\top: %d\n", (*node)->data.op_id);

        if (**cur == '(')
        {
            printf ("init right\n");
            initToken (&(*node)->right, cur);
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

void InitNum (Node<Token> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);

    size_t skip = 0;

    TYPE(N) = NUM_TYPE;
    sscanf (*cur, "%lf%n", &(node->data.data), &skip);

    node->data.op_id = 0;
    node->data.var = '~';

    *cur += skip;
}

void InitVar (Node<Token> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);

    TYPE(N) = VAR_TYPE;
    sscanf (*cur, "%c", &(node->data.var));

    node->data.data = -1;
    node->data.op_id = 0;

    (*cur)++;
}

void InitOp (Node<Token> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);
    
    size_t skip = 0;

    TYPE(N) = OP_TYPE;

    char * op = (char *) calloc (10, sizeof (*op));

    sscanf (*cur, "%[^()]%n", op, &skip);

    node->data.op_id = ParseOperation (op);

    node->data.data = -1;
    node->data.var = '~';

    assert (OPCODE(N) != UNDEF);

    *cur += skip;

    free (op);
}

int ParseOperation (const char * op)
{
    assert (op != nullptr);

    size_t op_len = strlen (op);

    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (op_len == operations[i].name_len && 
            !strncmp (op, operations[i].name, op_len))
        {
            return (i);
        }
    }

    return (0);
}

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