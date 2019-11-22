#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "differenciator.hpp"
#include "get_pic.hpp"
#include "get_latex.hpp"
#include "simplify.hpp"

void initExpression (BinaryTree<Token> * expression, const char * path);
void initToken      (Node<Token> ** node, char ** cur);

void InitNum (Node<Token> * node, char ** cur);
void InitVar (Node<Token> * node, char ** cur);
void InitOp  (Node<Token> * node, char ** cur);

int ParseOperation (const char * op);


void parseGeneral (BinaryTree<Token> * expression, const char * path);
Node<Token> * parseExpression (char ** cur);
Node<Token> * parseToken (char ** cur);
Node<Token> * parsePrimary (char ** cur);
Node<Token> * parseFunc (char ** cur);
Node<Token> * parseNum (char ** cur);

int main ()
{
    const char working_with[PATH_MAX] = "exp8";

    BinaryTree<Token> expression = {};

    char init_path[PATH_MAX] = "";

    sprintf (init_path, "data/%s", working_with);

    parseGeneral (&expression, init_path);

    expression.dotDump (PrintToken, 1);

    BinaryTree<Token> * diff_expression = DiffWithLaTeX (&expression, 'x', working_with);

    expression.dotDump (PrintToken, 2);
    diff_expression->dotDump (PrintToken, 3);

    getPic (diff_expression, "not_simple_diff_pic");

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    diff_expression->dotDump (PrintToken, 4);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    //getLaTeX (&expression, diff_expression, working_with);

    expression.clear();
    diff_expression->clear();

    return (0);
}

void parseGeneral (BinaryTree<Token> * expression, const char * path)
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

    expression->root = parseExpression (&cur);
}

Node<Token> * parseExpression (char ** cur)
{
    char op = 0;

    Node<Token> * left = parseToken (cur);

    while (**cur == '+' || **cur == '-')
    {
        op = **cur;
        (*cur)++;

        Node<Token> * right = parseToken (cur);

        if (op == '+')
        {
            left = ADD (left, right);
        }
        else
        {
            left = SUB (left, right);
        }
    }

    return (left);
}

Node<Token> * parseToken (char ** cur)
{
    char op = 0;

    Node<Token> * left = parsePrimary (cur);

    while (**cur == '*' || **cur == '\\')
    {
        op = **cur;
        (*cur)++;

        Node<Token> * right = parsePrimary (cur);

        if (op == '*')
        {
            left = MUL(left, right);
        }
        else
        {
            left = DIV(left, right);
        }
    } 

    return (left);
}

Node<Token> * parsePrimary (char ** cur)
{
    if (**cur == '(')
    {
        (*cur)++;

        Node<Token> * res = parseExpression (cur);

        if (**cur != ')')
        {
            printf ("syntax error!!! %s\n", *cur);
        }

        (*cur)++;

        return (res);
    }

    if (isalpha (**cur))
    {
        return (parseFunc (cur));
    }

    return (parseNum (cur));
}

Node<Token> * parseFunc (char ** cur)
{
    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (!strncmp (*cur, operations[i].name, operations[i].name_len))
        {
            (*cur) += operations[i].name_len;
            return (CreateNode (-1, i, '~', OP_TYPE, parsePrimary(cur)));
        }
    }

    Node<Token> * res = v(**cur);

    (*cur)++;

    return (res);
}

Node<Token> * parseNum (char ** cur)
{
    double val  = 0;
    size_t skip = 0;

    sscanf (*cur, "%lf%n", &val, &skip);

    (*cur) += skip;

    return (n(val));
}



// Node<Token> * ParseToken (char ** cur, size_t parent_pr)
// {
//     assert (cur != nullptr && *cur != nullptr);

//     Node<Token> * node = (Node<Token> *) calloc (1, sizeof (*node));

//     node->init ();

//     printf ("init left\n");
//     node->left = initToken (cur, parent_pr);
//     node->left->parent = node;
    
//     if (isdigit (**cur) || (**cur == '-' && isdigit (*(*cur + 1))))
//     {
//         InitNum (node, cur);
//     }
//     else if (isalpha (**cur) && *(*cur + 1) == ')') 
//     {
//         InitVar (node, cur);
//     }
//     else
//     {
//         InitOp (node, cur);
//     }

//     printf ("New token:\n");
//     printf ("\tdata: %.2lf\n", node->data.data);
//     printf ("\tvar: %c\n", node->data.var);
//     printf ("\top: %d\n", node->data.op_id);

//     if (**cur == '(')
//     {
//         printf ("init right\n");
//         initToken (&(*node)->right, cur);
//         (*node)->right->parent = *node;
//     }

//     if (**cur != ')')
//     {
//         printf ("wrong format!\n");
//         return;
//     }
    
//     (*cur)++;

//     printf (")\n");
// }

// Node<Token> * parseBraces (char ** cur, size_t parent_pr)
// {
//     Node<Token> * res = nullptr;

//     if (**cur == '(')
//     {
//         (*cur)++;

//         res = initToken (cur);

//         if (**cur != ')')
//         {
//             printf ("Unclosed braces at: %s\n", *cur);
//             return (nullptr);
//         }

//         return (res);
//     }

//     if (**cur == '|')
//     {
//         (*cur)++;

//         res = initToken (cur);

//         if (!res)
//         {
//             return (res);
//         }

//         if (**cur != '|')
//         {
//             printf ("Unclosed abs at: %s\n", *cur);
//             return (nullptr);
//         }

//         return (CreateNode (-1, ABS, '#', OP_TYPE, res));
//     }

//     assert (0);
// }


// int parseOperator (const char * cur)
// {
//     char * op = (char *) calloc (10, sizeof (*op));

//     sscanf (cur, "%[^()|0123456789]", op);

//     size_t len = strlen (op);

//     for (size_t i = 0; i < OP_CNT; i++)
//     {
//         if (len == operations[i].name_len && 
//             strncmp (op, operations[i].name, len))
//         {
//             free (op);
//             return (i);
//         }
//     }

//     return (0);
// }

// Node<Token> * parseAtom (char ** cur)
// {
//     if (isalpha (**cur))
//     {
//         char var = 0;

//         sscanf (*cur, "%c", &var);

//         (*cur)++;

//         return (CreateNode (-1, UNDEF, var, VAR_TYPE));
//     }
//     else if (isdigit (**cur) || ( **cur == '-' && isdigit (*(*cur + 1)) ))
//     {
//         double data = 0;
//         size_t skip = 0;

//         sscanf (*cur, "%lf%n", &data, &skip);

//         *cur += skip;

//         return (CreateNode (data, UNDEF, '#', NUM_TYPE));
//     }
//     else if (isalpha (**cur))
//     {
//         int op_id = ParseFunc (cur);
//     }
//     else
//     {
//         printf ("Can't parse at: %s\n", *cur);
//         return (nullptr);
//     }
//}


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

