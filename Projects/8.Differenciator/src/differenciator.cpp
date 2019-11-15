#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "../../6.Binary_Tree/src/tree.h"
#include "monomial.hpp"

#include "DSL.hpp"

enum types
{
    OP_TYPE,
    NUM_TYPE,
    VAR_TYPE
};

enum operations
{
    UNDEF,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SIN,
    COS,
    LN
};

const char * op_names[9] = {
                                "@",
                                "+",
                                "-",
                                "*",
                                "/",
                                "^",
                                "ln",
                                "sin",
                                "cos"
                           };

void PrintMonomDot (FILE * f, const void * ptr);

void initExpression (BinaryTree<Monomial> * node, const char * path);
void initMonomial   (Node<Monomial> ** node, char ** cur);

void InitNum (Node<Monomial> * node, char ** cur);
void InitVar (Node<Monomial> * node, char ** cur);
void InitOp  (Node<Monomial> * node, char ** cur);

int ParseOperation (const char * op);

Node<Monomial> * CreateNode (const int data, const char type, Node<Monomial> * right = nullptr, 
                                                              Node<Monomial> * left  = nullptr);
Node<Monomial> * Copy (const Node<Monomial> * origin);

BinaryTree<Monomial> * DiffExpression (const BinaryTree<Monomial> * node);
Node<Monomial> * Diff (Node<Monomial> * node);

int main ()
{
    BinaryTree<Monomial> expression = {};

    initExpression (&expression, "data/exp1");

    expression.dotDump (PrintMonomDot, 1);

    BinaryTree<Monomial> * diff_expression = DiffExpression (&expression);

    diff_expression->dotDump (PrintMonomDot, 2);

    return (0);
}

void PrintMonomDot (FILE * f, const void * ptr)
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
        break;
    }
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
        
        if (isdigit (**cur))
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

    node->data.type = NUM_TYPE;
    sscanf (*cur, "%d%n", &(node->data.data), &skip);

    *cur += skip;
}

void InitVar (Node<Monomial> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);

    node->data.type = VAR_TYPE;
    sscanf (*cur, "%c", &(node->data.data));

    (*cur)++;
}

void InitOp (Node<Monomial> * node, char ** cur)
{
    assert (node != nullptr);
    assert (cur != nullptr && *cur != nullptr);
    
    size_t skip = 0;

    node->data.type = OP_TYPE;

    char * op = (char *) calloc (10, sizeof (*op));

    sscanf (*cur, "%[^()]%n", op, &skip);

    node->data.data = ParseOperation (op);

    assert (node->data.data != UNDEF);

    *cur += skip;

    free (op);
}

int ParseOperation (const char * op)
{
    assert (op != nullptr);

    size_t op_len = strlen (op);

    switch (op_len)
    {
        case 1:
        {
            if (*op == '*')
            {
                return (MUL);
            }
            else if (*op == '/')
            {
                return (DIV);
            }
            else if (*op == '+')
            {
                return (ADD);
            }
            else if (*op == '-')
            {
                return (SUB);
            }
            else if (*op == '^')
            {
                return (POW);
            }
            else
            {
                return (UNDEF);
            }
        }
        break;

        case 2:
        {
            if (!strncmp (op, "ln", 2))
            {
                return (LN);
            }
            else
            {
                return (UNDEF);
            }
        }
        break;

        case 3:
        {
            if (!strncmp (op, "sin", 3))
            {
                return (SIN);
            }
            else if (!strncmp (op, "cos", 3))
            {
                return (COS);
            }
            else
            {
                return (UNDEF);
            }
        }
        break;
    
        default:
        {
            return (UNDEF);
        }
        break;
    }

    assert (0);
}

BinaryTree<Monomial> * DiffExpression (const BinaryTree<Monomial> * expression)
{
    assert (expression != nullptr);

    BinaryTree<Monomial> * diff = (BinaryTree<Monomial> *) calloc (1, sizeof (*diff));

    assert (diff != nullptr);

    diff->init ();

    diff->root = Diff (expression->root);

    return (diff);
}

Node<Monomial> * Diff (Node<Monomial> * node)
{
    assert (node != nullptr);

    if (node->data.type == NUM_TYPE)
    {
        return (CreateNode (0, NUM_TYPE));
    }
    else if (node->data.type == VAR_TYPE)
    {
        return (CreateNode (1, NUM_TYPE));
    }
    else
    {
        switch (node->data.data)
        {
            case ADD:
            {
                return (ADD ( d(L), d(R) ));
            }
            break;

            case SUB:
            {
                return (SUB ( d(L), d(R) ));
            }
            break;

            case MUL:
            {
                return (ADD ( MUL( d(L), c(R) ), 
                              MUL( c(L), d(R) )));
            }
            break;

            case DIV:
            {
                return (DIV ( SUB ( MUL ( d(L), c(R) ), 
                                    MUL ( c(L), d(R) )), 
                              POW ( c(R), v(2) )));
            }
        
            default:
            break;
        }
    }
    
}

Node<Monomial> * CreateNode (const int data, const char type, Node<Monomial> * right, Node<Monomial> * left)
{
    Node<Monomial> * node = (Node<Monomial> *) calloc (1, sizeof (*node));

    assert (node != nullptr);

    node->init ();

    node->data.data = data;
    node->data.type = type;

    node->right = right;
    node->left = left;

    assert (node != nullptr);

    return (node);
}

Node<Monomial> * Copy (const Node<Monomial> * origin)
{
    if (origin == nullptr)
    {
        return (nullptr);
    }

    return (CreateNode (origin->data.data, origin->data.type, Copy (origin->right), Copy (origin->left)));
}
