#include "headers.hpp"

#include "operations.hpp"

#include "get_pic.hpp"

void getPic (BinaryTree<Token> * expression, const char * name, bool open)
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

void getNodePic (Node<Token> * node, FILE * f)
{
    fprintf (f, "\tnode%p [shape = \"%s\" label = \"", N, ((TYPE(N) == OP_TYPE) ? "circle" : "square"));

    PrintToken (f, N);

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