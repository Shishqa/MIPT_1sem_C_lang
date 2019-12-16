
#include "libraries.hpp"

#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Operators.hpp"

#include "Decompiler.hpp"

#include "Translator.hpp"

int main () 
{
    FILE * in = fopen ("my_song", "r");

    char * buf = nullptr;
    size_t n_symbols = Read (&buf, in);

    fclose (in);

    Tokenizer t = {};

    Node<Token> ** arr = t.tokenize (buf, n_symbols);

    printf ("DONE\n");

    assert (arr);

    for (size_t i = 0; arr[i] != nullptr; i++)
    {
        PrintToken (stdout, &(arr[i]->data));
        fflush (stdout);
        printf ("\n");
    }

    return (0);
}