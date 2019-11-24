#include "headers.hpp"

#include "get_latex.hpp"

#define PRINT( s )                               \
        fprintf (f, s)

#define MATH_OP( f )                    \
        PRINT ("\\begin{math}\n\t")

#define MATH_CL( f )                    \
        PRINT ("\\end{math}\\\\\n")

#define INTRODUCE_DIF( node, f )                \
        PRINT ("Найдем производную $(");        \
        getNodeLaTeX (node, f);                 \
        PRINT (")'$\\\\\n")                

void latex_num (Node<Token> * node, FILE * f)
{
    PRINT ("$(");
    getNodeLaTeX (node, f);
    PRINT (")' = 0$\\\\");
}

void latex_var (Node<Token> * node, FILE * f)
{
    PRINT ("$(");
    getNodeLaTeX (node, f);
    PRINT (")' = 1$\\\\");
}

void latex_add   (Node<Token> * node, FILE * f)
{
    INTRODUCE_DIF (node, f);
    fprintf (f, "Раскроем как производную суммы:\\\\\n");

    MATH_OP (f);
    fprintf (f, "(");

    getNodeLaTeX (N, f);

    fprintf (f, ")' = (");

    getNodeLaTeX (L, f);

    fprintf (f, ")' + (");

    getNodeLaTeX (R, f);

    fprintf (f, ")'\n"); 
    MATH_CL(f);
}


void latex_sub   (Node<Token> * node, FILE * f)
{
    INTRODUCE_DIF (node, f);
    fprintf (f, "Раскроем как производную разности:\\\\\n");

    MATH_OP (f);
    fprintf (f, "(");

    getNodeLaTeX (N, f);

    fprintf (f, ")' = (");

    getNodeLaTeX (L, f);

    fprintf (f, ")' - (");

    getNodeLaTeX (R, f);

    fprintf (f, ")'\n");
    MATH_CL (f);
}


void latex_mul   (Node<Token> * node, FILE * f)
{
    INTRODUCE_DIF (node, f);
    fprintf (f, "Раскроем как производную произведения:\\\\\n");

    MATH_OP (f);
    fprintf (f, "(");

    getNodeLaTeX (N, f);

    fprintf (f, ")' = (");

    getNodeLaTeX (L, f);

    fprintf (f, ")' \\cdot (");

    getNodeLaTeX (R, f);

    fprintf (f, ") + (");

    getNodeLaTeX (L, f);

    fprintf (f, ") \\cdot (");

    getNodeLaTeX (R, f);

    fprintf (f, ")'\n");
    MATH_CL (f);
}

void latex_div   (Node<Token> * node, FILE * f)
{
    INTRODUCE_DIF (node, f);
    fprintf (f, "Раскроем как производную частного:\\\\\n");

    MATH_OP (f);
    fprintf (f, "(");

    getNodeLaTeX (N, f);

    fprintf (f, ")' = \\frac{(");

    getNodeLaTeX (L, f);

    fprintf (f, ")' \\cdot (");

    getNodeLaTeX (R, f);

    fprintf (f, ") - (");

    getNodeLaTeX (L, f);

    fprintf (f, ") \\cdot (");

    getNodeLaTeX (R, f);

    fprintf (f, ")'}{(");

    getNodeLaTeX (R, f);

    fprintf (f, ")^2}\n");
    MATH_CL (f);
}

// void latex_add   (Node<Token> * node, FILE * f)
// {
//     INTRODUCE_DIF (node, f);
//     fprintf (f, "Раскроем как производную cтепени:\\\\\n");

//     MATH_OP (f);
//     fprintf (f, "(");

//     getNodeLaTeX (N, f);

//     fprintf (f, ")' = (");

//     getNodeLaTeX (L, f);

//     fprintf (f, ")^{(");

//     getNodeLaTeX (R, f);

//     fprintf (f, ")} \\cdot (ln(");

//     getNodeLaTeX (L, f);

//     fprintf (f, ") \\cdot (");

//     getNodeLaTeX (R, f);

//     fprintf (f, ")' + \\frac ("); 
//     MATH_CL(f);
// }