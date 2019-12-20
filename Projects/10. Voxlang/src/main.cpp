
#include "libraries.hpp"
#include "Operators.hpp"

#include "DecompilerSong.hpp"
#include "Harmony.hpp"

int main () 
{
    BinaryTree<Token> t = {};

    t.init ("saved_tree1", TokenReader);

    t.dotDump (DotTokenPrinter, 123);

    assert (t.root);

    SongDecompiler d = {};

    d.GetCode (&t, "test_decomp", "harmonies/AmGEmF");

    return (0); 
}  