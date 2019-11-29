#include "libraries.hpp"
#include "Parser.hpp"
#include "Translator.hpp"

#define N node
#define L node->left
#define R node->right

bool Translator::BuildAndRun (const char * path, const char * output_path)
{
    Build (path, output_path);

    char call_cpu[200] = "";

    sprintf (call_cpu, "./bin/cpu %s", output_path);

    system (call_cpu);
}

bool Translator::Build (const char * path, const char * output_path)
{
    const char * tmp_output = "programs/tmp.asm";

    GetAsmCode (path, tmp_output);

    char call_asm[200] = "";

    sprintf (call_asm, "./bin/asm %s %s", tmp_output, output_path);

    system (call_asm);
}

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

    fprintf (out, "; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##\n");
    fprintf (out, "; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##\n");
    fprintf (out, "; ORIGIN :: %s\n\n", path);

    Proceed ();
    
    fclose (f);

    return (true);
}

void Translator::Init (FILE * f)
{
    out = f;

    global_var = (Variable *) calloc (VAR_MAX, sizeof (*global_var));
    var_cnt = 0;

    functions = (Function *) calloc (FUNC_MAX, sizeof (*functions));
    func_cnt = 0;

    curr_func = nullptr;

    case_cnt  = 0;

    if_cnt  = 0;
    curr_if = 0;

    cycle_cnt  = 0;
    curr_cycle = 0;
}

void Translator::Proceed ()
{
    fprintf (out, "\tCALL\tfunc_main\n");
    fprintf (out, "\tMOV\t\t%d\tex\n", VAR_MAX);
    fprintf (out, "\tEND\n\n");
    GetBlocks (prog->root);
}

void Translator::GetBlocks (Node<Token> * node)
{
    if (N->data.type == VOID)
    {
        return;
    }

    GetBlocks (R);

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
    fprintf (out, ";#####################################################################\n", var_cnt);

    curr_func = functions + (func_cnt++);

    curr_func->name      = node->data.lexem;
    curr_func->name_len  = node->data.lexem_len;
    curr_func->local_var = (Variable *) calloc (curr_func->VAR_MAX, sizeof (*curr_func->local_var));
    curr_func->var_cnt   = 0;
    curr_func->VAR_MAX   = 50;
    
    fprintf (out, ";\tfunction \"%s\"\n", curr_func->name);
    fprintf (out, "func_%s:\n", curr_func->name);

    Node<Token> * curr_arg = node->left;

    while (curr_arg->data.type != VOID)
    {
        if (curr_arg->left->data.type != DEF_VAR)
        {
            printf ("variables should be defined");
            return;
        }

        curr_func->local_var[curr_func->var_cnt].id = curr_arg->left->left->data.lexem;
        curr_func->local_var[curr_func->var_cnt].len = curr_arg->left->left->data.lexem_len;

        fprintf (out, ";\t%s_var_%s (arg_%lu)\n", curr_func->name, curr_func->local_var[curr_func->var_cnt].id, curr_func->var_cnt);
        fprintf (out, "\tPOP [ex+%lu]\n", var_cnt);

        curr_func->var_cnt++;

        curr_arg = curr_arg->right;
    }

    fprintf (out, ";\t\"%s\" body:\n\n", curr_func->name);
    GetOperators (node->right);

    fprintf (out, ";#####################################################################\n\n\n", var_cnt);

    curr_func = nullptr;
}

void Translator::GetOperators (Node<Token> * node)
{
    if (node->data.type == VOID)
    {
        return;
    }

    GetOperators (node->right);

    if (L->data.type == COND)
    {
        size_t remember_curr_if = curr_if;
        curr_if = (if_cnt++);

        fprintf (out, "\tif_%lu:\n", curr_if);
        GetIf (L);
        fprintf (out, "\tif_%lu_end:\n\n", curr_if);

        curr_if = remember_curr_if;
    }
    else if (L->data.type == ASSIGN)
    {
        Calculate (L->right);

        if (L->left->data.type == DEF_VAR)
        {
            DefVar (L->left);
            fprintf (out, "\tPOP\t[ex+%d]\n", curr_func->var_cnt - 1);
            return;
        }
        else
        {
            int id = FindVar (L->data.lexem, L->data.lexem_len);

            if (id != -1)
            {
                fprintf (out, "\tPOP\t[ex+%d]\n", id);
            }
        }
    }
    else if (L->data.type == FUNC)
    {
        //fprintf (out, ";\tCALL %s\n", L->left->data.lexem);
        GetCall (L);
    }
}

void Translator::DefVar (Node<Token> * node)
{
    Variable * curr_var = curr_func->local_var + (curr_func->var_cnt++);

    curr_var->id = node->left->data.lexem;
    curr_var->len = node->left->data.lexem_len;
}

void Translator::GetCall (Node<Token> * node)
{
    int call_func = FindFunc (L->data.lexem, L->data.lexem_len);

    size_t arg_cnt = 0;
    Node<Token> * curr_arg = R;

    while (curr_arg->data.type != VOID)
    {
        arg_cnt++;

        fprintf (out, "; pushing argument before call %s\n", L->data.lexem);
        Calculate (curr_arg->left);

        curr_arg = curr_arg->right;
    }

    if (call_func != -1)
    {
        fprintf (out, "\tMOV\tex+%lu\tex\n", functions[call_func].VAR_MAX);
        fprintf (out, "\tCALL\tfunc_%s\n", functions[call_func].name);
        return;
    }

    if (L->data.lexem_len == 6 &&!strncmp (L->data.lexem, "return", L->data.lexem_len))
    {
        fprintf (out, "\tPUSH\tex\n");
        fprintf (out, "\tPUSH\t%d\n", curr_func->VAR_MAX);
        fprintf (out, "\tSUB\n");
        fprintf (out, "\tPOP\tex\n");
        fprintf (out, "\tRET\n");
        return;
    }

    if (L->data.lexem_len == 5 &&!strncmp (L->data.lexem, "print", L->data.lexem_len))
    {
        for (size_t i = 0; i < arg_cnt; i++)
        {
            fprintf (out, "\tOUT\n");
        }
        return;
    }

    if (L->data.lexem_len == 5 &&!strncmp (L->data.lexem, "input", L->data.lexem_len))
    {
        fprintf (out, "\tIN\n");
        return;
    }

    fprintf (out, ";\t gonna call %s\n", L->data.lexem);
}

void Translator::GetIf (Node<Token> * node)
{
    if (N->data.type == VOID)
    {
        return;
    }

    GetIf (R);

    size_t curr_case = case_cnt;
    case_cnt++;

    if (L->data.type == IF)
    {
        fprintf (out, ";\tcase_%lu_condition\n", curr_case);
        Calculate (L->left);
        fprintf (out, "\t\tPUSH\t0\n");
        fprintf (out, "\t\tJNE\tcase_%lu_operators\n", curr_case);
        fprintf (out, "\t\tJMP\tcase_%lu_next\n", curr_case);
        fprintf (out, "\tcase_%lu_operators:\n\n", curr_case);
        GetOperators (L->right);
        fprintf (out, "\n\t\tJMP\tif_%lu_end\n", curr_if);
        fprintf (out, "\tcase_%lu_next:\n\n", curr_case);
    }
    else if (L->data.type == OPERATOR)
    {
        GetOperators (L);
    }
}

// void Translator::GetWhile (Node<Token> * node)
// {

// }

#define GET_BOOL( asm_jmp )                                                                                   \
    {                                                                                                         \
        fprintf (out, "\t%s case_%lu_positive\n", asm_jmp, case_cnt);                                         \
        fprintf (out, "\tJMP case_%lu_negative\n", case_cnt);                                                 \
        fprintf (out, "\tcase_%lu_positive:\n\t\tPUSH\t1\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_negative:\n\t\tPUSH\t0\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

#define GET_AND()                                                                                             \
    {                                                                                                         \
        fprintf (out, "\tcase_%lu_check1:\n");                                                                \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_negative\n");                                                         \
        fprintf (out, "\t\tJMP\tcase_%lu_check2\n");                                                          \
        fprintf (out, "\tcase_%lu_check2:\n");                                                                \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_negative\n");                                                         \
        fprintf (out, "\t\tJMP\tcase_%lu_positive\n");                                                        \
        fprintf (out, "\tcase_%lu_positive:\n\t\tPUSH\t1\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_negative:\n\t\tPUSH\t0\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

#define GET_OR()                                                                                              \
    {                                                                                                         \
        fprintf (out, "\tcase_%lu_check1:\n");                                                                \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_check2\n");                                                           \
        fprintf (out, "\t\tJMP\tcase_%lu_positive\n");                                                        \
        fprintf (out, "\tcase_%lu_check2:\n");                                                                \
        fprintf (out, "\t\tPUSH\t0\n");                                                                       \
        fprintf (out, "\t\tJE\tcase_%lu_negative\n");                                                         \
        fprintf (out, "\t\tJMP\tcase_%lu_positive\n");                                                        \
        fprintf (out, "\tcase_%lu_positive:\n\t\tPUSH\t1\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_negative:\n\t\tPUSH\t0\n\t\tJMP\tcase_%lu_continue\n", case_cnt, case_cnt); \
        fprintf (out, "\tcase_%lu_continue:\n", case_cnt);                                                    \
        case_cnt++;                                                                                           \
    }

void Translator::Calculate (Node<Token> * node)
{
    if (L && N->data.type != FUNC)
    {
        Calculate (L);
    }

    if (R && N->data.type != FUNC)
    {
        Calculate (R);
    }

    switch (node->data.type)
    {
        case ADD:
            fprintf (out, "\t\tADD\n");
        break;

        case SUB:
            fprintf (out, "\t\tSUB\n");
        break;

        case MUL:
            fprintf (out, "\t\tMUL\n");
        break;

        case DIV:
            fprintf (out, "\t\tDIV\n");
        break;
        
        case AND:
            GET_AND ();
        break;

        case OR:
            GET_OR ();
        break;

        case NUM:
            fprintf (out, "\t\tPUSH\t%s\n", N->data.lexem);
        break;

        case LESS:
            GET_BOOL ("JB");
        break;

        case GREATER:
            GET_BOOL ("JA");
        break;

        case LEQ:
            GET_BOOL ("JBE");
        break;

        case GEQ:
            GET_BOOL ("JAE");
        break;

        case EQ:
            GET_BOOL ("JE");
        break;

        case NEQ:
            GET_BOOL ("JNE");
        break;

        case ID:
            fprintf (out, "\t\tPUSH\t[ex+%d]\n", FindVar (N->data.lexem, N->data.lexem_len));
        break;

        case FUNC:
            //fprintf (out, ";\t\tCALL %s\n", N->left->data.lexem);
            GetCall (node);
        break;
    
        default:
            fprintf (out, "\t\tPUSH\t%s\n", node->data.lexem);
        break;
    }
}

int Translator::FindVar (const char * var_id,  const size_t len)
{
    for (int i = 0; i < curr_func->var_cnt; i++)
    {
        if (len == curr_func->local_var[i].len &&
            !strncmp (var_id, curr_func->local_var[i].id, len))
        {
            return (i);
        }
    }

    return (-1);
}

int Translator::FindFunc (const char * func_id, const size_t len)
{
    for (int i = 0; i < func_cnt; i++)
    {
        if (len == functions[i].name_len &&
            !strncmp (func_id, functions[i].name, len))
        {
            return (i);
        }
    }

    return (-1);
}