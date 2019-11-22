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

#endif // !GET_LATEX_HPP_