#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "libraries.hpp"

enum lexem_types
{
    FUNC_TYPE,
    FUNC_MARKER,
    OP_TYPE,
    OP_MARKER,
    NUM_TYPE,
    ID_TYPE,
    NEWLINE,
    BRAC
};

struct Token
{
    char   type;

    char * lexem;
    size_t lexem_len;
};

#endif // !TOKEN_HPP_

