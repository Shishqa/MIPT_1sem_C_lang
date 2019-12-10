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
const size_t OP_CNT = 33;

struct Operator 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    int (* op) (const int, const int);

    char type;
    size_t priority;

    size_t max_dist;
    size_t min_dist;
};

enum opcodes
{
    UNDEF,
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
    LEFT_BL,
    RIGHT_BL,
    ASSIGN,
    DEF_VAR,
    RET,
    IF,
    ELSE,
    WHILE,
    ID_REQ,
    DEF_FUNC,
    CALL,
    DEF,
    LINK,
    OPERATOR,
    BLOCK
};

static const Operator operators[OP_CNT] = {
    {"@",       1,  UNDEF,    nullptr,   0},
    {"!=",      2,  NEQ,      my_neq,    MATH_TYPE,   0,   9, 4},
    {"!",       1,  NOT,      my_not,    MATH_TYPE,   2,   9, 3},
    {"+",       1,  ADD,      my_add,    MATH_TYPE,   0,   9, 2},
    {"-",       1,  SUB,      my_sub,    MATH_TYPE,   0,   9, 1},
    {"*",       1,  MUL,      my_mul,    MATH_TYPE,   1,   9, 9},
    {"/",       1,  DIV,      my_div,    MATH_TYPE,   1,   8, 4},
    {"%",       1,  MOD,      my_mod,    MATH_TYPE,   1,   8, 3},
    {"&",       1,  AND,      my_and,    MATH_TYPE,   0,   8, 2},
    {"|",       1,  OR ,      my_or,     MATH_TYPE,   1,   8, 1},
    {"<=",      2,  LEQ,      my_leq,    MATH_TYPE,   0,   8, 8},
    {">=",      2,  GEQ,      my_geq,    MATH_TYPE,   0,   7, 3},
    {"<",       1,  LESS,     my_less,   MATH_TYPE,   0,   7, 2},
    {">",       1,  GRTR,     my_grtr,   MATH_TYPE,   0,   7, 1},
    {"==",      2,  EQ,       my_eq,     MATH_TYPE,   0,   7, 7},
    {",",       1,  COMMA,    my_com,    MATH_TYPE,   0,   6, 3},
    {"(",       1,  LEFT_B,   nullptr,   MATH_TYPE,   0,   6, 2},
    {")",       1,  RIGHT_B,  nullptr,   MATH_TYPE,   0,   6, 1},
    {"{",       1,  LEFT_BL,  nullptr,   OP_TYPE,     0,   6, 6},
    {"}",       1,  RIGHT_BL, nullptr,   OP_TYPE,     0,   5, 2},    
    {"=",       1,  ASSIGN,   nullptr,   MATH_TYPE,   0,   5, 1},
    {"var",     3,  DEF_VAR,  nullptr,   OP_TYPE,     0,   5, 5},
    {"return",  6,  RET,      nullptr,   OP_TYPE,     0,   3, 1},
    {"if",      2,  IF,       nullptr,   OP_TYPE,     0,   3, 3},
    {"else",    4,  ELSE,     nullptr,   OP_TYPE,     0,   2, 1},
    {"while",   5,  WHILE,    nullptr,   OP_TYPE,     0,   2, 2},
    {"{idreq}", 7,  ID_REQ,   nullptr,   OP_TYPE,     0,   1, 1},
    {"def",     3,  DEF_FUNC, nullptr,   OP_TYPE,     0},
    {"#CALL",   5,  CALL,     nullptr,   OP_TYPE,     0},
    {"#D",      2,  DEF,      nullptr,   OP_TYPE,     0},
    {"#C",      2,  LINK,     nullptr,   OP_TYPE,     0},
    {"#O",      2,  OPERATOR, nullptr,   OP_TYPE,     0},
    {"#B",      2,  BLOCK,    nullptr,   OP_TYPE,     0}
};

/*
1 - 1
2 - 2, 1
3 - 3, 1
4 - ВЫПИЛЕНО
5 - 5, 1, 2
6 - 6, 1, 2, 3
7 - 7, 1, 2, 3
8 - 8, 1, 2, 3, 4
9 - 9, 1, 2, 3, 4
*/

#endif // !OPERATORS_HPP_