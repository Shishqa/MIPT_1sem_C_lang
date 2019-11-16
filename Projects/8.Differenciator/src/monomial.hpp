#ifndef MONOMIAL_HPP_
#define MONOMIAL_HPP_

struct Monomial
{
    char type;
    int  data;
};

enum types
{
    OP_TYPE,
    NUM_TYPE,
    VAR_TYPE
};

void PrintMonomial (FILE * f, const void * ptr);

#endif // !MONOMIAL_HPP_