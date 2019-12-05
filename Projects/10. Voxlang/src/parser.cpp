#include "head.hpp"

#include "parser.hpp"

BinaryTree<Token> * SongParser::Parse (const char * path)
{
    FILE * in = fopen (path, "r");

    buff = nullptr;
    size_t file_len = Read (&buff, in);

    fclose (in);

    num_of_lines = ArrangePointers (buff, file_len, &code);

    cur = buff;
    curr_line = 0;

    BinaryTree<Token> * res = (BinaryTree<Token> *) calloc (1, sizeof (*res));

    res->init ();
    res->root = ParseGrammar ();

    free (code);
    free (buff);

    if (!res->root)
    {
        SETCOLOR (RED);
        printf ("FAILED\n");
        SETCOLOR (RESET);

        free (res);
        return (nullptr);
    }

    return (res);
}

Node<Token> * SongParser::ParseGrammar ()
{
    while (*cur != '|')
    {
        Next ();
    }

    Chord check = {};
    
    for (int i = 0; i < 20; i++)
    {
        check = GetChord ();
        cur++;

        printf ("%lu %lu\n", check.min_d, check.max_d);
    }

    return (nullptr);
}

void SongParser::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\t' || *cur == '\r')
    {
        cur++;
    }

    if (*cur == '\0')
    {
        Next ();
    }

    SkipSpaces ();
}

void SongParser::Next ()
{
    if (curr_line < num_of_lines - 1)
    {
        curr_line++;
        cur = code[curr_line].begin;
    }
}

Chord SongParser::GetChord ()
{
    printf ("parsing on line %lu\n", curr_line);

    size_t offset = cur - code[curr_line].begin;

    Chord res = {};
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
                printf ("* upd min");
                res.min_d = i - prev;
            }

            prev = i;

            if (first != -1)
            {
                res.max_d = i - first;
                printf ("* upd max");
            }
            else
            {
                printf ("* set first");
                first = i;
            }
        }

        printf ("\n");
    }

    if (notes_met == 1)
    {
        res.max_d = first - curr_line + 1;
    }

    return (res);
}