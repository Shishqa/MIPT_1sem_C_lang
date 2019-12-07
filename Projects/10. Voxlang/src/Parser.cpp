#include "head.hpp"
#include "OK_Parser.hpp"

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

    size_t num_of_lines = ArrangePointers (buff, file_len, &code);

    Node<Token> ** arr = tokenizer.tokenize (code, num_of_lines);

    printf ("SongParse::Parse - Tokenized\n");

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


