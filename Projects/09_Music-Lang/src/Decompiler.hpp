#ifndef DECOMPILER_HPP_
#define DECOMPILER_HPP_

#include "libraries.hpp"

class Decompiler
{
    size_t error;

    BinaryTree<Token> * prog;

    FILE * out;
    size_t deep;

    void Proceed (FILE * f);

    void Calculate     (Node<Token> * node);
    void GetOperator   (Node<Token> * node);

    void SetTabs ();

public:
    bool GetCode (BinaryTree<Token> * code, const char * output_path);
};

#endif // !DECOMPILER_HPP_