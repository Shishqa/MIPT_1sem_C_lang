#ifndef COMPILER_HPP_
#define COMPILER_HPP_

#include "Parser.hpp"

class Compiler
{
    Parser parser;

public:
    bool GetAsmCode  (const char * path);
    bool Build       (const char * path); 
    bool BuildAndRun (const char * path);
};

#endif // !COMPILER_HPP_