#ifndef OPERATIONS_HPP_
#define OPERATIONS_HPP_

enum operations
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