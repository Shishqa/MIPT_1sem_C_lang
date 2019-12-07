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

const size_t MAX_OP_NAME_LEN = 10;
const size_t OP_CNT = 13 + 15;

struct Operator 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    int (* op) (const int, const int);

    size_t max_dist;
    size_t min_dist;
};

enum opcodes
{
    UNDEF,
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
    {"@",      1,  UNDEF,    nullptr,   0, 0},
    {"+",      1,  ADD,      my_add,    8, 1},
    {"-",      1,  SUB,      my_sub,    8, 2},
    {"*",      1,  MUL,      my_mul,    8, 3},
    {"/",      1,  DIV,      my_div,    8, 4},
    {"&",      1,  AND,      my_and,    7, 7},
    {"|",      1,  OR ,      my_or,     7, 1},
    {"<",      1,  LESS,     my_less,   7, 2},
    {">",      1,  GRTR,     my_grtr,   7, 3},
    {"<=",     2,  LEQ,      my_leq,    6, 6},
    {">=",     2,  GEQ,      my_geq,    6, 1},
    {"==",     2,  EQ,       my_eq,     6, 2},
    {"!=",     2,  NEQ,      my_neq,    6, 3},
    {"(",      1,  LEFT_B,   nullptr,   5, 5},
    {")",      1,  RIGHT_B,  nullptr,   5, 1},
    {"ID",     2,  ID_REQ,   nullptr,   5, 2},
    {"DEF",    3,  DEF_FUNC, nullptr,   4, 4},
    {"VAR",    3,  DEF_VAR,  nullptr,   4, 1},
    {"CALL",   4,  CALL,     nullptr,   4, 2},
    {"RET",    3,  RET,      nullptr,   3, 3},
    {"D",      1,  DEF,      nullptr,   0, 0},
    {"B",      1,  BLOCK,    nullptr,   0, 0},
    {"OP",     2,  OPERATOR, nullptr,   0, 0},
    {",",      1,  COMMA,    nullptr,   3, 1},
    {"IF",     2,  IF,       nullptr,   2, 2},
    {"C",      1,  LINK,     nullptr,   0, 0},
    {"WH",     1,  WHILE,    nullptr,   2, 1},
    {"ASSN",   4,  ASSIGN,   nullptr,   1, 1},
};

#endif // !MATH_OPERATORS_HPP_