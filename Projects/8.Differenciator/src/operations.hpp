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
    SIN,
    COS,
    TG,
    CTG,
    LN,
    EXP,
    ARCSIN,
    ARCCOS,
    ARCTG,
    ARCCTG,
    SH,
    CH,
    TH,
    CTH,
    ABS,
    SIGN,
    DIV,
    POW,
    DIFF
};

static const Operation operations[OP_CNT] = {
    {"@",      1, UNDEF,  nullptr, 0},
    {"+",      1, ADD,    nullptr, 1},
    {"-",      1, SUB,    nullptr, 1},
    {"*",      1, MUL,    nullptr, 1},
    {"/",      1, DIV,    nullptr, 1},
    {"^",      1, POW,    nullptr, 1},
    {"abs",    3, ABS,    nullptr, 1},
    {"sign",   4, SIGN,   nullptr, 1},
    {"ln",     2, LN,     nullptr, 1},
    {"exp",    3, EXP,    nullptr, 1},
    {"sin",    3, SIN,    nullptr, 1},
    {"cos",    3, COS,    nullptr, 1},
    {"tg",     2, TG,     nullptr, 1},
    {"ctg",    3, CTG,    nullptr, 1},
    {"arcsin", 6, ARCSIN, nullptr, 1},
    {"arccos", 6, ARCCOS, nullptr, 1},
    {"arctg",  5, ARCTG,  nullptr, 1},
    {"arcctg", 6, ARCCTG, nullptr, 1},
    {"sh",     2, SH,     nullptr, 1},
    {"ch",     2, CH,     nullptr, 1},
    {"th",     2, TH,     nullptr, 1},
    {"cth",    3, CTH,    nullptr, 1},
    {"d",      1, DIFF,   nullptr, 1}
};

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


#endif // !OPERATIONS_HPP_