#include "libraries.hpp"

#include "Parser.hpp"
#include "CreateNode.hpp"

#define STOP( err_code, node )                          \
        {                                               \
            if (node)                                   \
            {                                           \
                program_parsed->deleteSubtree (node);   \
            }                                           \
                                                        \
            error = (err_code);                         \
            return (nullptr);                           \
        }                                       

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

    line  = 1;
    newline = str_begin;
    error = OK;

    program_parsed = (BinaryTree<Token> *) calloc (1, sizeof (*program_parsed));

    program_parsed->init();
    program_parsed->root = ParseGrammar ();

    if (!program_parsed->root)
    {
        PrintError ();
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

        res = SetNode (BLOCK, "block", 5, tmp, res);

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

    return (ParseAssignment ());
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

        if (!id)
        {
            STOP (error, id);
        }

        Move (0);

        id->right = ParseBlock ();

        if (!id->right)
        {
            STOP (error, id);
        }

        id->right->parent = id;

        return (SetNode (DEF_FUNC, "def", 3, id));
    }

    STOP (error, id);
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

            res = SetNode (OPERATOR, "op", 2, op, res);
        }

        Move (1);

        printf ("block <- }\n");
        return (res);
    }

    STOP (NO_BLOCK, nullptr);
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

        res = SetNode (ASSIGN, "=", 1, id, expression);

        if (*cur == ';')
        {
            printf ("assn <- id = exp;\n");

            Move (1);

            return (res);
        }

        STOP (NO_END, res);
    }

    if (*cur == ';')
    {
        printf ("assn <- id;\n");

        Move (1);

        return (id);
    }

    STOP (NO_END, id);
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

    return (ParseAssignment ());
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

            if (!cond)
            {
                STOP (error, cond);
            }
            if (*cur != ')')
            {
                STOP (UNCLOSED_BRACES, cond);
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

Node<Token> * Parser::ParseCond ()
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

        left = SetNode (AND, "&", 1, left, right);

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

        left = SetNode (OR, "|", 1, left, right);

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

        res = ParseCond ();

        Move (0);

        if (!res)
        {
            STOP (error, res);
        }

        if (*cur != ']')
        {
            STOP (UNCLOSED_BRACES, res);
        }

        Move (1);

        return (res);
    }

    return (ParseBool ());
}

#define CHECK_BOOL( type, op, op_len )                                  \
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
            return (SetNode (type, op, op_len, left, right));           \
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

    CHECK_BOOL (G,   ">",  1);
    CHECK_BOOL (L,   "<",  1);
    CHECK_BOOL (GEQ, ">=", 2);
    CHECK_BOOL (LEQ, "<=", 2);
    CHECK_BOOL (EQ,  "==", 2);
    CHECK_BOOL (NEQ, "!=", 2);

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
            left = SetNode (ADD, "+", 1, left, right);
        }
        else
        {
            left = SetNode (SUB, "-", 1, left, right);
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
            left = SetNode (MUL, "*", 1, left, right);
        }
        else
        {
            left = SetNode (DIV, "/", 1, left, right);
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

        STOP (UNCLOSED_BRACES, res);
    }

    if (isdigit (*cur) || (*cur == '-' && isdigit(*(cur + 1))))
    {
        printf ("-> num\n");
        return (ParseNum ());
    }

    STOP (PRIMARY_EXPECT, nullptr);
}

Node<Token> * Parser::ParseSequence ()
{
    Move (0);

    Node<Token> * res = ParseExpression ();
    Node<Token> * tmp = nullptr;

    if (!res)
    {
        STOP (error, res);
    }

    res = SetNode (SEQUENCE, "seq", 3, res);

    Move (0);

    while (*cur == ',')
    {
        Move (1);

        tmp = ParseExpression ();

        if (!tmp)
        {
            STOP (error, res);
        }

        res = SetNode (SEQUENCE, "seq", 3, tmp, res);

        Move (0);
    }

    return (res);
}

Node<Token> * Parser::ParseArgs ()
{
    Node<Token> * args = nullptr;

    if (*cur == '(')
    {
        Move (1);

        if (*cur == ')')
        {
            Move (1);
            return (SetNode (NOARG, "void", 4));
        }

        args = ParseSequence ();

        Move (0);

        if (!args)
        {
            STOP (error, args);
        }

        if (*cur != ')')
        {
            STOP (UNCLOSED_BRACES, args);
        }

        Move (1);

        return (args);
    }

    STOP (error, args);
}

Node<Token> * Parser::ParseId ()
{
    Node<Token> * tmp = nullptr;

    bool create = false;

    Move (0);

    if (!strncmp (cur, "var", 3))
    {
        Move (3);
        create = true;
    }

    size_t len = 0;
    char * id  = GetId (&len);

    if (!id)
    {
        STOP (ID_EXPECT, nullptr);
    }

    Move (0);

    if (*cur == '(')
    {
        if (create)
        {
            STOP (VAR_FUNC, tmp);
        }

        tmp = ParseArgs ();

        if (!tmp)
        {
            STOP (error, tmp);
        }

        return (SetNode (FUNC, id, len, tmp));
    }

    Node<Token> * res = SetNode (ID, id, len);

    if (create)
    {
        return (SetNode (DEF_VAR, "var", 3, res));
    }

    return (res);
}

char * Parser::GetId (size_t * len)
{
    sscanf (cur, "%*[^ =+-*/,;&|(){}\n\t]%n", len);

    if (!(*len))
    {
        return (nullptr);
    }

    char * id = (char *) calloc (*len, sizeof (*id));

    strncpy (id, cur, *len);

    cur += *len;
    printf ("id len = %lu id = \"%s\"\n", *len, id);

    return (id);
}

Node<Token> * Parser::ParseNum ()
{
    Move (0);

    size_t len = 0;
    char * num = GetNum (&len);

    if (!num)
    {
        STOP (NUM_EXPECT, nullptr);
    }

    return (SetNode (NUM, num, len));
}

char * Parser::GetNum (size_t * len)
{
    sscanf (cur, "%*lf%n", len);

    if (!(*len))
    {
        return (nullptr);
    }

    char * num = (char *) calloc (*len, sizeof (*num));

    strncpy (num, cur, *len);

    cur += *len;
    printf ("num len = %lu var = \"%s\"\n", *len, num);

    return (num);
}

void Parser::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\t' || *cur == '\n' || *cur == '\r')
    {
        if (*cur == '\n')
        {
            line++;
            newline = cur;
        }

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
    Token * token = (Token *) data;

    fprintf (out, "%s", token->lexem);
}
