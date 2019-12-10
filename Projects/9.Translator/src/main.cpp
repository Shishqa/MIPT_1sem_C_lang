
#include "libraries.hpp"

#include "Parser.hpp"
#include "Operators.hpp"

#include "Decompiler.hpp"

#include "Translator.hpp"

int main () 
{
    Parser p = {};

    BinaryTree<Token> * my_tree = p.Parse ("programs/fibonacci/main.plan"); 

    Decompiler d = {};

    d.GetCode (my_tree, "programs/fibonacci/DEC.plan");

    Translator t = {};

    t.BuildAndRun (my_tree, "programs/fibonacci/main.bin");

    return (0);
}