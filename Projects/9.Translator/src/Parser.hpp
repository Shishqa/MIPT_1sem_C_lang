#ifndef FLEXER_HPP_
#define FLEXER_HPP_

#include "libraries.hpp"

class Parser 
{
    BinaryTree<Token> * program_parsed;

    const char * str_begin;
    const char * cur;

    size_t error;

    Node<Token> * ParseGrammar ();

    Node<Token> * ParseGlobalBlock ();
    Node<Token> * ParseFunc  ();
    Node<Token> * ParseBlock ();
    Node<Token> * ParseArgs  ();
    Node<Token> * ParseOp    ();
    Node<Token> * ParseAssignment ();
    Node<Token> * ParseDefinition ();

    Node<Token> * ParseCond   ();
    Node<Token> * ParseCondOp (const char * op, const int opcode);
    Node<Token> * ParseReturn ();
    
    Node<Token> * ParseAnd ();
    Node<Token> * ParseOr  ();
    Node<Token> * ParsePrimaryBool ();
    Node<Token> * ParseBool ();

    Node<Token> * ParseExpression ();
    Node<Token> * ParseMulDiv     ();
    Node<Token> * ParsePrimary    ();

    Node<Token> * ParseId  ();
    Node<Token> * ParseNum ();

    void SkipSpaces ();
    void Move (size_t len);

public:

    BinaryTree<Token> * Parse (const char * str);
    BinaryTree<Token> * ParseFile (const char * path);
};

void PrintToken (FILE * out, const void * data);

#endif // !FLEXER_HPP_