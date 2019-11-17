#ifndef GET_LATEX_HPP_
#define GET_LATEX_HPP_

#include "headers.hpp"

void getLaTeX (BinaryTree<Token> * expression, BinaryTree<Token> * diff, const char * name, bool open = false);
void getNodeLaTeX (Node<Token> * node, FILE * f);

#endif // !GET_LATEX_HPP_