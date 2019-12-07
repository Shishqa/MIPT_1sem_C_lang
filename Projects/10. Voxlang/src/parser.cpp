#include "head.hpp"
#include "operators.hpp"
#include "CreateNode.hpp"

#include "parser.hpp"

#define CHECK()                 \
        if (error != PARSE_OK)  \
        {                       \
            if (res)            \
            {                   \
                free (res);     \
            }                   \
            return (nullptr);   \
        }                       

Node<Token> ** SongParser::Tokenize ()
{
    Node<Token> ** tokens = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*tokens));
    cnt_tokens = 0;

    Node<Token> ** names = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*tokens));
    cnt_tokens = 0;

    

    return (tokens);
}

BinaryTree<Token> * SongParser::Parse (const char * path)
{
    FILE * in = fopen (path, "r");

    buff = nullptr;
    size_t file_len = Read (&buff, in);

    fclose (in);

    num_of_lines = ArrangePointers (buff, file_len, &code);

    cur = buff;
    curr_line = 0;

    Node<Token> ** arr = Tokenize ();

    for (int i = 0; i < cnt_tokens; i++)
    {
        printf ("%s %lu %d\n", arr[i]->data.lexem, arr[i]->data.lexem_len, arr[i]->data.type);
    }

    // result = (BinaryTree<Token> *) calloc (1, sizeof (*result));

    // result->init ();
    // result->root = ParseGrammar ();

    // free (code);
    // free (buff);

    // if (error != PARSE_OK)
    // {
    //     SETCOLOR (RED);
    //     printf ("FAILED\n");
    //     SETCOLOR (RESET);

    //     free (result);
    //     return (nullptr);
    // }

    // return (result);
}

// Node<Token> * SongParser::ParseGrammar ()
// {
//     Node<Token> * res = nullptr;

//     while (*cur != '\0')
//     {
//         res = SetNode (DEFINITION, "D", 1, res, ParseDefinition ());
//         SkipSpaces ();
//     }
// }

// Node<Token> * SongParser::ParseDefinition ()
// {
//     if (strncmp (cur, "Song", 4) != 0)
//     {
//         error = SYNTAX_ERROR;
//         return (nullptr);
//     }

//     SkipSpaces (4);

//     Node<Token> * res = SetNode (DEF_FUNC, "def", 3, nullptr, ParseId ());
//     CHECK ();

//     res->right->right = ParseBlock ();
//     CHECK ();
//     LINK (res->right->right, res->right);

//     res->left = ParseVarList ();
//     CHECK ();
//     LINK (res->left, res);

//     return (res);
// }

// Node<Token> * SongParser::ParseBlock ()
// {
//     Node<Token> * res = SetNode (BLOCK, "B", 1);

//     SkipSpaces ();
//     InitStaff ();
//     CHECK ();

//     int op = GetOp ();

//     while (op != END)
//     {
//         res->right = ParseOp ();
//         CHECK ();
//         LINK (res->right, res);
//     }
// }

// Node<Token> * SongParser::ParseOp ()
// {

// }

void SongParser::SkipSpaces (size_t pre_step)
{
    if (cur + pre_step <= code[curr_line].end)
    {
        cur += pre_step;
    }

    while (*cur == ' ' || *cur == '\t' || *cur == '\r')
    {
        cur++;
    }

    if (*cur == '\0' && curr_line < num_of_lines - 1)
    {
        NextLine ();
        SkipSpaces ();
    }
}

void SongParser::NextLine ()
{
    curr_line++;
    cur = code[curr_line].begin;
}

