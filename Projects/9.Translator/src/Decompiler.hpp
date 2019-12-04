#ifndef DECOMPILER_HPP_
#define DECOMPILER_HPP_

#include "libraries.hpp"
#include "Parser.hpp"

// enum Errors
// {
//     TRANSLATOR_OK,
//     FUNC_REDEF,
//     VAR_REDEF,
//     FEW_ARGS,
//     MANY_ARGS,
//     NO_RETURN
// };

class Decompiler
{
    Parser parser;

    size_t error;

    BinaryTree<Token> * prog;

    FILE * out;

    void Proceed (FILE * f);

    void Calculate      (Node<Token> * node);
    void GetAdd         (Node<Token> * node);
    void GetSub         (Node<Token> * node);
    void GetMul         (Node<Token> * node);
    void GetDiv         (Node<Token> * node);
    void DefFunc        (Node<Token> * node);
    void GetCall        (Node<Token> * node);
    void GetAssign      (Node<Token> * node);
    void DefVar         (Node<Token> * node);
    void GetIf          (Node<Token> * node);
    void GetWhile       (Node<Token> * node);
    void GetBlocks      (Node<Token> * node);
    void GetOperators   (Node<Token> * node);
    void GetExpression  (Node<Token> * node);

public:
    bool GetCode (BinaryTree<Token> * code, const char * output_path);
};

#endif // !DECOMPILER_HPP_