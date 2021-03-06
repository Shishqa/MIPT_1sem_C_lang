#ifndef OPERATORS_HPP_
#define OPERATORS_HPP_

#include "Token.hpp"
#include "math.h"

static int my_add (const int l, const int r)
{
    return (l + r);
}

static int my_mul (const int l, const int r)
{
    return (l * r);
}

static int my_sub (const int l, const int r)
{
    return (l - r);
}

static int my_div (const int l, const int r)
{
    return (l / r);
}

static int my_mod (const int l, const int r)
{
    return (l % r);
}

static int my_and (const int l, const int r)
{
    return (l && r);
}

static int my_or (const int l, const int r)
{
    return (l || r);
}

static int my_less (const int l, const int r)
{
    return (l < r);
}

static int my_grtr (const int l, const int r)
{
    return (l > r);
}

static int my_leq (const int l, const int r)
{
    return (l <= r);
}

static int my_geq (const int l, const int r)
{
    return (l >= r);
}

static int my_eq (const int l, const int r)
{
    return (l == r);
}

static int my_neq (const int l, const int r)
{
    return (l != r);
}

static int my_not (const int l, const int r)
{
    return (!r);
}

static int my_com (const int l, const int r)
{
    return (l);
}

const size_t MAX_OP_NAME_LEN = 10;
const size_t OP_CNT = 31;

struct Operator 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    int (* op) (const int, const int);

    char type;
    size_t priority;

    char sequence[4];
};

enum opcodes
{
    NEQ,
    NOT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    LEQ,
    GEQ,
    LESS,
    GRTR,
    EQ,
    COMMA,
    LEFT_B,
    RIGHT_B,
    ASSIGN,
    DEF_VAR,
    RET,
    IF,
    ELSE,
    WHILE,
    RIGHT_BL,
    LEFT_BL,
    DEF_FUNC,
    CALL,
    DEF,
    LINK,
    OPERATOR,
    BLOCK
};

static const Operator operators[OP_CNT] = {
    {"!=",     2,  NEQ,      my_neq,    MATH_TYPE,   0, {3, 2, 0, 1}},
    {"!",      1,  NOT,      my_not,    MATH_TYPE,   2, {1, 3, 1, 1}}, 
    {"+",      1,  ADD,      my_add,    MATH_TYPE,   0, {1, 0, 1, 3}}, 
    {"-",      1,  SUB,      my_sub,    MATH_TYPE,   0, {1, 3, 3, 1}}, 
    {"*",      1,  MUL,      my_mul,    MATH_TYPE,   1, {1, 3, 0, 3}}, 
    {"/",      1,  DIV,      my_div,    MATH_TYPE,   1, {1, 3, 3, 2}}, 
    {"%",      1,  MOD,      my_mod,    MATH_TYPE,   1, {1, 3, 3, 3}}, 
    {"&",      1,  AND,      my_and,    MATH_TYPE,   0, {3, 3, 2, 0}}, 
    {"|",      1,  OR ,      my_or,     MATH_TYPE,   1, {3, 3, 1, 2}}, 
    {"<=",     2,  LEQ,      my_leq,    MATH_TYPE,   0, {2, 1, 1, 1}}, 
    {">=",     2,  GEQ,      my_geq,    MATH_TYPE,   0, {2, 1, 0, 1}}, 
    {"<",      1,  LESS,     my_less,   MATH_TYPE,   0, {0, 1, 1, 2}}, 
    {">",      1,  GRTR,     my_grtr,   MATH_TYPE,   0, {2, 1, 2, 2}}, 
    {"==",     2,  EQ,       my_eq,     MATH_TYPE,   0, {3, 2, 1, 2}}, 
    {",",      1,  COMMA,    my_com,    MATH_TYPE,   0, {1, 0, 3, 2}}, 
    {"(",      1,  LEFT_B,   nullptr,   MATH_TYPE,   0, {1, 2, 0, 2}}, 
    {")",      1,  RIGHT_B,  nullptr,   MATH_TYPE,   0, {1, 2, 0, 1}}, 
    {"=",      1,  ASSIGN,   nullptr,   MATH_TYPE,   0, {1, 0, 1, 2}}, 
    {"var",    3,  DEF_VAR,  nullptr,   OP_TYPE,     0, {2, 1, 3, 2}}, 
    {"return", 6,  RET,      nullptr,   OP_TYPE,     0, {3, 0, 0, 3}}, 
    {"if",     2,  IF,       nullptr,   OP_TYPE,     0, {2, 2, 3, 1}}, 
    {"else",   4,  ELSE,     nullptr,   OP_TYPE,     0, {2, 2, 3, 2}}, 
    {"while",  5,  WHILE,    nullptr,   OP_TYPE,     0, {2, 2, 1, 3}}, 
    {"}",      1,  RIGHT_BL, nullptr,   OP_TYPE,     0, {2, 0, 3, 2}}, 
    {"{",      1,  LEFT_BL,  nullptr,   OP_TYPE,     0, {2, 3, 2, 3}}, 
    {"def",    3,  DEF_FUNC, nullptr,   OP_TYPE,     0, {0, 0, 0, 0}}, 
    {"CALL",   4,  CALL,     nullptr,   OP_TYPE,     0, {0, 0, 0, 0}}, 
    {"D",      1,  DEF,      nullptr,   OP_TYPE,     0, {0, 0, 0, 0}},
    {"C",      1,  LINK,     nullptr,   OP_TYPE,     0, {0, 0, 0, 0}},
    {"O",      1,  OPERATOR, nullptr,   OP_TYPE,     0, {0, 0, 0, 0}},
    {"B",      1,  BLOCK,    nullptr,   OP_TYPE,     0, {0, 0, 0, 0}}
};

#endif // !OPERATORS_HPP_