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

// static void latex_add   (Node<Token> * node, FILE * f);
// static void latex_sub   (Node<Token> * node, FILE * f);
// static void latex_mul   (Node<Token> * node, FILE * f);
// static void latex_div   (Node<Token> * node, FILE * f);
// static void latex_pow   (Node<Token> * node, FILE * f);
// static void latex_abs   (Node<Token> * node, FILE * f);
// static void latex_sign  (Node<Token> * node, FILE * f);
// static void latex_ln    (Node<Token> * node, FILE * f);
// static void latex_exp   (Node<Token> * node, FILE * f);
// static void latex_sin   (Node<Token> * node, FILE * f);
// static void latex_cos   (Node<Token> * node, FILE * f);
// static void latex_tg    (Node<Token> * node, FILE * f);
// static void latex_ctg   (Node<Token> * node, FILE * f);
// static void latex_arcsin (Node<Token> * node, FILE * f);
// static void latex_arccos (Node<Token> * node, FILE * f);
// static void latex_arctg  (Node<Token> * node, FILE * f);
// static void latex_arcctg (Node<Token> * node, FILE * f);
// static void latex_sh    (Node<Token> * node, FILE * f);
// static void latex_ch    (Node<Token> * node, FILE * f);
// static void latex_th    (Node<Token> * node, FILE * f);
// static void latex_cth   (Node<Token> * node, FILE * f);


const size_t MAX_OP_NAME_LEN = 6 + 1;
const size_t OP_CNT = 23;
struct Operation 
{
    char name[MAX_OP_NAME_LEN];
    size_t name_len;

    size_t opcode;
    double (* op) (const double, const double);

    size_t priority;

    //void (* printDiff) (Node<Token> *, FILE * f);
    //Node<Token> * (* getDiff) (Node<Token> *);
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
    {"abs",    3, ABS,    nullptr, 4},
    {"+",      1, ADD,    my_add,  1},
    {"-",      1, SUB,    my_sub,  1},
    {"*",      1, MUL,    my_mul,  3},
    {"/",      1, DIV,    my_div,  3},
    {"^",      1, POW,    nullptr, 3},
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
    {"cth",    3, CTH,    nullptr, 3}
};

#endif // !OPERATIONS_HPP_