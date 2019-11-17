#ifndef SIMPLIFY_HPP_
#define SIMPLIFY_HPP_

Node<Token> * Simplify (BinaryTree<Token> * exp, Node<Token> * node);
Node<Token> * SimplifyConst (BinaryTree<Token> * exp, Node<Token> * node);

int Count (const char op, int r);
int Count (const char op, int l, int r);
int BinaryPow (int x, int a);

#endif // !SIMPLIFY_HPP_