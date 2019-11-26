#include "libraries.hpp"

#include "Parser.hpp"
#include "CreateNode.hpp"

enum
{
    OK,
    NUM_EXPECT,
    WRONG_FUNC_GRAMMAR
} errors;

BinaryTree<Token> * Parser::Parse (const char * str)
{
    str_begin = str;
    cur = str_begin;

    error = OK;

    BinaryTree<Token> * program = (BinaryTree<Token> *) calloc (1, sizeof (*program));

    program->init();
    program->root = ParseGrammar ();

    if (program->root = nullptr)
    {
        printf ("an error occured\n");
        return (nullptr);
    }

    return (program);
}

Node<Token> * Parser::ParseGrammar ()
{
    Node<Token> *  res = nullptr;

    while (*cur != '\0')
    {
        res = SetNode (FUNC_MARKER, "func", 4, ParseFunc (), res);
    }
    
    if (*cur != '\0')
    {
        return (nullptr);
    }

    return (res);
}

Node<Token> * Parser::ParseFunc ()
{
    Node<Token> * id   = nullptr;

    Move (0);

    if (!strcmp (cur, "def"))
    {
        Move (3);

        id = ParseId ();

        if (*cur == '(')
        {
            Move (1);

            id->left = ParseArgs ();

            if (*cur == ')')
            {
                Move (1);

                id->right = ParseBlock ();

                return (id);
            }
        }

        return (nullptr);
    }

    if (!strcmp (cur, "var"))
    {
        Move (3);

        return (ParseAssignment ());
    }

    return (nullptr);
}

Node<Token> * Parser::ParseBlock ()
{
    Node<Token> * res = nullptr;
    Node<Token> * op  = nullptr;

    Move (0);

    if (*cur == '{')
    {
        Move (1);

        while (*cur != '}')
        {
            op = ParseOp ();

            if (op == nullptr)
            {
                return (nullptr);
            }

            res = SetNode (OP_MARKER, "op", 2, op, res);
        }

        Move (1);

        return (res);
    }

    return (nullptr);
}

Node<Token> * Parser::ParseCond ()
{
    
}

Node<Token> * Parser::ParseIf ()
{
    
}

Node<Token> * Parser::ParseWhile ()
{
    
}

Node<Token> * Parser::ParseFor ()
{
    
}

Node<Token> * Parser::ParseAnd ()
{
    
}

Node<Token> * Parser::ParseOr ()
{
    
}

Node<Token> * Parser::ParseAddSub ()
{
    
}

Node<Token> * Parser::ParseMulDiv ()
{
    
}

Node<Token> * Parser::ParsePow ()
{
    
}

Node<Token> * Parser::ParsePrimary ()
{
    
}

Node<Token> * Parser::ParseId ()
{
    
}

Node<Token> * Parser::ParseNum ()
{
    
}

void Parser::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\t' || *cur == '\n')
    {
        cur++;
    }
}

void Parser::Move (size_t len)
{
    cur += len;
    SkipSpaces ();
}