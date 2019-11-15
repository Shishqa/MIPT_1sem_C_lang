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
    LN,
    DIFF
};

const char * op_names[10] = {
                                "@",
                                "+",
                                "-",
                                "*",
                                "/",
                                "^",
                                "sin",
                                "cos",
                                "ln",
                                "d"
                            };

void PrintMonomDot (FILE * f, const void * ptr);

void getPic   (BinaryTree<Monomial> * expression, const char * name, bool open = false);
void getNodePic (Node<Monomial> * node, FILE * f);

void getLaTeX (BinaryTree<Monomial> * expression, const char * name, bool open = false);
void getNodeLaTeX (Node<Monomial> * node, FILE * f);

void initExpression (BinaryTree<Monomial> * expression, const char * path);
void initMonomial   (Node<Monomial> ** node, char ** cur);

void InitNum (Node<Monomial> * node, char ** cur);
void InitVar (Node<Monomial> * node, char ** cur);
void InitOp  (Node<Monomial> * node, char ** cur);

int ParseOperation (const char * op);

Node<Monomial> * CreateNode (const int data, const char type, Node<Monomial> * right = nullptr, 
                                                              Node<Monomial> * left  = nullptr);
Node<Monomial> * Copy (const Node<Monomial> * origin);

BinaryTree<Monomial> * DiffExpression (const BinaryTree<Monomial> * node, const char var);
Node<Monomial> * Diff (Node<Monomial> * node, const char var);

int main ()
{
    BinaryTree<Monomial> expression = {};

    initExpression (&expression, "data/exp1");

    expression.dotDump (PrintMonomDot, 1);

    BinaryTree<Monomial> * diff_expression = DiffExpression (&expression, 'x');

    diff_expression->dotDump (PrintMonomDot, 2);

    getPic (diff_expression, "my_first_pic");

    getLaTeX (diff_expression, "hello");

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

    if (node->data.type == NUM_TYPE)
    {
        return (n(0));
    }
    else if (node->data.type == VAR_TYPE && node->data.data == var)
    {
        return (n(1));
    }
    else if (node->data.type == VAR_TYPE)
    {
        return (DIFF (v(node->data.data), v(var)));
    }
    else
    {
        switch (node->data.data)
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
                              ADD ( MUL ( d(R), LN(L) ),
                                    MUL ( DIV ( d(L), c(L) ), c(R) )))); 
            }
            break;

            case LN: // (1 / R) * dR
            {
                return (MUL ( DIV ( n(1), c(R) ), d(R) )); 
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

    if (node->left)
    {
        node->left->parent = node;
    }

    if (node->right)
    {
        node->right->parent = node;
    }

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

void getPic (BinaryTree<Monomial> * expression, const char * name, bool open)
{
    system("mkdir -p pic");

    system("mkdir -p pic/pic_tmp");

	FILE *log = fopen("pic/pic_tmp/tempautogeneratedlog.gv", "w");

    fprintf (log, "digraph dump\n{\n");

    fprintf (log, "node [style = filled]\n");

    getNodePic (expression->root, log);

    fprintf (log, "\n}\n");

    fclose(log);

	char call_dot[100] = "";

    sprintf(call_dot, "dot -Tpng -opic/%s.png pic/pic_tmp/tempautogeneratedlog.gv", name);

	system(call_dot);

    char call_open[100] = "";

    if (open)
    {
        sprintf (call_open, "xdg-open pic/%s.png", name);

        //printf ("%s\n", call_open);

        system (call_open);
    }
}

void getNodePic (Node<Monomial> * node, FILE * f)
{
    fprintf (f, "\tnode%p [shape = \"%s\" label = \"", node, ((node->data.type == OP_TYPE) ? "circle" : "square"));

    PrintMonomDot (f, node);

    switch (node->data.type)
    {
        case OP_TYPE:
        {
            fprintf (f, "\", fillcolor = \"#fffacd\", fontsize = 18]\n");
        }
        break;

        case VAR_TYPE:
        {
            fprintf (f, "\", fillcolor = \"#9acd32\"]\n");
        }
        break;

        case NUM_TYPE:
        {
            fprintf (f, "\", fillcolor = \"#ffa500\"]\n");
        }
        break;
    
        default:
        break;
    }

    if (node->left)
    {
        getNodePic (node->left, f);
        fprintf (f, "\tnode%p -> node%p\n", node, node->left);
    }

    if (node->right)
    {
        getNodePic (node->right, f);
        fprintf (f, "\tnode%p -> node%p\n", node, node->right);
    }
}

void getLaTeX (BinaryTree<Monomial> * expression, const char * name, bool open)
{
    system("mkdir -p LaTeX");

    system("mkdir -p LaTeX/tmp");

	FILE *log = fopen("LaTeX/tmp/temp.tex", "w");

    fprintf (log, "\\documentclass{report}\n");
    //fprintf (log, "\\usepackage[cp1251]{inputenc}\n");
    //fprintf (log, "\\usepackage[russian]{babel}\n\n");

    fprintf (log, "\\begin{document}\n$$");

    getNodeLaTeX (expression->root, log);

    fprintf (log, "$$\n\\end{document}\n");

    fclose (log);

    system ("pdflatex LaTeX/tmp/temp.tex");
}

void getNodeLaTeX (Node<Monomial> * node, FILE * f)
{
    bool low_priority = false;

    if (node->parent && node->parent->data.type == OP_TYPE &&
        node->data.type == OP_TYPE && node->data.data < node->parent->data.data)
    {
        low_priority = true;
    }

    if (low_priority)
    {
        fprintf (f, "(");
    }

    if (node->data.type != OP_TYPE)
    {
        PrintMonomDot (f, node);
    }
    else
    {
        switch (node->data.data)
        {
            case ADD:
            {
                getNodeLaTeX (node->left, f);
                fprintf (f, " + ");
                getNodeLaTeX (node->right, f);
            }
            break;

            case SUB:
            {
                getNodeLaTeX (node->left, f);
                fprintf (f, " - ");
                getNodeLaTeX (node->right, f);
            }
            break;

            case DIV:
            {
                fprintf (f, " \\frac{");
                getNodeLaTeX (node->left, f);
                fprintf (f, "}{");
                getNodeLaTeX (node->right, f);
                fprintf (f, "} ");
            }
            break;

            case MUL:
            {
                getNodeLaTeX (node->left, f);
                fprintf (f, " \\cdot ");
                getNodeLaTeX (node->right, f);
            }
            break;

            case POW:
            {
                getNodeLaTeX (node->left, f);
                fprintf (f, " ^{");
                getNodeLaTeX (node->right, f);
                fprintf (f, "} ");
            }
            break;

            case SIN:
            {
                fprintf (f, " \\sin (");
                getNodeLaTeX (node->right, f);
                fprintf (f, ") ");
            }
            break;

            case COS:
            {
                fprintf (f, " \\cos (");
                getNodeLaTeX (node->right, f);
                fprintf (f, ") ");
            }
            break;

            case LN:
            {
                fprintf (f, " \\ln (");
                getNodeLaTeX (node->right, f);
                fprintf (f, ") ");
            }
            break;

            case DIFF:
            {
                getNodeLaTeX (node->left, f);
                fprintf (f, "'_{");
                getNodeLaTeX (node->right, f);
                fprintf (f, "} ");
            }
            break;
        
        default:
            break;
        }
    }

    if (low_priority)
    {
        fprintf (f, ")");
    }
}
