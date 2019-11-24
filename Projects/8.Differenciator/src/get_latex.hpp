#ifndef GET_LATEX_HPP_
#define GET_LATEX_HPP_

#define PRIORITY( node )                        \
        operations[node->data.op_id].priority

#include "headers.hpp"

FILE * initLaTeX  (const char * name);
void closeLaTeX   (FILE * latex);
void compileLaTeX (const char * name);
void getLaTeX     (BinaryTree<Token> * expression, BinaryTree<Token> * diff, const char * name, bool open = false);
void getNodeLaTeX (Node<Token> * node, FILE * f);
void getExpressionLaTeX (Node<Token> * node, FILE * f);

void latex_num   (Node<Token> * node, FILE * f);
void latex_var   (Node<Token> * node, FILE * f);
void latex_add   (Node<Token> * node, FILE * f);
void latex_sub   (Node<Token> * node, FILE * f);
void latex_mul   (Node<Token> * node, FILE * f);
void latex_div   (Node<Token> * node, FILE * f);


// static void latex_pow   (Node<Token> * node, FILE * f);
// static void latex_abs   (Node<Token> * node, FILE * f);
// static void latex_sign  (Node<Token> * node, FILE * f);
// static void latex_ln    (Node<Token> * node, FILE * f);
// static void latex_exp   (Node<Token> * node, FILE * f);
// static void latex_sin   (Node<Token> * node, FILE * f);
// static void latex_cos   (Node<Token> * node, FILE * f);
// static void latex_tg    (Node<Token> * node, FILE * f);
// static void latex_ctg   (Node<Token> * node, FILE * f);
// static void latex_arcsin (Node<Token> * node, FILE * f);
// static void latex_arccos (Node<Token> * node, FILE * f);
// static void latex_arctg  (Node<Token> * node, FILE * f);
// static void latex_arcctg (Node<Token> * node, FILE * f);
// static void latex_sh    (Node<Token> * node, FILE * f);
// static void latex_ch    (Node<Token> * node, FILE * f);
// static void latex_th    (Node<Token> * node, FILE * f);
// static void latex_cth   (Node<Token> * node, FILE * f);


#endif // !GET_LATEX_HPP_