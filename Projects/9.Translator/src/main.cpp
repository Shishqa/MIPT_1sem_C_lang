#include "libraries.hpp"

#include "Translator.hpp"

int main () 
{
    Translator t = {};

    t.BuildAndRun ("programs/fibonacci/main.plan", "programs/fibonacci/test.bin");

    return (0);
}