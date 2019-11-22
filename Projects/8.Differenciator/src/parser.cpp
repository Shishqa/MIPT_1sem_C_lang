#include "headers.hpp"

#include "node_operations.hpp"

#include "parser.hpp"

void RecursiveDescentParser::parse (BinaryTree<Token> * expression, const char * path)
{
    FILE * f = fopen (path, "r");

    if (!f)
    {
        printf ("Can't open %s\n", path);
        return;
    }

    char * buff = nullptr;
    size_t size_of_file = Read (&buff, f);

    end_ptr = buff + size_of_file;

    fclose (f);

    expression->root = parseExpression (buff);
}

Node<Token> * RecursiveDescentParser::parseExpression (const char * str_begin)
{
    cur = str_begin;

    Node<Token> * res = parseAddSub ();

    return (res);
}

Node<Token> * RecursiveDescentParser::parseAddSub ()
{
    char op = 0;

    Node<Token> * left  = parseMulDiv ();
    Node<Token> * right = nullptr;

    while (*cur == '+' || *cur == '-')
    {
        op = *cur;
        cur++;

        right = parseMulDiv ();

        if (op == '+')
        {
            left = ADD (left, right);
        }
        else
        {
            left = SUB (left, right);
        }
    }

    return (left);
}

Node<Token> * RecursiveDescentParser::parseMulDiv ()
{
    char op = 0;

    Node<Token> * left  = parsePow ();
    Node<Token> * right = nullptr;

    while (*cur == '*' || *cur == '\\')
    {
        op = *cur;
        cur++;

        Node<Token> * right = parsePow ();

        if (op == '*')
        {
            left = MUL (left, right);
        }
        else
        {
            left = DIV (left, right);
        }
    } 

    return (left);
}

Node<Token> * RecursiveDescentParser::parsePow ()
{
    Node<Token> * left  = parsePrimary ();
    Node<Token> * right = nullptr;

    if (*cur == '^')
    {
        cur++;

        right = parseAddSub ();

        if (left->data.type == VAR_TYPE &&
            left->data.var  == 'e')
        {
            left = EXP (right);
        }
        else
        {
            left = POW (left, right);
        }
    }

    return (left);
}

Node<Token> * RecursiveDescentParser::parsePrimary ()
{
    Node<Token> * res = nullptr;

    if (*cur == '(')
    {
        cur++;

        res = parseAddSub ();

        if (*cur != ')')
        {
            printf ("syntax error!!! %s\n", *cur);
        }

        cur++;

        return (res);
    }

    if (*cur == '|')
    {
        cur++;

        res = parseAddSub ();

        if (*cur != '|')
        {
            printf ("syntax error!!! %s\n", *cur);
        }

        cur++;

        return (ABS (res));
    }

    if (isalpha (*cur))
    {
        return (parseFunc ());
    }

    if (*cur == '-' && isalpha (*(cur + 1)))
    {
        cur++;
        return (MUL(n(-1), parseFunc()));
    }

    return (parseNum ());
}

Node<Token> * RecursiveDescentParser::parseFunc ()
{
    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (cur + operations[i].name_len <= end_ptr &&
            !strncmp (cur, operations[i].name, operations[i].name_len))
        {
            cur += operations[i].name_len;
            return (CreateNode (-1, i, '~', OP_TYPE, parsePrimary()));
        }
    }

    return (parseVar ());
}

Node<Token> * RecursiveDescentParser::parseVar ()
{
    Node<Token> * res = v(*cur);

    cur++;

    return (res);
}

Node<Token> * RecursiveDescentParser::parseNum ()
{
    double val  = 0;
    size_t skip = 0;

    sscanf (cur, "%lf%n", &val, &skip);

    cur += skip;

    return (n(val));
}