
#include "libraries.hpp"

#include "Tokenizer.hpp"
#include "Operators.hpp"

int main () 
{
    Tokenizer t = {};

    FILE * in = fopen ("programs/fibonacci/main.plan", "r");

    char * buf = nullptr;
    Read (&buf, in);

    fclose (in);

    //printf ("%s", buf);

    t.tokenize (buf);

    //t.GetAsmCode ("programs/fibonacci/main.plan", "programs/fibonacci/main.asm");
    //t.BuildAndRun ("programs/get_chars/main.plan", "programs/get_chars/test.bin");

    // Parser p = {};

    // BinaryTree<Token> * prog = p.ParseFile ("programs/fibonacci/main.plan");

    // Decompiler d = {};

    // d.GetCode (prog, "programs/fibonacci/DECOMPILED.plan");

    return (0);
}