#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "libraries.hpp"

enum lexem_types
{
    DEF_FUNC,
    FUNC,
    DEF_VAR,
    ID,
    BLOCK,
    OPERATOR,
    SEQUENCE,
    NUM,
    ASSIGN,
    IF,
    WHILE,
    AND,
    OR,
    G,
    L,
    EQ,
    NEQ,
    LEQ,
    GEQ,
    ADD,
    SUB,
    MUL,
    DIV,
    INC,
    DEC,
    VOID
};

struct Token
{
    char   type;

    const char * lexem;
    size_t lexem_len;
};

#endif // !TOKEN_HPP_

