#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "libraries.hpp"

//YACC Bizon

enum errors
{
    PARSER_OK,
    NUM_EXPECT,
    PRIMARY_EXPECT,
    ID_EXPECT,
    UNCLOSED_BRACES,
    VAR_FUNC,
    NO_END,
    NO_BLOCK,
    EMPTY_BLOCK
};

class Parser 
{
    BinaryTree<Token> * program_parsed;

    const char * str_begin;
    const char * cur;

    size_t error;
    size_t line;
    const char * newline;

    Node<Token> * ParseGrammar ();

    Node<Token> * ParseGlobalBlock ();

    Node<Token> * ParseFunc       ();
    Node<Token> * ParseAssignment ();

    Node<Token> * ParseBlock  ();
    Node<Token> * ParseOp     ();
    Node<Token> * ParseCondOp (const char * op, const int opcode);

    Node<Token> * ParseCond     ();
    Node<Token> * ParseSequence ();
    Node<Token> * ParseArgs     ();
    
    Node<Token> * ParseExpression  ();
    Node<Token> * ParseOr          ();
    Node<Token> * ParsePrimaryBool ();
    Node<Token> * ParseBool        ();

    Node<Token> * ParseAddSub      ();
    Node<Token> * ParseMulDiv      ();
    Node<Token> * ParsePrimary     ();

    Node<Token> * ParseVar ();
    Node<Token> * ParseId  ();
    Node<Token> * ParseNum ();

    char * GetId  (size_t * len);
    char * GetNum (size_t * len);

    void SkipSpaces ();
    void Move (size_t len);

    void SkipComment ();

    void PrintError ();

public:

    BinaryTree<Token> * Parse (const char * str);
    BinaryTree<Token> * ParseFile (const char * path);
};

void PrintToken (FILE * out, const void * data);

#endif // !PARSER_HPP_