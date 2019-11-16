#ifndef SIMPLIFY_HPP_
#define SIMPLIFY_HPP_

Node<Monomial> * Simplify (BinaryTree<Monomial> * exp, Node<Monomial> * node);
Node<Monomial> * SimplifyConst (BinaryTree<Monomial> * exp, Node<Monomial> * node);

int Count (const char op, int r);
int Count (const char op, int l, int r);
int BinaryPow (int x, int a);

#endif // !SIMPLIFY_HPP_