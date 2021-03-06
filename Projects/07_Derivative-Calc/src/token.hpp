#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include "operations.hpp"

struct Token
{
    char    type;
    double  data;
    int     op_id;

    char    var;

    //Удобно носить с собой ссылку на функцию, если type == OP_TYPE
    //Аналогично удобно будет носить с собой ссылку на переменную в дальнейшем
};

enum types
{
    OP_TYPE,
    NUM_TYPE,
    VAR_TYPE
};

void PrintToken (FILE * f, const void * ptr);

#endif // !TOKEN_HPP_