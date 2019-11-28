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

/*
    Grammar :: GlobalBlock* '\0'
*/
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

/*
    GlobalBlock :: Func | Assignment
*/
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

/*
    Func :: "def" Id Args Block
*/
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

        id->left = ParseArgs ();

        if (!id->left)
        {
            STOP (error, id);
        }

        id->left->parent = id;

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

/*
    Block :: '{' Op* '}'
*/
Node<Token> * Parser::ParseBlock ()
{
    printf ("block %c\n", *cur);

    Node<Token> * res = nullptr;
    Node<Token> * op  = nullptr;

    Move (0);

    if (*cur == '{')
    {
        Move (1);

        if (*cur == '}')
        {
            Move (1);
            return (SetNode (VOID, "void", 4));
        }

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

/*
    Assignment :: Expression = Expression';' || Expression';'
*/
Node<Token> * Parser::ParseAssignment ()
{
    printf ("assn %c\n", *cur);

    Move (0);

    Node<Token> * id = ParseExpression ();
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

/*
    Op :: Block | CondOp | Assignment
*/
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

/*
    CondOp :: {"if","while"} '(' Cond ')' Block
*/
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

            cond = ParseExpression ();

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

/*
    Cond :: Or '&' Or+
*/
Node<Token> * Parser::ParseExpression ()
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

/*
    Or :: PrimaryBool '|' PrimaryBool+
*/
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

/*
    PrimaryBool :: '[' Cond ']' | Bool
*/
Node<Token> * Parser::ParsePrimaryBool ()
{
    printf ("primary bool\n");

    Node<Token> * res = nullptr;

    Move (0);

    if (*cur == '[')
    {
        Move (1);

        res = ParseExpression ();

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

/*
    Bool :: Expression {<,>,==,<=,>=,!=} Expression | Expression
*/
Node<Token> * Parser::ParseBool ()
{
    Move (0);

    Node<Token> * left  = ParseAddSub ();
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

/*
    Expression :: MulDiv {+,-} MulDiv
*/
Node<Token> * Parser::ParseAddSub ()
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

/*
    MulDiv :: Primary {*,/} Primary
*/
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


#define CHECK_POSTFIX( opcode, op_str, op_len )         \
    if (!strncmp (cur, op_str, op_len))                 \
    {                                                   \
        Move (op_len);                                  \
        return (SetNode (opcode, op_str, op_len, res)); \
    }                                         


/*
    Primary :: Id Args | Var | Id{++,--}
*/
Node<Token> * Parser::ParsePrimary ()
{
    printf ("primary ");

    Node<Token> * res  = nullptr;
    Node<Token> * args = nullptr; 

    Move (0);

    if (isalpha (*cur))
    {
        printf ("-> id\n");

        if (!strncmp (cur, "var", 3))
        {
            return (ParseVar ());
        }

        res = ParseId ();

        if (!res)
        {
            STOP (error, res);
        }

        Move (0);

        if (*cur == '(')
        {
            args = ParseArgs ();

            if (!args) 
            {
                STOP (error, res);
            }

            return (SetNode (FUNC, "call", 4, res, args));
        }

        CHECK_POSTFIX (INC, "++", 2);
        CHECK_POSTFIX (DEC, "--", 2);

        return (res);
    }

    if (*cur == '(')
    {
        printf ("-> (\n");
        Move (1);

        res = ParseExpression ();

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

/*
    Var :: "var" Id
*/
Node<Token> * Parser::ParseVar ()
{
    Node<Token> * res = nullptr;

    if (!strncmp (cur, "var", 3))
    {
        Move (3);

        res = ParseId ();

        if (!res)
        {
            STOP (error, res);
        }

        return (SetNode (DEF_VAR, "var", 3, res));
    }

    STOP (ID_EXPECT, res);
}

/*
    Sequence :: Expression ',' Expression+
*/
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

/*
    Args :: '(' Sequence ')' | '(' ')'
*/
Node<Token> * Parser::ParseArgs ()
{
    Node<Token> * args = nullptr;

    if (*cur == '(')
    {
        Move (1);

        if (*cur == ')')
        {
            Move (1);
            return (SetNode (VOID, "void", 4));
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

/*
    Id :: [a-zA-Z_]+
*/
Node<Token> * Parser::ParseId ()
{
    Node<Token> * tmp = nullptr;

    Move (0);

    size_t len = 0;
    char * id  = GetId (&len);

    if (!id)
    {
        STOP (ID_EXPECT, nullptr);
    }

    return (SetNode (ID, id, len));
}

/*
    Парсит имя вида Typical_Name и возвращает указатель на строку
*/
char * Parser::GetId (size_t * len)
{
    if (isdigit (*cur))
    {
        return (nullptr);
    }

    sscanf (cur, "%*[a-zA-Z0-9_]%n", len);

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

/*
    Num :: {-}[0-9]+
*/
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

/*
    Парсит число с плавающей точкой и возвращает его в виде строки
*/
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

/*
    Пропускает пробельные символы
*/
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

/*
    Сдвигает курсор на len, затем пропускает пробелы и комментарии
*/
void Parser::Move (size_t len)
{
    cur += len;

    SkipSpaces  ();
    SkipComment ();
    SkipSpaces  ();
}

/*
    Комментарии:
        $$ my_comment

        или

        $ my
          comment $
*/

void Parser::SkipComment ()
{
    if (!strncmp (cur, "$$", 2))
    {
        cur += 2;

        while (*cur != '\n')
        {
            cur++;
        }

        cur++;
    }
    else if (*cur == '$')
    {
        cur += 1;

        while (*cur != '$')
        {
            cur++;
        }

        cur += 1;
    }
}

void PrintToken (FILE * out, const void * data)
{
    Token * token = (Token *) data;

    if (*(token->lexem) == '|' ||       // Экранирует спец.символы Dot 
        *(token->lexem) == '<' ||
        *(token->lexem) == '>' )
    {
        fprintf (out, "\\");
    }

    fprintf (out, "%s", token->lexem);
}

