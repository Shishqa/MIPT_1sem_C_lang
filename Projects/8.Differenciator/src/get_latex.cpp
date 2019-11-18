#include "headers.hpp"

#include "operations.hpp"

#include "get_latex.hpp"

void getLaTeX (BinaryTree<Token> * expression, BinaryTree<Token> * diff, const char * name, bool open)
{
    system("mkdir -p LaTeX");

    char call_mkdir[PATH_MAX] = {};

    sprintf (call_mkdir, "mkdir -p LaTeX/%s", name);

    system(call_mkdir);

    char path_file[PATH_MAX] = {};

    sprintf (path_file, "LaTeX/%s/%s.tex", name, name); 

	FILE *log = fopen(path_file, "w");

    fprintf (log, "\\documentclass[a4paper,12pt]{article}\n");

    fprintf (log, "\\begin{document}\n\\begin{math}\n\t(");

    getNodeLaTeX (expression->root, log);

    fprintf (log, ")' = ");

    getNodeLaTeX (diff->root, log);

    fprintf (log, "\\end{math}\n\\end{document}\n");

    fclose (log);

    char call_tex[PATH_MAX] = {};

    sprintf (call_tex, "pdflatex -interaction nonstopmode -output-directory LaTeX/%s LaTeX/%s/%s.tex", name, name, name); 

    system (call_tex);
}

void getNodeLaTeX (Node<Token> * node, FILE * f)
{
    bool low_priority = false;

    if (N->P && TYPE(N->P) == OP_TYPE &&
        TYPE(N) == OP_TYPE && PRIORITY(N) < PRIORITY(N->P))
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
        PrintToken (f, &(N->data));
    }
    else
    {
        switch (OPCODE(N))
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
                fprintf (f, " %s (", operations[node->data.op_id].name);
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