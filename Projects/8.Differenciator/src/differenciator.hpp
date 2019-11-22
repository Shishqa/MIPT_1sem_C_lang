#ifndef DIFFERENCIATOR_HPP_
#define DIFFERENCIATOR_HPP_

BinaryTree<Token> * DiffWithLaTeX  (const BinaryTree<Token> * expression, const char var, const char * latex);
BinaryTree<Token> * DiffExpression (const BinaryTree<Token> * expression, const char var, FILE * latex = nullptr);
Node<Token> * Diff (Node<Token> * node, const char var, FILE * latex);
Node<Token> * getRoot (Node<Token> * node);

#endif // !DIFFERENCIATOR_HPP_