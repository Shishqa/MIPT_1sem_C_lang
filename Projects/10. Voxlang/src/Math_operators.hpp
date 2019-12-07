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

const size_t MAX_OP_NAME_LEN = 2;
const size_t OP_CNT = 6;

struct Math_Operation 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    int (* op) (const int, const int);

    size_t priority;
};

enum math_op_enum
{
    UNDEF,
    DIFF,
    ADD,
    SUB,
    MUL,
    DIV
};

static const Math_Operation math_op[OP_CNT] = {
    {"@",      1, UNDEF,  nullptr, 0},
    {"d",      1, DIFF,   nullptr, 0},
    {"+",      1, ADD,    my_add,  1},
    {"-",      1, SUB,    my_sub,  1},
    {"*",      1, MUL,    my_mul,  3},
    {"/",      1, DIV,    my_div,  3}
};

#endif // !MATH_OPERATORS_HPP_