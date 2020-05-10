#ifndef DIFFERENCIATOR_HPP_
#define DIFFERENCIATOR_HPP_

class Differenciator
{
    const BinaryTree<Token> * expression;
    char var;
    FILE * latex;

    BinaryTree<Token> * diff_expression;

    BinaryTree<Token> * DiffExpression ();
    Node<Token> * Diff (Node<Token> * node);

public:
    BinaryTree<Token> * DiffWithLaTeX  (const BinaryTree<Token> * exp, const char dvar, const char * latex_path);
};

#endif // !DIFFERENCIATOR_HPP_