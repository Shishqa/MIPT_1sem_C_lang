#ifndef DECOMPILER_SONG_HPP_
#define DECOMPILER_SONG_HPP_

#include "libraries.hpp"
#include "Harmony.hpp"

class SongDecompiler
{
    Harmony h;

    BinaryTree<Token> * prog;

    FILE * out;

    void Proceed (FILE * f);

    void Calculate  (Node<Token> * node);
    void GetOp      (Node<Token> * node);
    void GetNum     (Node<Token> * node);
    void GetId      (Node<Token> * node);

    void PrintOp    (size_t opcode);

public:
    bool GetCode (BinaryTree<Token> * code, const char * output_path, const char * harmony);
};

#endif // !DECOMPILER_SONG_HPP_