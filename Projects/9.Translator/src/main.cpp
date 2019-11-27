#include "libraries.hpp"

#include "Parser.hpp"

int main () 
{
    Parser p = {};

    BinaryTree<Token> * prog = p.Parse ("var abacaba = 2 * (12 + 4);");

    prog->dotDump (PrintToken, 111);

    return (0);
}