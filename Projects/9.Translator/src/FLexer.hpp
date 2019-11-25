#ifndef FLEXER_HPP_
#define FLEXER_HPP_

#include "tree.h"

template <typename >
class Int
{
    
};

template <class Data>
struct Token
{
    char type;
    Data data;
};

class Lexer 
{
    const char * cur;
    const char * str_begin;

    Node< Token <int> >
};

#endif // !FLEXER_HPP_