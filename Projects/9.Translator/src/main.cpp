#include "libraries.hpp"

#include "Parser.hpp"

int main () 
{
    Parser p = {};

    BinaryTree<Token> * prog = p.ParseFile ("test");

    if (!prog)
    {
        return (0);
    }

    prog->dotDump (PrintToken, 1);

    return (0);
}