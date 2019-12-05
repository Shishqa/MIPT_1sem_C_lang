#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "libraries.hpp"

enum lexem_types
{
    DEF_FUNC,
    DEF_VAR,
    ID,
    NUM,
    CALL,
    CHAR,
    STRING,
    DEFINITION,
    BLOCK,
    OPERATOR,
    COMMA,
    IF,
    LINK,
    WHILE,
    ASSIGN,
    AND,
    OR,
    GREATER,
    LESS,
    EQ,
    NEQ,
    LEQ,
    GEQ,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD
};

struct Token
{
    char   type;

    const char * lexem;
    size_t lexem_len;
};

void PrintToken (FILE * out, const void * data);

#endif // !TOKEN_HPP_

