#ifndef GET_LATEX_HPP_
#define GET_LATEX_HPP_

#include "headers.hpp"

void getLaTeX (BinaryTree<Monomial> * expression, BinaryTree<Monomial> * diff, const char * name, bool open = false);
void getNodeLaTeX (Node<Monomial> * node, FILE * f);

#endif // !GET_LATEX_HPP_