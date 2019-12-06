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

BinaryTree<Token> * SongParser::Parse (const char * path)
{
    FILE * in = fopen (path, "r");

    buff = nullptr;
    size_t file_len = Read (&buff, in);

    fclose (in);

    num_of_lines = ArrangePointers (buff, file_len, &code);

    cur = buff;
    curr_line = 0;

    result = (BinaryTree<Token> *) calloc (1, sizeof (*result));

    result->init ();
    result->root = ParseGrammar ();

    free (code);
    free (buff);

    if (error != PARSE_OK)
    {
        SETCOLOR (RED);
        printf ("FAILED\n");
        SETCOLOR (RESET);

        free (result);
        return (nullptr);
    }

    return (result);
}

Node<Token> * SongParser::ParseGrammar ()
{
    Node<Token> * res = nullptr;

    while (cur != '\0')
    {
        res = SetNode (DEFINITION, "D", 1, res, ParseDefinition ());
        SkipSpaces ();
    }
}

Node<Token> * SongParser::ParseDefinition ()
{
    if (strncmp (cur, "Song", 4) != 0)
    {
        error = SYNTAX_ERROR;
        return (nullptr);
    }

    SkipSpaces (4);

    Node<Token> * res = SetNode (DEF_FUNC, "def", 3, nullptr, ParseId ());
    CHECK ();

    res->right->right = ParseBlock ();
    CHECK ();
    LINK (res->right->right, res->right);

    res->left = ParseVarList ();
    CHECK ();
    LINK (res->left, res);

    return (res);
}

Node<Token> * SongParser::ParseBlock ()
{
    Node<Token> * res = SetNode (BLOCK, "B", 1);

    SkipSpaces ();
    InitStaff ();
    CHECK ();

    int op = GetOp ();

    while (op != END)
    {
        switch (op)
        {
            case IF:
                res->right = ParseIf ();
            break;

            case WHILE:
                res->right = ParseWhile ();
            break;

            case DEF_VAR:
                res->right = ParseVar ();
            break;
        
            default:
                res->right = ParseAssignment ();
            break;
        }

        CHECK ();
        LINK (res->right, res);

    }
}

Node<Token> * SongParser::ParseOp ()
{

}

bool SongParser::InitStaff ()
{
    error = EXPECT_STAFF;

    if (curr_line + 9 > num_of_lines)
    {
        return (false);
    }

    if (code[curr_line].end - code[curr_line].begin <= 2)
    {
        return (false);
    }

    for (size_t i = curr_line; i < curr_line + 9; i++)
    {
        if (*(code[i].begin) != '|' || *(code[i].end - 1) != '|')
        {
            return (false);
        }
    }

    error = PARSE_OK;

    cur = code[curr_line].begin + 1;

    return (true);
}

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

int SongParser::GetOp ()
{
    Chord ch = GetChord ();

    if (ch.max_d == 10 && ch.min_d == 10)
    {
        return (END);
    }

    if (ch.max_d == 0)
    {
        return (-ch.min_d);
    }

    for (int i = 1; i < OP_CNT; i++)
    {
        if (ch.min_d == operators[i].min_d &&
            ch.max_d == operators[i].max_d)
        {
            return (i);
        }
    }
}

Chord SongParser::GetChord ()
{
    Chord res = {};

    size_t offset = cur - code[curr_line].begin;

    res.min_d = 10;
    res.max_d = 0;

    int first = -1;
    int prev = -1;

    size_t notes_met = 0;

    for (int i = curr_line; i < curr_line + 9; i++)
    {
        printf ("%c", *(code[i].begin + offset));

        if (*(code[i].begin + offset) == '@')
        {
            notes_met++;

            if (prev != -1 && res.min_d > i - prev)
            {
                res.min_d = i - prev;
            }

            prev = i;

            if (first != -1)
            {
                res.max_d = i - first;
            }
            else
            {
                first = i;
            }
        }
        else if (*(code[i].begin + offset) == '|')
        {
            res.min_d = 10;
            res.max_d = 10;
            return (res);
        }
        else if (*(code[i].begin + offset) != ' ' &&
                 *(code[i].begin + offset) != '-')
        {
            error = SYNTAX_ERROR;
            return (res);
        }
    }

    if (notes_met == 1)
    {
        res.min_d = first - curr_line + 1;
    }
    else if (!notes_met)
    {
        res.min_d = 0;
    }

    return (res);
}