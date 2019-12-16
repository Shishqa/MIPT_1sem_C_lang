
#include "libraries.hpp"

//#include "Parser.hpp"
#include "Operators.hpp"

#include "Decompiler.hpp"

//#include "Translator.hpp"
//#include "Tokenizer.hpp"


int main () 
{
    BinaryTree<Token> * prog = (BinaryTree<Token> *) calloc (1, sizeof (*prog));

    prog->init ("saved_tree_2", TokenReader);
    prog->dotDump (PrintToken, 111);

    printf ("hello\n");

    Decompiler d = {};

    d.GetCode (prog, "test_output.abc", "harmonies/AmGEmF");

    // prog->dotDump (PrintToken, 111);
    // printf ("printed!\n");

    // Tokenizer t = {};

    // FILE * in = fopen ("test_song", "r");
    // char * buf = nullptr;
    // Read (&buf, in);
    // fclose (in);

    // t.tokenize (buf);

    // Parser p = {};

    // BinaryTree<Token> * my_tree = p.Parse ("programs/fibonacci/main.plan"); 

    // assert (my_tree);

    // Decompiler d = {};

    // d.GetCode (my_tree, "programs/fibonacci/DEC.plan");

    // Translator t = {};

    // t.BuildAndRun (my_tree, "programs/fibonacci/main.bin");

    return (0); 
}