#ifndef SIMPLIFY_HPP_
#define SIMPLIFY_HPP_

Node<Token> * Simplify (BinaryTree<Token> * exp, Node<Token> * node);
Node<Token> * SimplifyConst (BinaryTree<Token> * exp, Node<Token> * node);

double Count (const int op_id, const double l, const double r);
int BinaryPow (int x, int a);

#endif // !SIMPLIFY_HPP_