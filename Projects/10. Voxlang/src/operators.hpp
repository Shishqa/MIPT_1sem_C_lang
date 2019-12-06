#ifndef OPERATORS_HPP_
#define OPERATORS_HPP_

#include "head.hpp"

struct Operator
{
    size_t opcode;

    const char * lexem;
    size_t max_d;
    size_t min_d;
};

const size_t OP_CNT = 30;

const size_t END = 50;

static Operator operators[OP_CNT] = {
    {LEFT,      "(",       2, 1},
    {RIGHT,     ")",       2, 2},
    {IF,        "if",      3, 1},                                  
    {WHILE,     "while",   3, 3},                                  
    {DEF_VAR,   "var",     4, 4},
    {ID,        "id",      4, 2},                                  
    {CALL,      "call",    4, 1},                                  
    {RET,       "ret",     5, 1},                                  
    {COMMA,     ",",       5, 2},                                  
    {ASSIGN,    "=",       5, 5},                                      
    {EQ,        "==",      9, 4},                                      
    {NEQ,       "!=",      9, 3},                                      
    {LEQ,       "<=",      9, 2},                                          
    {GEQ,       ">=",      9, 1},                                          
    {LESS,      "<",       8, 8},                                      
    {GREATER,   ">",       8, 1},
    {AND,       "&",       8, 2},
    {OR,        "|",       8, 3},
    {ADD,       "+",       8, 4},
    {SUB,       "-",       7, 7},
    {MUL,       "*",       7, 1},
    {DIV,       "/",       7, 2},
    {MOD,       "%",       7, 3}
};

//    пустая строка - ноль  
//    @ - цифра [1-9], если 
//                          
//    2 - 1, 2              
//    3 - 1, 3              
//    4 - 1, 2, 4           
//    5 - 1, 2, 5           
//    6 - 1, 2, 3, 6        
//    7 - 1, 2, 3, 7        
//    8 - 1, 2, 3, 4, 8     
//    9 - 1, 2, 3, 4, 9     
//                         
//      99, 6* 

#endif // !OPERATORS_HPP_