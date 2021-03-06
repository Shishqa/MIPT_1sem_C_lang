#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_

#include "math.h"

static double my_add (const double l, const double r)
{
    return (l + r);
}

static double my_mul (const double l, const double r)
{
    return (l * r);
}

static double my_sub (const double l, const double r)
{
    return (l - r);
}

static double my_div (const double l, const double r)
{
    return (l / r);
}

static double my_sign (const double l, const double r)
{
    if (r > 0)
    {
        return (1);
    }
    if (r < 0)
    {
        return (-1);
    }
    return (0);
}

static double my_abs (const double l, const double r)
{
    return (fabs (r));
}

static double my_ln (const double l, const double r)
{
    return (log (r));
}

static double my_sin (const double l, const double r)
{
    return (sin (r));
}

static double my_cos (const double l, const double r)
{
    return (cos (r));
}


const size_t MAX_OP_NAME_LEN = 6 + 1;
const size_t OP_CNT = 23;

struct Operation 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    double (* op) (const double, const double);

    size_t priority;
};

enum op_enum
{
    UNDEF,
    DIFF,
    ABS,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SIGN,
    LN,
    EXP,
    SIN,
    COS,
    TG,
    CTG,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,
    SH,
    CH,
    TH,
    CTH
};

static const Operation operations[OP_CNT] = {
    {"@",      1, UNDEF,  nullptr, 0},
    {"d",      1, DIFF,   nullptr, 0},
    {"abs",    3, ABS,    my_abs,  4},
    {"+",      1, ADD,    my_add,  1},
    {"-",      1, SUB,    my_sub,  1},
    {"*",      1, MUL,    my_mul,  3},
    {"/",      1, DIV,    my_div,  3},
    {"^",      1, POW,    nullptr, 3},
    {"sign",   4, SIGN,   my_sign, 3},
    {"ln",     2, LN,     my_ln,   3},
    {"exp",    3, EXP,    nullptr, 3},
    {"sin",    3, SIN,    my_sin,  3},
    {"cos",    3, COS,    my_cos,  3},
    {"tg",     2, TG,     nullptr, 3},
    {"ctg",    3, CTG,    nullptr, 3},
    {"arcsin", 6, ARCSIN, nullptr, 3},
    {"arccos", 6, ARCCOS, nullptr, 3},
    {"arctg",  5, ARCTG,  nullptr, 3},
    {"arcctg", 6, ARCCTG, nullptr, 3},
    {"sh",     2, SH,     nullptr, 3},
    {"ch",     2, CH,     nullptr, 3},
    {"th",     2, TH,     nullptr, 3},
    {"cth",    3, CTH,    nullptr, 3}
};

#endif // !OPERATIONS_HPP_