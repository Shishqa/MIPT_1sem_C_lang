#ifndef MONOMIAL_HPP_
#define MONOMIAL_HPP_

struct Monomial
{
    char    type;
    double  data;
    int     opcode;

    //Удобно носить с собой ссылку на функцию, если type == OP_TYPE
    //Аналогично удобно будет носить с собой ссылку на переменную в дальнейшем
};

enum types
{
    OP_TYPE,
    NUM_TYPE,
    VAR_TYPE
};

void PrintMonomial (FILE * f, const void * ptr);

#endif // !MONOMIAL_HPP_