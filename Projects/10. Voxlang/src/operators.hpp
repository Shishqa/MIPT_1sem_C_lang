#ifndef OPERATORS_HPP_
#define OPERATORS_HPP_

#include "head.hpp"

struct Operator
{
    const char * lexem;
    size_t min_d;
    size_t max_d;
};

const size_t OP_CNT = 30;

Operator operators[OP_CNT] = 
{
    {"var",     1, 3},
    {"def",     3, 3},
    {",",       2, 4},
    {"id",      4, 4},
    {"if",      2, 5},
    {"while",   5, 5},
    {"=",       1, 4},
    {"call",    1, 5},
    {"ret",     3, 6},
    {"==",      4, 9},
    {"!=",      3, 9},
    {"<=",      2, 9},
    {">=",      1, 9},
    {"<",       1, 8},
    {">",       2, 8},
    {"&",       3, 8},
    {"|",       4, 8},
    {"+",       1, 7},
    {"-",       2, 7},
    {"*",       3, 7},
    {"/",       1, 6},
    {"%",       2, 6},

    {"(",       1, 2},
    {")",       2, 2}
};


#endif // !OPERATORS_HPP_