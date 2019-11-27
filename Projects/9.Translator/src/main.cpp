#include "libraries.hpp"

#include "Parser.hpp"

int main () 
{
    Parser p = {};

    BinaryTree<Token> * prog = p.ParseFile ("test");

    assert (prog);
    assert (prog->root);

    prog->dotDump (PrintToken, 1);

    return (0);
}