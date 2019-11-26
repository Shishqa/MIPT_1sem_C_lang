#ifndef FLEXER_HPP_
#define FLEXER_HPP_

#include "libraries.hpp"

class Parser 
{
    // const char FUNC_MARKER[5]   = "func";
    // const char MAIN_MARKER[5]   = "main";
    // const char IF_MARKER[3]     = "if";
    // const char WHILE_MARKER[6]  = "while";
    // const char FOR_MARKER[4]    = "for";
    // const char VAR_MARKER[4]    = "var";
    // const char RETURN_MARKER[7] = "return";

    const char * str_begin;
    const char * cur;

    size_t error;

    Node<Token> * ParseGrammar ();

    Node<Token> * ParseFunc  ();
    Node<Token> * ParseBlock ();
    Node<Token> * ParseArgs  ();
    Node<Token> * ParseOp    ();
    Node<Token> * ParseAssignment ();

    Node<Token> * ParseCond  ();
    Node<Token> * ParseIf    ();
    Node<Token> * ParseWhile ();
    Node<Token> * ParseFor   ();
    
    Node<Token> * ParseAnd ();
    Node<Token> * ParseOr  ();

    Node<Token> * ParseExpression ();
    Node<Token> * ParseMulDiv     ();
    Node<Token> * ParsePrimary    ();

    Node<Token> * ParseId  ();
    Node<Token> * ParseNum ();

    void SkipSpaces ();
    void Move (size_t len);

public:

    BinaryTree<Token> * Parse (const char * path);
};

#endif // !FLEXER_HPP_