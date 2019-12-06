#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "head.hpp"

enum Parser_Errors
{
    PARSE_OK,
    SYNTAX_ERROR,
    EXPECT_STAFF
};

struct Chord
{
    int max_d;
    int min_d;
};

class SongParser
{
    BinaryTree<Token> * result;

    char * buff;
    const char * cur;
    size_t num_of_lines;
    size_t curr_line;

    size_t error;

    String * code;

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
    Node<Token> * ParseChar   ();
    Node<Token> * ParseString ();
    Node<Token> * ParseId     ();
    Node<Token> * ParseNum    ();

    char * GetId  (size_t * len);
    char * GetNum (size_t * len);

    bool InitStaff ();
    int GetOp ();
    Chord GetChord ();
    void NextLine ();
    void SkipSpaces (size_t pre_step = 0);

    public:

    BinaryTree<Token> * Parse (const char * path);
};

#endif // !PARSER_HPP_