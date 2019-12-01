#include "libraries.hpp"

#include "Translator.hpp"

int main () 
{
    Translator t = {};

    //t.GetAsmCode ("programs/get_chars/main.plan", "programs/get_chars/main.asm");
    t.BuildAndRun ("programs/get_chars/main.plan", "programs/get_chars/test.bin");

    return (0);
}