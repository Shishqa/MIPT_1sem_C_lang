
#include "libraries.hpp"

#include "Parser.hpp"
#include "Operators.hpp"

int main () 
{
    Parser p = {};

    BinaryTree<Token> * my_tree = p.Parse ("programs/fibonacci/main.plan"); 

    FILE * f = fopen ("log/tree", "w");
    my_tree->print (f, 'i', TokenPrinter);
    fclose (f);

    //printf ("%s", buf);

    //t.tokenize (buf);

    //t.GetAsmCode ("programs/fibonacci/main.plan", "programs/fibonacci/main.asm");
    //t.BuildAndRun ("programs/get_chars/main.plan", "programs/get_chars/test.bin");

    // Parser p = {};

    // BinaryTree<Token> * prog = p.ParseFile ("programs/fibonacci/main.plan");

    // Decompiler d = {};

    // d.GetCode (prog, "programs/fibonacci/DECOMPILED.plan");

    return (0);
}