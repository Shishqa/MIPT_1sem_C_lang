#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_

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
    ADD,
    SUB,
    MUL,
    ABS,
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
    CTH,
    DIV,
    POW,
    DIFF
};

static const Operation operations[OP_CNT] = {
    {"@",      1, UNDEF,  nullptr, 0},
    {"+",      1, ADD,    nullptr, 1},
    {"-",      1, SUB,    nullptr, 1},
    {"*",      1, MUL,    nullptr, 2},
    {"abs",    3, ABS,    nullptr, 0},
    {"sign",   4, SIGN,   nullptr, 3},
    {"ln",     2, LN,     nullptr, 3},
    {"exp",    3, EXP,    nullptr, 3},
    {"sin",    3, SIN,    nullptr, 3},
    {"cos",    3, COS,    nullptr, 3},
    {"tg",     2, TG,     nullptr, 3},
    {"ctg",    3, CTG,    nullptr, 3},
    {"arcsin", 6, ARCSIN, nullptr, 3},
    {"arccos", 6, ARCCOS, nullptr, 3},
    {"arctg",  5, ARCTG,  nullptr, 3},
    {"arcctg", 6, ARCCTG, nullptr, 3},
    {"sh",     2, SH,     nullptr, 3},
    {"ch",     2, CH,     nullptr, 3},
    {"th",     2, TH,     nullptr, 3},
    {"cth",    3, CTH,    nullptr, 3},
    {"/",      1, DIV,    nullptr, 4},
    {"^",      1, POW,    nullptr, 4},
    {"d",      1, DIFF,   nullptr, 4}
};

/*
static const char * op_names[23] = {
        "@",
        "+",
        "-",
        "*",
        "sin",
        "cos",
        "tg",
        "ctg",
        "ln",
        "exp",
        "arcsin",
        "arccos",
        "arctg",
        "arcctg",
        "sh",
        "ch",
        "th",
        "cth",
        "abs",
        "sign",
        "/",
        "^",
        "d"
};
*/


#endif // !OPERATIONS_HPP_