#ifndef TOKENIZER_HPP_
#define TOKENIZER_HPP_

#include "libraries.hpp"

enum Tokenizer_errors
{
    TOKENIZER_OK,
    UNKNOWN_OPERATOR,
    EXPECTED_LINE
};

class Tokenizer
{
    size_t error;

    Node<Token> ** tokens;
    const size_t MAX_TOKENS = 10000;
    size_t n_tokens;

    const size_t MAX_NAME_LEN = 100;

    const char * cur;
    String * code;

    size_t line;
    size_t n_lines;

    void Proceed ();
    void ParseLine ();
    void SkipSpaces ();
    char FindLetter ();

    void ParseChords ();
    void ParseNum ();
    void ParseId ();
    void ParseOp ();

    void PrintError ();

    public:

    Node<Token> ** tokenize (char * str, size_t str_len);
};

#endif // !TOKENIZER_HPP_