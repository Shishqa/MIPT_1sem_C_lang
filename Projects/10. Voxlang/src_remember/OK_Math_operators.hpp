#ifndef MATH_OPERATORS_HPP_
#define MATH_OPERATORS_HPP_

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

const size_t MAX_OP_NAME_LEN = 10;
const size_t OP_CNT = 29;

struct Operator 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    int (* op) (const int, const int);

    size_t max_dist;
    size_t min_dist;

    char type;
};

enum opcodes
{
    UNDEF,
    NOT,
    ADD,
    SUB,
    MUL,
    DIV,
    AND,
    OR,
    LESS,
    GRTR,
    LEQ,
    GEQ,
    EQ,
    NEQ,
    LEFT_B,
    RIGHT_B,
    ID_REQ,
    DEF_FUNC,
    DEF_VAR,
    CALL,
    RET,
    DEF,
    BLOCK,
    OPERATOR,
    COMMA,
    IF,
    LINK,
    WHILE,
    ASSIGN
};

static const Operator operators[OP_CNT] = {
    {"@",      1,  UNDEF,    nullptr,   0, 0, 0},
    {"!",      1,  NOT,      my_not,    8, 8, MATH_TYPE},
    {"+",      1,  ADD,      my_add,    8, 1, MATH_TYPE},
    {"-",      1,  SUB,      my_sub,    8, 2, MATH_TYPE},
    {"*",      1,  MUL,      my_mul,    8, 3, MATH_TYPE},
    {"/",      1,  DIV,      my_div,    8, 4, MATH_TYPE},
    {"&",      1,  AND,      my_and,    7, 7, MATH_TYPE},
    {"|",      1,  OR ,      my_or,     7, 1, MATH_TYPE},
    {"<",      1,  LESS,     my_less,   7, 2, MATH_TYPE},
    {">",      1,  GRTR,     my_grtr,   7, 3, MATH_TYPE},
    {"<=",     2,  LEQ,      my_leq,    6, 6, MATH_TYPE},
    {">=",     2,  GEQ,      my_geq,    6, 1, MATH_TYPE},
    {"==",     2,  EQ,       my_eq,     6, 2, MATH_TYPE},
    {"!=",     2,  NEQ,      my_neq,    6, 3, MATH_TYPE},
    {"(",      1,  LEFT_B,   nullptr,   5, 5, OP_TYPE},
    {")",      1,  RIGHT_B,  nullptr,   5, 1, OP_TYPE},
    {"ID",     2,  ID_REQ,   nullptr,   5, 2, OP_TYPE},
    {"DEF",    3,  DEF_FUNC, nullptr,   4, 4, OP_TYPE},
    {"VAR",    3,  DEF_VAR,  nullptr,   4, 1, OP_TYPE},
    {"CALL",   4,  CALL,     nullptr,   4, 2, OP_TYPE},
    {"RET",    3,  RET,      nullptr,   3, 3, OP_TYPE},
    {",",      1,  COMMA,    nullptr,   3, 1, OP_TYPE},
    {"IF",     2,  IF,       nullptr,   2, 2, OP_TYPE},
    {"WH",     1,  WHILE,    nullptr,   2, 1, OP_TYPE},
    {"ASSN",   4,  ASSIGN,   nullptr,   1, 1, OP_TYPE},
    {"D",      1,  DEF,      nullptr,   0, 0, OP_TYPE},
    {"C",      1,  LINK,     nullptr,   0, 0, OP_TYPE},
    {"OP",     2,  OPERATOR, nullptr,   0, 0, OP_TYPE},
    {"B",      1,  BLOCK,    nullptr,   0, 0, OP_TYPE}
};

#endif // !MATH_OPERATORS_HPP_