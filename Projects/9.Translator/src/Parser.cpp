#include "libraries.hpp"

#include "Parser.hpp"
#include "CreateNode.hpp"

#define STOP( err_code, node )                          \
        {                                               \
            PRINT("ERROR\n");                           \
                                                        \
            if (node)                                   \
            {                                           \
                program_parsed->deleteSubtree (node);   \
            }                                           \
                                                        \
            error = (err_code);                         \
            return (nullptr);                           \
        }                                       

#define SET_VOID()                          \
        SetNode(VOID, "void", 4)            \


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
    error = PARSER_OK;

    program_parsed = (BinaryTree<Token> *) calloc (1, sizeof (*program_parsed));

    program_parsed->init();
    program_parsed->root = ParseGrammar ();

    if (!program_parsed->root || error != PARSER_OK)
    {
        PrintError ();
        SETCOLOR (RED);
        printf ("PARSING FAILED\n");
        SETCOLOR (RESET);
        return (nullptr);
    }

    printf ("PARSING COMPLETED\n");

    program_parsed->dotDump (PrintToken, 0);

    return (program_parsed);
}

/*
    Grammar :: GlobalBlock* '\0'
*/
Node<Token> * Parser::ParseGrammar ()
{
    PRINT ("grammar\n");

    Node<Token> * res = nullptr;
    Node<Token> * tmp = nullptr;

    while (*cur != '\0')
    {
        PRINT ("new definition\n");

        tmp = ParseDefinition ();

        if (!tmp)
        {
            STOP (error, nullptr);
        }

        res = SetNode (DEFINITION, "D", 1, res, tmp);

        PRINT ("definition set\n");

        Move (0);
    }

    return (res);
}

/*
    GlobalBlock :: Func | Assignment
*/
Node<Token> * Parser::ParseDefinition ()
{
    Move (0);

    PRINT ("definition\n");

    if (!strncmp (cur, "def", 3))
    {
        return (ParseDef ());
    }

    return (ParseVar ());
}

/*
    Func :: "def" Id Args Block
*/
Node<Token> * Parser::ParseDef ()
{
    Move (0);

    PRINT ("def_func\n");

    Node<Token> * res = SetNode (DEF_FUNC, "def", 3);

    if (!strncmp (cur, "def", 3))
    {
        Move (3);

        res->right = ParseId ();

        if (!res->right)
        {
            STOP (error, res);
        }

        res->right->parent = res;

        Move (0);

        if (*cur != '(')
        {
            STOP (ID_EXPECT, res);
        }

        Move (1);

        if (*cur != ')')
        {
            res->left = ParseVarList ();

            if (!res->left)
            {
                STOP (error, res);
            }

            res->left->parent = res;
        }

        res->right->right = ParseBlock ();

        if (!res->right->right)
        {
            STOP (error, res);
        }

        res->right->right->parent = res;

        return (res);
    }

    STOP (error, res);
}

/*
    Block :: '{' Op* '}'
*/
Node<Token> * Parser::ParseBlock ()
{
    PRINT ("block\n");

    Node<Token> * res = nullptr;
    Node<Token> * op  = nullptr;

    Move (0);

    if (*cur == '{')
    {
        Move (1);

        if (*cur == '}')
        {
            Move (1);
            return (SetNode (BLOCK, "B", 1));
        }

        while (*cur != '}')
        {
            op = ParseOp ();

            if (!op)
            {                
                STOP (error, res);
            }

            op->left    = res;
            res->parent = op;
            res = op;

            Move (0);
        }

        Move (1);

        PRINT ("block <- }\n");

        return (SetNode (BLOCK, "B", 1, nullptr, res));
    }

    STOP (NO_BLOCK, nullptr);
}

/*
    Assignment :: Expression = Expression';' || Expression';'
*/
Node<Token> * Parser::ParseAssignment ()
{
    PRINT ("assn\n");

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
            PRINT ("assn <- id = exp;\n");

            Move (1);

            return (res);
        }

        STOP (NO_END, res);
    }

    if (*cur == ';')
    {
        PRINT ("assn <- id;\n");

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
    PRINT ("op\n");

    Move (0);

    if (!strncmp (cur, "if", 2))
    {
        return (SetNode (OPERATOR, "op", 2, nullptr, ParseIf ()));
    }

    if (!strncmp (cur, "while", 5))
    {
        return (SetNode (OPERATOR, "op", 2, nullptr, ParseWhile ()));
    }

    if (!strncmp (cur, "var", 3))
    {
        return (SetNode (OPERATOR, "op", 2, nullptr, ParseVar ()));
    }

    return (ParseExpression ());
}

/*
    CondOp :: "while" '(' Expression ')' Block
*/
Node<Token> * Parser::ParseWhile ()
{
    Node<Token> * cond  = nullptr;
    Node<Token> * block = nullptr;

    Move (0);

    if (!strncmp (cur, "while", 5))
    {
        Move (5);

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

            return (SetNode (WHILE, "while", 5, cond, block));
        }
    }

    STOP (error, cond);
}

/*
    Expression :: Or ('&' Or)*
*/
Node<Token> * Parser::ParseAnd ()
{
    PRINT ("and\n");

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
    PRINT ("or\n");

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
    PRINT ("primary bool\n");

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
    Bool :: AddSub {<,>,==,<=,>=,!=} AddSub | AddSub
*/
Node<Token> * Parser::ParseBool ()
{
    PRINT ("bool\n");

    Move (0);

    Node<Token> * left  = ParseAddSub ();
    Node<Token> * right = nullptr;

    if (!left)
    {
        STOP (error, left);
    }

    Move (0);

    CHECK_BOOL (GEQ,     ">=", 2);
    CHECK_BOOL (LEQ,     "<=", 2);
    CHECK_BOOL (EQ,      "==", 2);
    CHECK_BOOL (NEQ,     "!=", 2);
    CHECK_BOOL (GREATER, ">",  1);
    CHECK_BOOL (LESS,    "<",  1);

    return (left);
}

/*
    AddSub :: MulDiv {+,-} MulDiv
*/
Node<Token> * Parser::ParseAddSub ()
{
    PRINT ("exp\n");

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
    PRINT ("muldiv\n");

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
    PRINT ("primary ");

    Node<Token> * res  = nullptr;
    Node<Token> * args = nullptr; 

    Move (0);

    if (isalpha (*cur))
    {
        PRINT ("-> id\n");

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
        PRINT ("-> (\n");
        Move (1);

        res = ParseExpression ();

        if (!res)
        {
            STOP (error, res);
        }

        if (*cur == ')')
        {
            PRINT ("primary <- )\n");
            Move (1);
            return (res);
        }

        STOP (UNCLOSED_BRACES, res);
    }

    if (isdigit (*cur) || (*cur == '-' && isdigit(*(cur + 1))))
    {
        PRINT ("-> num\n");
        return (ParseNum ());
    }

    if (*cur == '\'')
    {
        return (ParseId ());
    }

    STOP (PRIMARY_EXPECT, nullptr);
}

/*
    Var :: "var" Id
*/
Node<Token> * Parser::ParseVar ()
{
    PRINT ("var\n");

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
    PRINT ("seq\n");

    Move (0);

    Node<Token> * res = ParseExpression ();
    Node<Token> * tmp = nullptr;

    if (!res)
    {
        STOP (error, res);
    }

    res = SetNode (SEQUENCE, "seq", 3, res, SET_VOID ());

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
    PRINT ("args\n");

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
    PRINT ("id\n");

    Node<Token> * tmp = nullptr;

    Move (0);

    size_t len = 0;
    char * id  = GetId (&len);

    if (!id)
    {
        STOP (ID_EXPECT, nullptr);
    }

    if (*id == '\'')
    {
        return (SetNode (CHAR, id, len));
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

    if (*cur == '\'')
    {
        sscanf (cur, "'%*c'%n", len);

        if (*len != 3)
        {
            return (nullptr);
        }
    }
    else
    {
        sscanf (cur, "%*[a-zA-Z0-9_]%n", len);
    }

    if (!(*len))
    {
        return (nullptr);
    }

    char * id = (char *) calloc (*len, sizeof (*id));

    strncpy (id, cur, *len);

    Move (*len);

    PRINT (id); PRINT ("\n");

    return (id);
}

/*
    Num :: {-}[0-9]+
*/
Node<Token> * Parser::ParseNum ()
{
    PRINT ("num\n");

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

    PRINT (num); PRINT ("\n");

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

