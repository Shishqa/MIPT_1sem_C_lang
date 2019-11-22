#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "headers.hpp"

class RecursiveDescentParser
{
    const char * cur;

    const char * error_marker;
    const char * end_ptr;

    Node<Token> * parseExpression  (const char * str_begin);

    Node<Token> * parseAddSub   ();
    Node<Token> * parseMulDiv   ();
    Node<Token> * parsePow      ();
    Node<Token> * parsePrimary  ();
    Node<Token> * parseFunc     ();
    Node<Token> * parseVar      ();
    Node<Token> * parseNum      ();

public:
    void parse (BinaryTree<Token> * expression, const char * path);
};

#endif //!PARSER_HPP_