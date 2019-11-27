#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "libraries.hpp"

enum lexem_types
{
    DEFINITION,
    VARIABLE,
    FUNC_TYPE,
    FUNC_MARKER,
    OP_TYPE,
    OP_MARKER,
    BOOL_TYPE,
    NUM_TYPE,
    ID_TYPE,
    NEWLINE,
    BRAC,
    FOR,
    IF,
    WHILE,
    SEQ
};

struct Token
{
    char   type;

    const char * lexem;
    size_t lexem_len;
};

#endif // !TOKEN_HPP_

