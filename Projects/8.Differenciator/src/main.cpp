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

int main ()
{
    const char working_with[PATH_MAX] = "exp4";

    BinaryTree<Token> expression = {};

    char init_path[PATH_MAX] = "";

    sprintf (init_path, "data/%s", working_with);

    initExpression (&expression, init_path);

    expression.dotDump (PrintToken, 1);

    BinaryTree<Token> * diff_expression = DiffExpression (&expression, 'x');

    expression.dotDump (PrintToken, 2);
    diff_expression->dotDump (PrintToken, 3);

    getPic (diff_expression, "not_simple_diff_pic");

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    diff_expression->dotDump (PrintToken, 4);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    getLaTeX (&expression, diff_expression, working_with);

    expression.clear();
    diff_expression->clear();

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

