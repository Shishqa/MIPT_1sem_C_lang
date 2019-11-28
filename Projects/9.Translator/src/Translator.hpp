#ifndef TRANSLATOR_HPP_
#define TRANSLATOR_HPP_

#include "libraries.hpp"
#include "Parser.hpp"

struct Function
{
    const char * name;

    size_t func_mem;

    const size_t VAR_MAX = 100;
    const char ** local_var;
    
    size_t var_cnt;
};

class Translator
{
    Parser parser;

    BinaryTree<Token> * prog;

    const size_t VAR_MAX = 100;

    size_t var_cnt;
    const char ** global_var;

    const size_t FUNC_MAX = 100;

    size_t func_cnt;
    Function * functions;

    Function * curr_func;

    FILE * out;

    void Init (FILE * f);
    void Proceed ();

    void GetAdd         (Node<Token> * node);
    void GetSub         (Node<Token> * node);
    void GetMul         (Node<Token> * node);
    void GetDiv         (Node<Token> * node);
    void DefFunc        (Node<Token> * node);
    void CallFunc       (Node<Token> * node);
    void GetAssign      (Node<Token> * node);
    void DefVar         (Node<Token> * node);
    void GetIf          (Node<Token> * node);
    void GetWhile       (Node<Token> * node);
    void GetBlocks      (Node<Token> * node);
    void GetOperators   (Node<Token> * node);
    void GetExpression  (Node<Token> * node);

public:
    bool GetAsmCode  (const char * path, const char * output_path = nullptr);
    bool Build       (const char * path); 
    bool BuildAndRun (const char * path);
};

#endif // !TRANSLATOR_HPP_