
#include "libraries.hpp"

#include "Translator.hpp"

#include "Decompiler.hpp"

int main () 
{
    Translator t = {};

    //t.GetAsmCode ("programs/fibonacci/main.plan", "programs/fibonacci/main.asm");
    t.BuildAndRun ("programs/get_chars/main.plan", "programs/get_chars/test.bin");

    // Parser p = {};

    // BinaryTree<Token> * prog = p.ParseFile ("programs/fibonacci/main.plan");

    // Decompiler d = {};

    // d.GetCode (prog, "programs/fibonacci/DECOMPILED.plan");

    return (0);
}