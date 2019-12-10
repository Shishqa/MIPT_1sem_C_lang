#ifndef TOKEN_HPP_
#define TOKEN_HPP_

struct Token
{
    char    type;
    int     data;
    const char * name;

    //Удобно носить с собой ссылку на функцию, если type == OP_TYPE
    //Аналогично удобно будет носить с собой ссылку на переменную в дальнейшем
};

enum types
{
    OP_TYPE,
    MATH_TYPE,
    NUM_TYPE,
    ID_TYPE
};

#endif // !TOKEN_HPP_