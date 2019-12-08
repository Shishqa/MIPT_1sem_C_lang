// #include "libraries.hpp"

// #include "Parser.hpp"
// #include "CreateNode.hpp"

// #define STOP( err_code, node )                          \
//         {                                               \
//             PRINT("ERROR\n");                           \
//                                                         \
//             if (node)                                   \
//             {                                           \
//                 program_parsed->deleteSubtree (node);   \
//             }                                           \
//                                                         \
//             error = (err_code);                         \
//             return (nullptr);                           \
//         }                                       

// #define CHECK( node, to_del )             \
//         if (!node)                        \
//         {                                 \
//             STOP( error, to_del );        \
//         }                       

// BinaryTree<Token> * Parser::ParseFile (const char * path)
// {
//     FILE * f = fopen (path, "r");

//     if (!f)
//     {
//         printf ("can't open file\n");
//         return (nullptr);
//     }

//     char * str = nullptr;

//     Read (&str, f);

//     fclose (f);

//     BinaryTree<Token> * res = Parse (str);

//     free (str);

//     return (res);
// }

// BinaryTree<Token> * Parser::Parse (const char * str)
// {
//     str_begin = str;
//     cur = str_begin;

//     line  = 1;
//     newline = str_begin;
//     error = PARSER_OK;

//     program_parsed = (BinaryTree<Token> *) calloc (1, sizeof (*program_parsed));

//     program_parsed->init();

//     printf ("STARTED\n\n");

//     program_parsed->root = ParseGrammar ();

//     if (!program_parsed->root || error != PARSER_OK)
//     {
//         PrintError ();
//         SETCOLOR (RED);
//         printf ("PARSING FAILED\n");
//         SETCOLOR (RESET);
//         return (nullptr);
//     }

//     program_parsed->dotDump (PrintToken, 0);
//     printf ("PARSING COMPLETED\n");

//     return (program_parsed);
// }

// /*
//     Grammar :: GlobalBlock* '\0'
// */
// Node<Token> * Parser::ParseGrammar ()
// {
//     PRINT ("grammar\n");

//     Node<Token> * res = nullptr;
//     Node<Token> * tmp = nullptr;

//     while (*cur != '\0')
//     {
//         PRINT ("new definition\n");

//         tmp = ParseDefinition ();

//         CHECK (tmp, tmp);

//         res = SetNode (DEFINITION, "D", 1, res, tmp);

//         PRINT ("definition set\n");

//         Move (0);
//     }

//     return (res);
// }

// /*
//     GlobalBlock :: Func | Assignment
// */
// Node<Token> * Parser::ParseDefinition ()
// {
//     Move (0);

//     PRINT ("definition\n");

//     if (!strncmp (cur, "def", 3))
//     {
//         return (ParseDef ());
//     }

//     return (ParseVar ());
// }

// /*
//     Func :: "def" Id Args Block
// */
// Node<Token> * Parser::ParseDef ()
// {
//     Move (0);

//     PRINT ("def_func\n");

//     Node<Token> * res = SetNode (DEF_FUNC, "def", 3);

//     if (!strncmp (cur, "def", 3))
//     {
//         Move (3);

//         res->right = ParseId ();

//         CHECK (res->right, res);

//         LINK (res->right, res);

//         Move (0);

//         if (*cur != '(')
//         {
//             STOP (ID_EXPECT, res);
//         }

//         Move (1);

//         if (*cur != ')')
//         {
//             res->left = ParseVarList ();

//             CHECK (res->left, res);

//             LINK (res->left, res);
//         }

//         Move (1);

//         res->right->right = ParseBlock ();

//         CHECK (res->right->right, res);

//         LINK (res->right->right, res);

//         return (res);
//     }

//     STOP (error, res);
// }

// Node<Token> * Parser::ParseVarList ()
// {
//     PRINT ("varlist\n");

//     Move (0);

//     Node<Token> * res = ParseVar ();
//     Node<Token> * tmp = nullptr;

//     CHECK (res, res);

//     res = SetNode (COMMA, ",", 1, nullptr, res);

//     Move (0);

//     while (*cur == ',')
//     {
//         Move (1);

//         tmp = ParseVar ();

//         CHECK (tmp, res);

//         res = SetNode (COMMA, ",", 1, res, tmp);

//         Move (0);
//     }

//     return (res);
// }

// /*
//     Var :: "var" Id
// */
// Node<Token> * Parser::ParseVar ()
// {
//     PRINT ("var\n");

//     Move (0);

//     Node<Token> * res = nullptr;

//     if (!strncmp (cur, "var", 3))
//     {
//         Move (3);

//         res = ParseAssignment ();

//         CHECK (res, res);

//         if (res->data.type == ASSIGN)
//         {
//             res->data.type = DEF_VAR;
//             res->data.lexem = "var";
//             res->data.lexem_len = 3;
//         }
//         else
//         {
//             res = SetNode (DEF_VAR, "var", 3, res);
//         }

//         return (res);
//     }

//     STOP (ID_EXPECT, res);
// }

// /*
//     Block :: '{' Op* '}'
// */
// Node<Token> * Parser::ParseBlock ()
// {
//     PRINT ("block\n");

//     Node<Token> * res = nullptr;
//     Node<Token> * op  = nullptr;

//     Move (0);

//     if (*cur == '{')
//     {
//         Move (1);

//         if (*cur == '}')
//         {
//             Move (1);
//             return (SetNode (BLOCK, "B", 1));
//         }

//         while (*cur != '}')
//         {
//             op = ParseOp ();

//             CHECK (op, res);

//             op->left = res;

//             if (res)
//             {
//                 LINK (res, op);
//             }

//             res = op;

//             Move (0);
//         }

//         Move (1);

//         PRINT ("block <- }\n");

//         return (SetNode (BLOCK, "B", 1, nullptr, res));
//     }

//     STOP (NO_BLOCK, nullptr);
// }

// /*
//     Assignment :: Expression = Expression';' || Expression';'
// */
// Node<Token> * Parser::ParseAssignment ()
// {
//     PRINT ("assign\n");

//     Node<Token> * value = nullptr;
//     Node<Token> * res   = ParseExpression ();

//     CHECK (res, res);

//     Move (0);

//     if (*cur == '=')
//     {
//         PRINT ("=\n");

//         Move (1);

//         value = ParseExpression ();

//         CHECK (value, res);

//         res = SetNode (ASSIGN, "=", 1, res, value);

//         Move (0);
//     }

//     return (res);
// }

// /*
//     Op :: Block | CondOp | Assignment
// */
// Node<Token> * Parser::ParseOp ()
// {
//     PRINT ("op\n");

//     Node<Token> * res = nullptr;

//     Move (0);

//     if (!strncmp (cur, "if ", 3))
//     {
//         res = ParseIf ();
//     }
//     else if (!strncmp (cur, "while ", 6))
//     {
//         res = ParseWhile ();
//     }
//     else if (!strncmp (cur, "var ", 4))
//     {
//         res = ParseVar ();
//     }
//     else
//     {
//         res = ParseAssignment ();
//     }

//     CHECK (res, res);

//     return (SetNode (OPERATOR, "op", 2, nullptr, res));
// }

// /*
//     CondOp :: "while" '(' Expression ')' Block
// */
// Node<Token> * Parser::ParseWhile ()
// {
//     Node<Token> * cond  = nullptr;
//     Node<Token> * block = nullptr;

//     Move (0);

//     if (!strncmp (cur, "while", 5))
//     {
//         Move (5);

//         if (*cur == '(')
//         {
//             Move (1);

//             cond = ParseExpression ();

//             CHECK (cond, cond);

//             if (*cur != ')')
//             {
//                 STOP (UNCLOSED_BRACES, cond);
//             }

//             Move (1);

//             block = ParseBlock ();

//             CHECK (block, cond);

//             return (SetNode (WHILE, "while", 5, cond, block));
//         }
//     }

//     STOP (error, cond);
// }

// Node<Token> * Parser::ParseIf ()
// {
//     Node<Token> * res = SetNode (IF, "if", 2);
//     Node<Token> * tmp = nullptr;

//     Move (0);

//     if (!strncmp (cur, "if", 2))
//     {
//         Move (2);

//         if (*cur == '(')
//         {
//             Move (1);

//             res->left = ParseExpression ();

//             CHECK (res->left, res);

//             if (*cur != ')')
//             {
//                 STOP (UNCLOSED_BRACES, res);
//             }

//             LINK (res->left, res);
//             Move (1);

//             res->right = SetNode (LINK, "C", 1);
//             LINK (res->right, res);

//             tmp = res->right;

//             tmp->right = ParseBlock ();

//             CHECK (tmp->right, res);

//             LINK (tmp->right, tmp);

//             if (!strncmp (cur, "else", 4))
//             {
//                 Move (4);

//                 tmp->left = ParseBlock ();

//                 CHECK (tmp->left, res);

//                 LINK (tmp->left, tmp);
//             }

//             return (res);
//         }
//     }

//     STOP (error, res);
// }

// /*
//     Expression :: Or ('&' Or)*
// */
// Node<Token> * Parser::ParseExpression ()
// {
//     PRINT ("and\n");

//     Node<Token> * left = ParseOr ();
//     Node<Token> * right = nullptr;

//     CHECK (left, left);

//     Move (0);

//     while (*cur == '&')
//     {
//         Move (1);

//         right = ParseOr ();

//         CHECK (right, left);

//         left = SetNode (AND, "&", 1, left, right);

//         Move (0);
//     }

//     return (left);
// }

// /*
//     Or :: PrimaryBool '|' PrimaryBool+
// */
// Node<Token> * Parser::ParseOr ()
// {
//     PRINT ("or\n");

//     Node<Token> * left  = ParsePrimaryBool ();
//     Node<Token> * right = nullptr;

//     CHECK (left, left);

//     Move (0);

//     while (*cur == '|')
//     {
//         Move (1);

//         right = ParsePrimaryBool ();

//         CHECK (right, left);

//         left = SetNode (OR, "|", 1, left, right);

//         Move (0);
//     }

//     return (left);
// }

// /*
//     PrimaryBool :: '[' Cond ']' | Bool
// */
// Node<Token> * Parser::ParsePrimaryBool ()
// {
//     PRINT ("primary bool\n");

//     Node<Token> * res = nullptr;

//     Move (0);

//     if (*cur == '[')
//     {
//         Move (1);

//         res = ParseExpression ();

//         Move (0);

//         CHECK (res, res);

//         if (*cur != ']')
//         {
//             STOP (UNCLOSED_BRACES, res);
//         }

//         Move (1);

//         return (res);
//     }

//     return (ParseBool ());
// }

// #define CHECK_BOOL( type, op, op_len )                                  \
//         if (!strncmp (cur, op, op_len))                                 \
//         {                                                               \
//             Move (op_len);                                              \
//             right = ParseExpression ();                                 \
//                                                                         \
//             if (!right)                                                 \
//             {                                                           \
//                 STOP (error, left);                                     \
//             }                                                           \
//                                                                         \
//             return (SetNode (type, op, op_len, left, right));           \
//         }

// /*
//     Bool :: AddSub {<,>,==,<=,>=,!=} AddSub | AddSub
// */
// Node<Token> * Parser::ParseBool ()
// {
//     PRINT ("bool\n");

//     Move (0);

//     Node<Token> * left  = ParseAddSub ();
//     Node<Token> * right = nullptr;

//     CHECK (left, left);

//     Move (0);

//     CHECK_BOOL (GEQ,     ">=", 2);
//     CHECK_BOOL (LEQ,     "<=", 2);
//     CHECK_BOOL (EQ,      "==", 2);
//     CHECK_BOOL (NEQ,     "!=", 2);
//     CHECK_BOOL (GREATER, ">",  1);
//     CHECK_BOOL (LESS,    "<",  1);

//     return (left);
// }

// /*
//     AddSub :: MulDiv {+,-} MulDiv
// */
// Node<Token> * Parser::ParseAddSub ()
// {
//     PRINT ("addsub\n");

//     Move (0);

//     char op = 0;

//     Node<Token> * left  = ParseMulDiv ();
//     Node<Token> * right = nullptr;

//     CHECK (left, left);

//     Move (0);

//     while (*cur == '+' || *cur == '-')
//     {
//         op = *cur;
//         Move (1);

//         right = ParseMulDiv ();

//         CHECK (right, left);

//         if (op == '+')
//         {
//             left = SetNode (ADD, "+", 1, left, right);
//         }
//         else
//         {
//             left = SetNode (SUB, "-", 1, left, right);
//         }

//         Move (0);
//     }

//     return (left);
// }

// /*
//     MulDiv :: Primary {*,/} Primary
// */
// Node<Token> * Parser::ParseMulDiv ()
// {
//     PRINT ("muldiv\n");

//     Move (0);

//     char op = 0;

//     Node<Token> * left  = ParsePrimary ();
//     Node<Token> * right = nullptr;

//     Move (0);

//     if (!left)
//     {
//         STOP (error, left);
//     }

//     while (*cur == '*' || *cur == '/' || *cur == '%')
//     {
//         op = *cur;
//         Move (1);

//         right = ParsePrimary ();

//         CHECK (right, left);

//         if (op == '*')
//         {
//             left = SetNode (MUL, "*", 1, left, right);
//         }
//         else if (op == '/')
//         {
//             left = SetNode (DIV, "/", 1, left, right);
//         }
//         else
//         {
//             left = SetNode (MOD, "%", 1, left, right);
//         }

//         Move (0);
//     } 

//     return (left);
// }

// /*
//     Primary :: Id Args | Var 
// */
// Node<Token> * Parser::ParsePrimary ()
// {
//     PRINT ("primary ");

//     Node<Token> * res  = nullptr;
//     Node<Token> * id   = nullptr; 

//     Move (0);

//     if (isalpha (*cur))
//     {
//         PRINT ("-> id\n");

//         id = ParseId ();

//         CHECK (id, id);

//         Move (0);

//         if (*cur == '(')
//         {
//             PRINT ("-> call\n");

//             res = ParseCall ();

//             CHECK (res, id);

//             res->left = id;
//             LINK (res->left, res);

//             return (res);
//         }

//         return (id);
//     }

//     if (*cur == '(')
//     {
//         PRINT ("-> (\n");
//         Move (1);

//         res = ParseExpression ();

//         CHECK (res, res);

//         if (*cur != ')')
//         {
//             STOP (UNCLOSED_BRACES, res);
//         }

//         PRINT ("primary <- )\n");
//         Move (1);
//         return (res);
//     }

//     if (isdigit (*cur) || (*cur == '-' && isdigit(*(cur + 1))))
//     {
//         PRINT ("-> num\n");
//         return (ParseNum ());
//     }

//     if (*cur == '\'')
//     {
//         return (ParseChar ());
//     }

//     if (*cur == '"')
//     {
//         return (ParseString ());
//     }

//     STOP (PRIMARY_EXPECT, nullptr);
// }

// Node<Token> * Parser::ParseCall ()
// {
//     PRINT ("call\n");

//     Node<Token> * args = nullptr;

//     if (*cur == '(')
//     {
//         Move (1);

//         if (*cur == ')')
//         {
//             Move (1);
//             return (SetNode (CALL, "call", 4));
//         }

//         args = ParseSequence ();

//         CHECK (args, args);

//         if (*cur != ')')
//         {
//             STOP (UNCLOSED_BRACES, args);
//         }

//         Move (1);

//         return (SetNode (CALL, "call", 4, nullptr, args));
//     }
// }

// /*
//     Sequence :: Expression ',' Expression+
// */
// Node<Token> * Parser::ParseSequence ()
// {
//     PRINT ("seq\n");

//     Move (0);

//     Node<Token> * res = ParseExpression ();
//     Node<Token> * tmp = nullptr;

//     CHECK (res, res);

//     res = SetNode (COMMA, ",", 1, nullptr, res);

//     Move (0);

//     while (*cur == ',')
//     {
//         Move (1);

//         tmp = ParseExpression ();

//         CHECK (tmp, res);

//         res = SetNode (COMMA, ",", 1, res, tmp);

//         Move (0);
//     }

//     return (res);
// }

// Node<Token> * Parser::ParseChar ()
// {
//     PRINT ("char\n");

//     if (*cur != '\'')
//     {
//         STOP (error, nullptr);
//     }

//     if (*(cur + 2) != '\'')
//     {
//         STOP (UNCLOSED_BRACES, nullptr);
//     }

//     cur += 1;

//     char * ch = (char *) calloc (1, sizeof (char));

//     sscanf (cur, "%[^']", ch);

//     Move (2);

//     return (SetNode (CHAR, ch, 1));
// }

// Node<Token> * Parser::ParseString ()
// {
//     PRINT ("string\n");

//     if (*cur != '"')
//     {
//         STOP (error, nullptr);
//     }

//     cur += 1;

//     size_t len = 0;
//     char * s = (char *) calloc (100, sizeof (char));

//     sscanf (cur, "%[^\"]%n", s, &len);

//     Move (len);

//     if (*cur != '"')
//     {
//         STOP (UNCLOSED_BRACES, nullptr);
//     }

//     Move (1);

//     return (SetNode (STRING, s, len));
// }

// /*
//     Id :: [a-zA-Z_]+
// */
// Node<Token> * Parser::ParseId ()
// {
//     PRINT ("id\n");

//     Node<Token> * tmp = nullptr;

//     Move (0);

//     size_t len = 0;
//     char * id  = GetId (&len);

//     if (!id)
//     {
//         STOP (ID_EXPECT, nullptr);
//     }

//     return (SetNode (ID, id, len));
// }

// /*
//     Парсит имя вида Typical_Name и возвращает указатель на строку
// */
// char * Parser::GetId (size_t * len)
// {
//     if (isdigit (*cur))
//     {
//         return (nullptr);
//     }

//     sscanf (cur, "%*[a-zA-Z0-9_]%n", len);

//     if (!(*len))
//     {
//         return (nullptr);
//     }

//     char * id = (char *) calloc (*len, sizeof (*id));

//     strncpy (id, cur, *len);

//     Move (*len);

//     PRINT (id); PRINT ("\n");

//     return (id);
// }

// /*
//     Num :: {-}[0-9]+
// */
// Node<Token> * Parser::ParseNum ()
// {
//     PRINT ("num\n");

//     Move (0);

//     size_t len = 0;
//     char * num = GetNum (&len);

//     if (!num)
//     {
//         STOP (NUM_EXPECT, nullptr);
//     }

//     return (SetNode (NUM, num, len));
// }

// /*
//     Парсит число с плавающей точкой и возвращает его в виде строки
// */
// char * Parser::GetNum (size_t * len)
// {
//     sscanf (cur, "%*lf%n", len);

//     if (!(*len))
//     {
//         return (nullptr);
//     }

//     char * num = (char *) calloc (*len, sizeof (*num));

//     strncpy (num, cur, *len);

//     cur += *len;

//     PRINT (num); PRINT ("\n");

//     return (num);
// }

// /*
//     Пропускает пробельные символы
// */
// void Parser::SkipSpaces ()
// {
//     while (*cur == ' ' || *cur == '\t' || *cur == '\n' || *cur == '\r')
//     {
//         if (*cur == '\n')
//         {
//             line++;
//             newline = cur;
//         }

//         cur++;
//     }
// }

// /*
//     Сдвигает курсор на len, затем пропускает пробелы и комментарии
// */
// void Parser::Move (size_t len)
// {
//     cur += len;

//     SkipSpaces  ();
//     SkipComment ();
//     SkipSpaces  ();
// }

// /*
//     Комментарии:
//         $$ my_comment

//         или

//         $ my
//           comment $
// */

// void Parser::SkipComment ()
// {
//     if (!strncmp (cur, "$$", 2))
//     {
//         cur += 2;

//         while (*cur != '\n')
//         {
//             cur++;
//         }

//         cur++;
//     }
//     else if (*cur == '$')
//     {
//         cur += 1;

//         while (*cur != '$')
//         {
//             cur++;
//         }

//         cur += 1;
//     }
// }

