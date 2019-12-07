#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "head.hpp"
#include "OK_Tokenizer.hpp"

class SongParser
{
    char * buff;
    String * code;

    Tokenizer tokenizer;

    BinaryTree<Token> * result;

    Node<Token> ** token_arr;

    Node<Token> * ParseGrammar ();

    Node<Token> * ParseDefinition ();

    Node<Token> * ParseDef     ();
    Node<Token> * ParseVar     ();
    Node<Token> * ParseVarList ();

    Node<Token> * ParseBlock  ();
    Node<Token> * ParseOp     ();
    Node<Token> * ParseIf     ();
    Node<Token> * ParseWhile  ();

    Node<Token> * ParseCond     ();
    Node<Token> * ParseSequence ();
    Node<Token> * ParseArgs     ();

    Node<Token> * ParseExpression ();
    Node<Token> * ParseAssignment ();
    
    Node<Token> * ParseAnd         ();
    Node<Token> * ParseOr          ();
    Node<Token> * ParsePrimaryBool ();
    Node<Token> * ParseBool        ();

    Node<Token> * ParseAddSub      ();
    Node<Token> * ParseMulDiv      ();
    Node<Token> * ParsePrimary     ();

    Node<Token> * ParseCall   ();
    Node<Token> * ParseId     ();
    Node<Token> * ParseNum    ();

    public:

    BinaryTree<Token> * Parse (const char * path);
};

#endif // !PARSER_HPP_