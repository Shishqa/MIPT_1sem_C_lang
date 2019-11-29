#include "libraries.hpp"

#include "Translator.hpp"

int main () 
{
    Translator t = {};

    t.Build ("programs/fibonacci/main.plan", "programs/fibonacci/test.bin");

    return (0);
}