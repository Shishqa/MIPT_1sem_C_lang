#include "libraries.hpp"

#include "Translator.hpp"

int main () 
{
    Translator t = {};

    t.GetAsmCode ("programs/fibonacci/main.plan", "programs/fibonacci/test.asm");

    return (0);
}