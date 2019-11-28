#include "libraries.hpp"
#include "Parser.hpp"
#include "Translator.hpp"

#define N node
#define L node->left
#define R node->right

bool Translator::GetAsmCode (const char * path, const char * output_path)
{
    prog = parser.ParseFile (path);

    if (!prog)
    {
        return (false);
    }

    prog->dotDump (PrintToken, 2);

    FILE * f = fopen (output_path, "w");

    Init (f);

    Proceed ();
    
    fclose (f);

    return (true);
}

void Translator::Init (FILE * f)
{
    out = f;

    global_var = (const char **) calloc (VAR_MAX, sizeof (*global_var));
    var_cnt = 0;

    functions = (Function *) calloc (FUNC_MAX, sizeof (*functions));
    func_cnt = 0;
}

void Translator::Proceed ()
{
    fprintf (out, "\tCALL\tfunc_main\n\tEND\n\n");

    GetBlocks (prog->root);
}

void Translator::GetBlocks (Node<Token> * node)
{
    if (R)
    {
        GetBlocks (R);
    }

    if (L->data.type == DEF_FUNC)
    {
        DefFunc (L->left);
    }
    else if (L->data.type == ASSIGN)
    {
        //GetAssign (L);
    }
    else if (L->data.type == DEF_VAR)
    {
        //DefVar (L->left);
    }
}

void Translator::DefFunc (Node<Token> * node)
{
    curr_func = functions + (func_cnt++);

    curr_func->name = node->data.lexem;
    
    fprintf (out, "func_%s:\n", curr_func->name);

    if (node->left->data.type != VOID)
    {
        // PARSE ARGS
    }

    // Перед вызовом функции делаем REG += VAR_MAX(для функции)

    // регистр для смещения в RAM для конкретного вызова функции
    // пушим все аргументы из стека в RAM, работаем с ними
    // перед выходом из функции возвращаем смещение RAM обратно, в RAM остается "мусор"
}
