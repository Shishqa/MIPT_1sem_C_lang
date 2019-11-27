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

    if (program->root == nullptr)
    {
        printf ("an error occured\n");
        return (nullptr);
    }

    printf ("PARSING COMPLETED\n");

    return (program);
}

Node<Token> * Parser::ParseGrammar ()
{
    printf ("grammar %s\n", cur);

    Node<Token> * res = nullptr;
    Node<Token> * tmp = nullptr;

    while (*cur != '\0')
    {
        printf ("new block\n");

        tmp = ParseGlobalBlock ();

        if (!tmp)
        {
            return (nullptr);
        }

        res = SetNode (FUNC_MARKER, "block", 4, tmp);

        printf ("block set\n");

        Move (0);
    }

    return (res);
}

Node<Token> * Parser::ParseGlobalBlock ()
{
    Move (0);

    printf ("block %s\n", cur);

    if (!strncmp (cur, "def", 3))
    {
        return (ParseFunc ());
    }

    if (!strncmp (cur, "var", 3))
    {
        return (ParseDefinition ());
    }

    return (nullptr);
}

Node<Token> * Parser::ParseFunc ()
{
    Move (0);

    printf ("func %s\n", cur);

    Node<Token> * id = nullptr;

    if (!strncmp (cur, "var", 3))
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

    return (nullptr);
}

Node<Token> * Parser::ParseDefinition ()
{
    printf ("def %c\n", *cur);

    Move (0);

    if (!strncmp (cur, "var", 3))
    {
        Move (3);

        return (SetNode (DEFINITION, "def", 3, ParseAssignment ()));
    }

    return (nullptr);
}

Node<Token> * Parser::ParseArgs ()
{
    printf ("args %s\n", cur);
}

Node<Token> * Parser::ParseReturn ()
{
    printf ("return %s\n", cur);
}

Node<Token> * Parser::ParseBlock ()
{
    printf ("block %s\n", cur);

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

Node<Token> * Parser::ParseAssignment ()
{
    printf ("assn %c\n", *cur);

    Move (0);

    Node<Token> * id = ParseId ();
    Node<Token> * expression = nullptr;

    Move (0);

    if (*cur == '=')
    {
        Move (1);

        expression = ParseExpression ();

        Move (0);

        if (*cur == ';')
        {
            printf ("assn <- ;\n");

            Move (1);

            return (SetNode (OP_TYPE, "=", 1, id, expression));
        }

        return (nullptr);
    }

    return (id);
}

Node<Token> * Parser::ParseOp ()
{
    printf ("op %c\n", *cur);

    Move (0);

    if (*cur == '{')
    {
        return (ParseBlock ());
    }

    if (!strncmp (cur, "if", 2))
    {
        return (ParseIf ());
    }

    if (!strncmp (cur, "while", 5))
    {
        return (ParseWhile ());
    }

    if (!strncmp (cur, "for", 3))
    {
        return (ParseFor ());
    }

    if (!strncmp (cur, "var", 3))
    {
        return (ParseDefinition ());
    }

    if (!strncmp (cur, "return", 6))
    {
        return (ParseReturn ());
    }

    return (ParseAssignment ());
}

Node<Token> * Parser::ParseCond ()
{
    printf ("cond %c\n", *cur);   
}

Node<Token> * Parser::ParseIf ()
{
    printf ("if %c\n", *cur);
}

Node<Token> * Parser::ParseWhile ()
{
    printf ("while %c\n", *cur);
}

Node<Token> * Parser::ParseFor ()
{
    printf ("for\n");
}

Node<Token> * Parser::ParseAnd ()
{
    printf ("and\n");
}

Node<Token> * Parser::ParseOr ()
{
    printf ("or\n");
}

Node<Token> * Parser::ParseExpression ()
{
    printf ("exp\n");

    Move (0);

    char op = 0;

    Node<Token> * left  = ParseMulDiv ();
    Node<Token> * right = nullptr;

    Move (0);

    while (*cur == '+' || *cur == '-')
    {
        op = *cur;
        Move (1);

        right = ParseMulDiv ();

        if (op == '+')
        {
            left = SetNode (OP_TYPE, "+", 1, left, right);
        }
        else
        {
            left = SetNode (OP_TYPE, "-", 1, left, right);
        }
    }

    return (left);
}

Node<Token> * Parser::ParseMulDiv ()
{
    printf ("muldiv\n");

    Move (0);

    char op = 0;

    Node<Token> * left  = ParsePrimary ();
    Node<Token> * right = nullptr;

    Move (0);

    if (!left)
    {
        return (nullptr);
    }

    while (*cur == '*' || *cur == '/')
    {
        op = *cur;
        Move (1);

        Node<Token> * right = ParsePrimary ();

        if (op == '*')
        {
            left = SetNode (OP_TYPE, "*", 1, left, right);
        }
        else
        {
            left = SetNode (OP_TYPE, "/", 1, left, right);
        }
    } 

    return (left);
}

Node<Token> * Parser::ParsePrimary ()
{
    printf ("primary ");

    Move (0);

    if (isalpha (*cur))
    {
        printf ("-> id\n");
        return (ParseId ());
    }

    if (*cur == '(')
    {
        printf ("-> (\n");
        Move (1);

        Node<Token> * res = ParseExpression ();

        if (*cur == ')')
        {
            printf ("primary <- )\n");
            Move (1);
            return (res);
        }

        return (nullptr);
    }

    printf ("-> num\n");
    return (ParseNum ());
}

Node<Token> * Parser::ParseId ()
{
    Move (0);

    size_t len = 0;

    sscanf (cur, "%*[^ =+-*/;&|(){}]%n", &len);

    char * id = (char *) calloc (len, sizeof (*id));

    strncpy (id, cur, len);

    cur += len;
    printf ("id len = %lu var = \"%s\"\n", len, id);

    return (SetNode (ID_TYPE, id, len));
}

Node<Token> * Parser::ParseNum ()
{
    Move (0);

    size_t len = 0;

    sscanf (cur, "%*lf%n", &len);

    char * num = (char *) calloc (len, sizeof (*num));

    strncpy (num, cur, len);

    cur += len;
    printf ("num len = %lu var = \"%s\"\n", len, num);

    return (SetNode (NUM_TYPE, num, len));
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

void PrintToken (FILE * out, const void * data)
{
    fprintf (out, "%s", ((Token *) data)->lexem);
}