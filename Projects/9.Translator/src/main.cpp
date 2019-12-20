
#include "libraries.hpp"

#include "Parser.hpp"
#include "Operators.hpp"

#include "Decompiler.hpp"

#include "Translator.hpp"

int main (int argc, char ** argv) 
{
    if (argc <= 1)
    {
        printf ("not enough arguments, type:\n");
        printf ("./plan <filename> <flags>\n");
        return (0);
    }

    char * path = argv[1];

    Parser p = {};

    BinaryTree<Token> * my_tree = p.Parse (path); 

    assert (my_tree);
    printf ("parsed\n");

    FILE * log = fopen ("saved_tree1", "w");
    my_tree->print (log, 'p', TokenPrinter, '{', '}', '@');
    fclose (log);  

    printf ("printed\n");

    if (argc >= 2 && !strncmp (argv[2], "-f", 2))
    {
        return (0);
    }

    Decompiler d = {};

    d.GetCode (my_tree, "DECOMPILED.plan");

    Translator t = {};

    t.BuildAndRun (my_tree, "main.bin");

    return (0); 
}