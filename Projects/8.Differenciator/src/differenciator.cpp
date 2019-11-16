#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "../../../Include/ms_stack.h"

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
    SIN,
    COS,
    TG,
    CTG,
    LN,
    EXP,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,
    SH,
    CH,
    TH,
    CTH,
    ABS,
    SIGN,
    DIV,
    POW,
    DIFF
};

const char * op_names[23] = {
                                "@",
                                "+",
                                "-",
                                "*",
                                "sin",
                                "cos",
                                "tg",
                                "ctg",
                                "ln",
                                "exp",
                                "arcsin",
                                "arccos",
                                "arctg",
                                "arcctg",
                                "sh",
                                "ch",
                                "th",
                                "cth",
                                "abs",
                                "sign",
                                "/",
                                "^",
                                "d"
                            };

void PrintMonomDot (FILE * f, const void * ptr);

void getPic   (BinaryTree<Monomial> * expression, const char * name, bool open = false);
void getNodePic (Node<Monomial> * node, FILE * f);

void getLaTeX (BinaryTree<Monomial> * expression, BinaryTree<Monomial> * diff, const char * name, bool open = false);
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

BinaryTree<Monomial> * DiffExpression (const BinaryTree<Monomial> * expression, const char var);
Node<Monomial> * Diff (Node<Monomial> * node, const char var);

Node<Monomial> * Simplify (BinaryTree<Monomial> * exp, Node<Monomial> * node);
Node<Monomial> * SimplifyConst (BinaryTree<Monomial> * exp, Node<Monomial> * node);

int Count (const char op, int r);
int Count (const char op, int l, int r);
int BinaryPow (int x, int a);

int main ()
{
    BinaryTree<Monomial> expression = {};

    initExpression (&expression, "data/exp1");

    expression.dotDump (PrintMonomDot, 1);

    BinaryTree<Monomial> * diff_expression = DiffExpression (&expression, 'x');

    expression.dotDump (PrintMonomDot, 3);
    diff_expression->dotDump (PrintMonomDot, 2);

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    expression.dotDump (PrintMonomDot, 3);
    diff_expression->dotDump (PrintMonomDot, 4);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    getLaTeX (&expression, diff_expression, "hello");

    return (0);
}

void PrintMonomDot (FILE * f, const void * ptr)
{
    switch (((Node<Monomial> *) ptr)->data.type)
    {
        case OP_TYPE:
        {
            fprintf (f, "%s", op_names[((Node<Monomial> *) ptr)->data.data]);
            printf ("%s\n", op_names[((Node<Monomial> *) ptr)->data.data]);
        }
        break;

        case NUM_TYPE:
        {
            fprintf (f, "%d", ((Node<Monomial> *) ptr)->data.data);
            printf ("%d\n", ((Node<Monomial> *) ptr)->data.data);
        }
        break;

        case VAR_TYPE:
        {
            fprintf (f, "%c", ((Node<Monomial> *) ptr)->data.data);
            printf ("%c\n", ((Node<Monomial> *) ptr)->data.data);
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
    fprintf (f, "\tnode%p [shape = \"%s\" label = \"", N, ((TYPE(N) == OP_TYPE) ? "circle" : "square"));

    PrintMonomDot (f, N);

    switch (TYPE(N))
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

    if (L)
    {
        getNodePic (L, f);
        fprintf (f, "\tnode%p -> node%p\n", N, L);
    }

    if (R)
    {
        getNodePic (R, f);
        fprintf (f, "\tnode%p -> node%p\n", N, R);
    }
}

void getLaTeX (BinaryTree<Monomial> * expression, BinaryTree<Monomial> * diff, const char * name, bool open)
{
    system("mkdir -p LaTeX");

    system("mkdir -p LaTeX/tmp");

	FILE *log = fopen("LaTeX/tmp/temp.tex", "w");

    fprintf (log, "\\documentclass[a4paper,12pt]{article}\n");

    fprintf (log, "\\begin{document}\n$$(");

    getNodeLaTeX (expression->root, log);

    fprintf (log, ")' = ");

    getNodeLaTeX (diff->root, log);

    fprintf (log, "$$\n\\end{document}\n");

    fclose (log);

    system ("pdflatex LaTeX/tmp/temp.tex");
}

void getNodeLaTeX (Node<Monomial> * node, FILE * f)
{
    bool low_priority = false;

    if (N->parent && TYPE(N->parent) == OP_TYPE &&
        TYPE(N) == OP_TYPE && DATA(N) < DATA(N->parent))
    {
        low_priority = true;
    }
    else if (TYPE(N) == NUM_TYPE && DATA(N) < 0)
    {
        low_priority = true;
    }

    if (low_priority)
    {
        fprintf (f, "(");
    }

    if (TYPE(N) != OP_TYPE)
    {
        if (TYPE(N) == NUM_TYPE)
        {
            fprintf (f, "%d", DATA(N));
        }
        else
        {
            fprintf (f, "%c", DATA(N));
        }
    }
    else
    {
        switch (DATA(N))
        {
            case ADD:
            {
                getNodeLaTeX (L, f);
                fprintf (f, " + ");
                getNodeLaTeX (R, f);
            }
            break;

            case SUB:
            {
                getNodeLaTeX (L, f);
                fprintf (f, " - ");
                getNodeLaTeX (R, f);
            }
            break;

            case DIV:
            {
                fprintf (f, " \\frac{");
                getNodeLaTeX (L, f);
                fprintf (f, "}{");
                getNodeLaTeX (R, f);
                fprintf (f, "} ");
            }
            break;

            case MUL:
            {
                getNodeLaTeX (L, f);
                fprintf (f, " \\cdot ");
                getNodeLaTeX (R, f);
            }
            break;

            case POW:
            {
                getNodeLaTeX (L, f);
                fprintf (f, " ^{");
                getNodeLaTeX (R, f);
                fprintf (f, "} ");
            }
            break;

            case ABS:
            {
                fprintf (f, " |");
                getNodeLaTeX (R, f);
                fprintf (f, "| ");
            }
            break;

            case SIGN:
            {
                fprintf (f, " sign (");
                getNodeLaTeX (R, f);
                fprintf (f, ") ");
            }
            break;

            case EXP:
            {
                fprintf (f, " e^{");
                getNodeLaTeX (R, f);
                fprintf (f, "} ");
            }
            break;

            case DIFF:
            {
                getNodeLaTeX (L, f);
                fprintf (f, "'_{");
                getNodeLaTeX (R, f);
                fprintf (f, "} ");
            }
            break;
        
            default:
            {
                fprintf (f, " \\%s (", op_names[DATA(N)]);
                getNodeLaTeX (R, f);
                fprintf (f, ") ");
            }
            break;
        }
    }

    if (low_priority)
    {
        fprintf (f, ")");
    }
}

Node<Monomial> * Simplify (BinaryTree<Monomial> * exp, Node<Monomial> * node)
{
    if (L)
    {
        L = Simplify (exp, L);
        L->parent = N;
    }

    if (R)
    {
        R = Simplify (exp, R);
        R->parent = N;
    }

    if (TYPE(N) != OP_TYPE)
    {
        return (N);
    }

    Node<Monomial> * tmp = nullptr;
    char op = 0;
    int l_val = 0;
    int r_val = 0;

    if ((R && TYPE(R) == NUM_TYPE) && (L && TYPE(L) == NUM_TYPE))
    {
        op = DATA(N);
        l_val = DATA(L);
        r_val = DATA(R);
        exp->deleteSubtree (N);
        return (n(Count (op, l_val, r_val)));
    }

    //ZERO
    if ((DATA(N) == ADD || DATA(N) == SUB) && RIGHT (0))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (DATA(N) == ADD && LEFT (0))
    {
        tmp = c(R);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (DATA(N) == SUB && LEFT (0))
    {
        tmp = c(R);
        exp->deleteSubtree (N);
        return (MUL( n(-1), tmp ));
    }
    if ((DATA(N) == MUL || DATA(N) == DIV) && (LEFT (0) || RIGHT (0)))
    {
        exp->deleteSubtree (N);
        return (n(0));
    }
    if (DATA(N) == POW && LEFT (0))
    {
        exp->deleteSubtree (N);
        return (n(0));
    }
    if (DATA(N) == POW && RIGHT (0))
    {
        exp->deleteSubtree (N);
        return (n(1));
    }

    //ONE

    if (DATA(N) == MUL && LEFT (1))
    {
        tmp = c(R);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (DATA(N) == MUL && RIGHT (1))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (DATA(N) == DIV && RIGHT (1))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }
    if (DATA(N) == POW && LEFT (1))
    {
        exp->deleteSubtree (N);
        return (n(1));
    }
    if (DATA(N) == POW && RIGHT (1))
    {
        tmp = c(L);
        exp->deleteSubtree (N);
        return (tmp);
    }

    //

    if (((DATA(N) == ADD || DATA(N) == MUL) && 
          L && R && TYPE(L) == OP_TYPE && TYPE(R) == OP_TYPE &&
          DATA(L) < DATA(R)) ||
        (DATA(N) == MUL && TYPE(R) == NUM_TYPE))
    {
        tmp = L;
        L = R;
        R = tmp;

        return (N);
    }

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