#ifndef OPERATORS_HPP_
#define OPERATORS_HPP_

#include "head.hpp"

struct Operator
{
    size_t opcode;

    const char * lexem;
    size_t lexem_len;

    size_t max_d;
    size_t min_d;
};

const size_t OP_CNT = 24;

const size_t END = 50;

static Operator operators[OP_CNT] = {
    {END,       "X", 1,      0, 0},
    {ID,        "id", 2,     1, 1},                                  
    {LEFT,      "(", 1,      2, 1},
    {RIGHT,     ")", 1,      2, 2},
    {IF,        "if", 2,     3, 1},                                  
    {WHILE,     "while", 5,  3, 3},                                  
    {DEF_VAR,   "var", 3,    4, 4},
    {CALL,      "call", 4,   4, 1},                                  
    {RET,       "ret", 3,    5, 1},                                  
    {COMMA,     ",", 1,      5, 2},                                  
    {ASSIGN,    "=", 1,      5, 5},                                      
    {EQ,        "==", 2,     8, 8},                                      
    {NEQ,       "!=", 2,     8, 4},                                      
    {LEQ,       "<=", 2,     8, 3},                                          
    {GEQ,       ">=", 2,     8, 2},                                          
    {LESS,      "<", 1,      8, 1},                                      
    {GREATER,   ">", 1,      7, 7},
    {AND,       "&", 1,      7, 3},
    {OR,        "|", 1,      7, 2},
    {ADD,       "+", 1,      7, 1},
    {SUB,       "-", 1,      6, 6},
    {MUL,       "*", 1,      6, 1},
    {DIV,       "/", 1,      6, 2},
    {MOD,       "%", 1,      6, 3}
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
//                         
//      99, 6* 

#endif // !OPERATORS_HPP_