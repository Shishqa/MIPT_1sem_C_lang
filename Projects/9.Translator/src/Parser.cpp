#include "libraries.hpp"

#include "Parser.hpp"
#include "CreateNode.hpp"

#include <unistd.h>

#define STOP( err_code, node )                          \
        {                                               \
            printf ("STOPSTOPSTOP\n");                  \
                                                        \
            if (node)                                   \
            {                                           \
                program_parsed->deleteSubtree (node);   \
            }                                           \
                                                        \
            error = (err_code);                         \
            return (nullptr);                           \
        }                                       

enum
{
    OK,
    NUM_EXPECT,
    WRONG_FUNC_GRAMMAR
} errors;

BinaryTree<Token> * Parser::ParseFile (const char * path)
{
    FILE * f = fopen (path, "r");

    if (!f)
    {
        printf ("can't open file\n");
        return (nullptr);
    }

    char * str = nullptr;

    Read (&str, f);

    fclose (f);

    BinaryTree<Token> * res = Parse (str);

    free (str);

    return (res);
}

BinaryTree<Token> * Parser::Parse (const char * str)
{
    str_begin = str;
    cur = str_begin;

    error = OK;

    program_parsed = (BinaryTree<Token> *) calloc (1, sizeof (*program_parsed));

    program_parsed->init();
    program_parsed->root = ParseGrammar ();

    if (!program_parsed->root)
    {
        printf ("an error occured\n");
        return (nullptr);
    }

    printf ("PARSING COMPLETED\n");

    return (program_parsed);
}

Node<Token> * Parser::ParseGrammar ()
{
    printf ("grammar %c\n", *cur);

    Node<Token> * res = nullptr;
    Node<Token> * tmp = nullptr;

    while (*cur != '\0')
    {
        printf ("new block\n");

        tmp = ParseGlobalBlock ();

        if (!tmp)
        {
            STOP (error, nullptr);
        }

        res = SetNode (FUNC_MARKER, "block", 5, tmp, res);

        printf ("block set\n");

        Move (0);
    }

    return (res);
}

Node<Token> * Parser::ParseGlobalBlock ()
{
    Move (0);

    printf ("block %c\n", *cur);

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

    printf ("func %c\n", *cur);

    Node<Token> * id = nullptr;

    if (!strncmp (cur, "def", 3))
    {
        Move (3);

        id = ParseId ();

        Move (0);

        if (*cur == '(')
        {
            Move (1);

            id->left = ParseArgs ();

            if (id->left)
            {
                id->left->parent = id;
            }

            if (*cur == ')')
            {
                Move (1);

                id->right = ParseBlock ();

                if (!id->right)
                {
                    STOP (error, id);
                }

                id->right->parent = id;

                return (SetNode (FUNC_MARKER, "func", 4, id));
            }
        }
    }

    STOP (error, id);
}

Node<Token> * Parser::ParseDefinition ()
{
    printf ("def %c\n", *cur);

    Move (0);

    if (!strncmp (cur, "var", 3))
    {
        Move (3);

        return (SetNode (DEFINITION, "def", 3, nullptr, ParseAssignment ()));
    }

    STOP (error, nullptr);
}

Node<Token> * Parser::ParseArgs ()
{
    printf ("args %c\n", *cur);

    Node<Token> * res = nullptr;

    Move (0);

    while (!strncmp (cur, "var", 3))
    {
        Move (3);

        res = SetNode (VARIABLE, "var", 3, ParseId (), res);

        Move (0);

        if (*cur != ',')
        {
            break;
        }

        Move (1);
    }

    return (res);
}

Node<Token> * Parser::ParseReturn ()
{
    printf ("return %c\n", *cur);
}

Node<Token> * Parser::ParseBlock ()
{
    printf ("block %c\n", *cur);

    Node<Token> * res = nullptr;
    Node<Token> * op  = nullptr;

    Move (0);

    if (*cur == '{')
    {
        Move (1);

        while (*cur != '}')
        {
            op = ParseOp ();

            if (!op)
            {
                STOP (error, res);
            }

            res = SetNode (OP_MARKER, "op", 2, op, res);
        }

        Move (1);

        printf ("block <- }\n");
        return (res);
    }

    STOP (error, nullptr);
}

Node<Token> * Parser::ParseAssignment ()
{
    printf ("assn %c\n", *cur);

    Move (0);

    Node<Token> * id = ParseId ();
    Node<Token> * expression = nullptr;
    Node<Token> * res = nullptr;

    if (!id)
    {
        STOP (error, id);
    }

    Move (0);

    if (*cur == '=')
    {
        Move (1);

        expression = ParseExpression ();

        if (!expression)
        {
            STOP (error, id);
        }

        Move (0);

        res = SetNode (OP_TYPE, "=", 1, id, expression);

        if (*cur == ';')
        {
            printf ("assn <- id = exp;\n");

            Move (1);

            return (res);
        }

        STOP (error, res);
    }

    if (*cur == ';')
    {
        printf ("assn <- id;\n");

        Move (1);

        return (id);
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
        return (ParseCondOp ("if", IF));
    }

    if (!strncmp (cur, "while", 5))
    {
        return (ParseCondOp ("while", WHILE));
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

    return (ParseAnd ());
}

Node<Token> * Parser::ParseCondOp (const char * op, const int opcode)
{
    printf ("%s %c\n", op, *cur);

    size_t op_len = strlen (op);

    Node<Token> * cond  = nullptr;
    Node<Token> * block = nullptr;

    Move (0);

    if (!strncmp (cur, op, op_len))
    {
        Move (op_len);

        if (*cur == '(')
        {
            Move (1);

            cond = ParseCond ();

            if (!cond || *cur != ')')
            {
                STOP (error, cond);
            }

            Move (1);

            block = ParseBlock ();

            if (!block)
            {
                STOP (error, cond);
            }

            return (SetNode (opcode, op, op_len, cond, block));
        }
    }

    STOP (error, cond);
}

Node<Token> * Parser::ParseAnd ()
{
    printf ("and\n");

    Node<Token> * left = ParseOr ();
    Node<Token> * right = nullptr;

    if (!left)
    {
        STOP (error, left);
    }

    Move (0);

    while (*cur == '&')
    {
        Move (1);

        right = ParseOr ();

        if (!right)
        {
            STOP (error, left);
        }

        left = SetNode (OP_TYPE, "&", 1, left, right);

        Move (0);
    }

    return (left);
}

Node<Token> * Parser::ParseOr ()
{
    printf ("or\n");

    Node<Token> * left  = ParsePrimaryBool ();
    Node<Token> * right = nullptr;

    if (!left)
    {
        STOP (error, left);
    }

    Move (0);

    while (*cur == '|')
    {
        Move (1);

        right = ParsePrimaryBool ();

        if (!right)
        {
            STOP (error, left);
        }

        left = SetNode (OP_TYPE, "\\|", 1, left, right);

        Move (0);
    }

    return (left);
}

Node<Token> * Parser::ParsePrimaryBool ()
{
    printf ("primary bool\n");

    Node<Token> * res = nullptr;

    Move (0);

    if (*cur == '[')
    {
        Move (1);

        res = ParseAnd ();

        Move (0);

        if (!res || *cur != ']')
        {
            STOP (error, res);
        }

        Move (1);

        return (res);
    }

    return (ParseBool ());
}

#define CHECK_BOOL( op, op_len )                                        \
        if (!strncmp (cur, op, op_len))                                 \
        {                                                               \
            Move (op_len);                                              \
            right = ParseExpression ();                                 \
                                                                        \
            if (!right)                                                 \
            {                                                           \
                STOP (error, left);                                     \
            }                                                           \
                                                                        \
            return (SetNode (BOOL_TYPE, op, op_len, left, right));      \
        }

Node<Token> * Parser::ParseBool ()
{
    Move (0);

    Node<Token> * left  = ParseExpression ();
    Node<Token> * right = nullptr;

    if (!left)
    {
        STOP (error, left);
    }

    Move (0);

    CHECK_BOOL (">",  1);
    CHECK_BOOL ("<",  1);
    CHECK_BOOL (">=", 2);
    CHECK_BOOL ("<=", 2);
    CHECK_BOOL ("==", 2);
    CHECK_BOOL ("!=", 2);

    return (left);
}

Node<Token> * Parser::ParseExpression ()
{
    printf ("exp\n");

    Move (0);

    char op = 0;

    Node<Token> * left  = ParseMulDiv ();
    Node<Token> * right = nullptr;

    if (!left)
    {
        STOP (error, left);
    }

    Move (0);

    while (*cur == '+' || *cur == '-')
    {
        op = *cur;
        Move (1);

        right = ParseMulDiv ();

        if (!right)
        {
            STOP (error, left);
        }

        if (op == '+')
        {
            left = SetNode (OP_TYPE, "+", 1, left, right);
        }
        else
        {
            left = SetNode (OP_TYPE, "-", 1, left, right);
        }

        Move (0);
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
        STOP (error, left);
    }

    while (*cur == '*' || *cur == '/')
    {
        op = *cur;
        Move (1);

        right = ParsePrimary ();

        if (!right)
        {
            STOP (error, left);
        }

        if (op == '*')
        {
            left = SetNode (OP_TYPE, "*", 1, left, right);
        }
        else
        {
            left = SetNode (OP_TYPE, "/", 1, left, right);
        }

        Move (0);
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

        if (!res)
        {
            STOP (error, res);
        }

        if (*cur == ')')
        {
            printf ("primary <- )\n");
            Move (1);
            return (res);
        }

        STOP (error, res);
    }

    printf ("-> num\n");
    return (ParseNum ());
}

Node<Token> * Parser::ParseId ()
{
    Move (0);

    size_t len = 0;

    sscanf (cur, "%*[^ =+-*/,;&|(){}\n\t]%n", &len);

    if (!len)
    {
        return (nullptr);
    }

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

    if (!len)
    {
        return (nullptr);
    }

    char * num = (char *) calloc (len, sizeof (*num));

    strncpy (num, cur, len);

    cur += len;
    printf ("num len = %lu var = \"%s\"\n", len, num);

    return (SetNode (NUM_TYPE, num, len));
}

void Parser::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\t' || *cur == '\n' || *cur == '\r')
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
