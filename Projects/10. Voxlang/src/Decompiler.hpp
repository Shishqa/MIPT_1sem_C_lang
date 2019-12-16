#ifndef DECOMPILER_HPP_
#define DECOMPILER_HPP_

#include "libraries.hpp"
#include "Parser.hpp"

const size_t MAX_CHORD_NUM = 100;
const size_t MAX_NOTES = 6;

struct Chord
{
    size_t num_of_notes;
    char * bass[MAX_NOTES];
    char * high[MAX_NOTES];
};
class Harmony
{
    public:
    size_t curr_chord;
    size_t chord_cnt;

    Chord chords[MAX_CHORD_NUM];
    
    void init (const char * path);
    void next ();
};

class Decompiler
{
    Harmony hrmny;

    size_t offset;
    size_t func_cnt;

    BinaryTree<Token> * prog;

    FILE * out;

    void Proceed (FILE * f);

    void Calculate     (Node<Token> * node);
    void GetOperator   (Node<Token> * node);
    void PrintNum      (Node<Token> * node);
    void PrintId       (Node<Token> * node);
    void PrintCall     (Node<Token> * node);
    void PrintOp       (size_t opcode);

public:
    bool GetCode (BinaryTree<Token> * code, const char * output_path, const char * harmony_path = "harmonies/CAmGDm");
};

#endif // !DECOMPILER_HPP_