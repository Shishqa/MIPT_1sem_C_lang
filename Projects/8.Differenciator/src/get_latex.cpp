#include "headers.hpp"

#include "operations.hpp"

#include "get_latex.hpp"

void getLaTeX (BinaryTree<Monomial> * expression, BinaryTree<Monomial> * diff, const char * name, bool open)
{
    system("mkdir -p LaTeX");

    char call_mkdir[PATH_MAX] = {};

    sprintf (call_mkdir, "mkdir -p LaTeX/%s", name);

    system(call_mkdir);

    char path_file[PATH_MAX] = {};

    sprintf (path_file, "LaTeX/%s/%s.tex", name, name); 

	FILE *log = fopen(path_file, "w");

    fprintf (log, "\\documentclass[a4paper,12pt]{article}\n");

    fprintf (log, "\\begin{document}\n$$(");

    getNodeLaTeX (expression->root, log);

    fprintf (log, ")' = ");

    getNodeLaTeX (diff->root, log);

    fprintf (log, "$$\n\\end{document}\n");

    fclose (log);

    char call_tex[PATH_MAX] = {};

    sprintf (call_tex, "pdflatex -output-directory LaTeX/%s LaTeX/%s/%s.tex", name, name, name); 

    system (call_tex);
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