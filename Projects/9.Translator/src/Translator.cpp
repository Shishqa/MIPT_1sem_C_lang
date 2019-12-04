#include "libraries.hpp"
#include "Parser.hpp"
#include "Translator.hpp"

#define N node
#define L node->left
#define R node->right

#define STOP( err_code )                    \
    {                                       \
        error = err_code;                   \
        return;                             \
    }                                   

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
    var_cnt = 1;

    functions = (Function *) calloc (FUNC_MAX, sizeof (*functions));
    func_cnt = 1;

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
    PRINT ("blocks\n");

    if (!N)
    {
        return;
    }

    GetBlocks (L);

    if (R->data.type == DEF_FUNC)
    {
        DefFunc (R);
    }
    else if (R->data.type == DEF_VAR)
    {
        DefVar (R);
    }
}

void Translator::DefFunc (Node<Token> * node)
{
    PRINT ("func\n");

    fprintf (out, ";#####################################################################\n", var_cnt);

    int check = FindFunc (R->data.lexem, R->data.lexem_len);

    if (check != 0)
    {
        return;
    }

    curr_func = functions + (func_cnt++);

    curr_func->name      = R->data.lexem;
    curr_func->name_len  = R->data.lexem_len;
    curr_func->VAR_MAX   = 50;
    curr_func->local_var = (Variable *) calloc (curr_func->VAR_MAX, sizeof (*curr_func->local_var));
    curr_func->var_cnt   = 1;
    
    fprintf (out, ";\tfunction \"%s\"\n", curr_func->name);
    fprintf (out, "func_%s:\n", curr_func->name);

    Node<Token> * curr_arg = L;

    while (curr_arg != nullptr)
    {
        if (curr_arg->right->data.type != DEF_VAR)
        {
            printf ("variables should be defined");
            return;
        }

        curr_func->local_var[curr_func->var_cnt].id = curr_arg->right->left->data.lexem;
        curr_func->local_var[curr_func->var_cnt].len = curr_arg->right->left->data.lexem_len;

        fprintf (out, ";\t%s_var_%s (arg_%lu)\n", curr_func->name, curr_func->local_var[curr_func->var_cnt].id, curr_func->var_cnt);
        fprintf (out, "\tPOP [ex+%lu]\n", var_cnt);

        curr_func->var_cnt++;

        curr_arg = curr_arg->left;
    }

    fprintf (out, ";\t\"%s\" body:\n\n", curr_func->name);
    GetOperators (R->right);

    fprintf (out, ";#####################################################################\n\n\n", var_cnt);

    curr_func = nullptr;
}

void Translator::GetOperators (Node<Token> * node)
{
    PRINT ("operator\n");

    if (!N)
    {
        return;
    }

    if (N->data.type == BLOCK)
    {
        GetOperators (R);
        return;
    }

    GetOperators (L);

    if (R->data.type == IF)
    {
        GetIf (R);
    }
    else if (R->data.type == WHILE)
    {
        GetWhile (R);
    }
    else if (R->data.type == DEF_VAR)
    {
        DefVar (R);
    }
    else if (R->data.type == ASSIGN)
    {
        GetAssign (R);
    }
    else if (R->data.type == CALL)
    {
        GetCall (R);
    }
}

void Translator::GetAssign (Node<Token> * node)
{
    PRINT ("assn\n");

    if (L->data.type != ID)
    {
        return;
    }

    Calculate (R);

    int id = FindVar (L->data.lexem, L->data.lexem_len);

    if (id != 0)
    {
        if (id > 0)
        {
            fprintf (out, "\tPOP\t[ex+%d]\n", id);
        }
        else
        {
            fprintf (out, "\tPOP\t[%d]\n", -id);
        }
    }
    else
    {
        printf ("UNDEFINED %s\n\n", L->data.lexem);
    }
}

void Translator::DefVar (Node<Token> * node)
{
    PRINT ("var\n");

    int check = FindVar (node->data.lexem, node->data.lexem_len);

    PRINT ("FOUND\n");

    Variable * curr_var = nullptr;

    if (check)
    {
        return;
    }

    if (curr_func)
    {
        curr_var = curr_func->local_var + (curr_func->var_cnt++);
    }
    else
    {
        curr_var = global_var + (var_cnt++);
    }

    curr_var->id  = L->data.lexem;
    curr_var->len = L->data.lexem_len;

    if (R)
    {
        GetAssign (N);
    }
}

void Translator::GetCall (Node<Token> * node)
{
    PRINT ("call\n");

    int call_func = FindFunc (L->data.lexem, L->data.lexem_len);

    size_t arg_cnt = 0;
    Node<Token> * curr_arg = R;

    while (curr_arg)
    {
        arg_cnt++;

        fprintf (out, "; pushing argument before call %s\n", L->data.lexem);
        Calculate (curr_arg->right);

        curr_arg = curr_arg->left;
    }

    if (call_func != 0)
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

    if (L->data.lexem_len == 4 &&!strncmp (L->data.lexem, "putc", L->data.lexem_len))
    {
        fprintf (out, "\tOUTC\n");
        return;
    }

    fprintf (out, ";\t gonna call %s\n", L->data.lexem);
}

void Translator::GetIf (Node<Token> * node)
{
    PRINT ("if\n");

    size_t remember_curr_if = curr_if;
    curr_if = (if_cnt++);

    fprintf (out, "\tif_%lu:\n", curr_if);
    fprintf (out, ";\tif_%lu_condition\n", curr_if);

    Calculate (N->left);

    fprintf (out, "\t\tPUSH\t0\n");
    fprintf (out, "\t\tJNE\tif_%lu_positive\n", curr_if);
    fprintf (out, "\t\tJMP\tif_%lu_negative\n", curr_if);

    Node<Token> * tmp = R;

    fprintf (out, "\tif_%lu_positive:\n\n", curr_if);

    GetOperators (R->right);

    fprintf (out, "\n\t\tJMP\tif_%lu_end\n", curr_if);
    fprintf (out, "\tif_%lu_negative:\n\n", curr_if);

    GetOperators (R->left);

    fprintf (out, "\n\tif_%lu_end:\n", curr_if);

    curr_if = remember_curr_if;
}

void Translator::GetWhile (Node<Token> * node)
{
    PRINT ("while\n");

    size_t remember_cycle = curr_cycle;

    curr_cycle = (cycle_cnt++);

    fprintf (out, "cycle_%lu:\n", curr_cycle);

    Calculate (L);

    fprintf (out, "\tPUSH\t0\n");
    fprintf (out, "\tJE cycle_%lu_stop\n", curr_cycle);
    
    GetOperators (R);

    fprintf (out, "\tJMP cycle_%lu\n", curr_cycle);
    fprintf (out, "cycle_%lu_stop:\n", curr_cycle);

    curr_cycle = remember_cycle;
}

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
    PRINT ("calculate\n");

    if (L && N->data.type != CALL)
    {
        Calculate (L);
    }

    if (R && N->data.type != CALL)
    {
        Calculate (R);
    }

    int tmp = 0;

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

        case MOD:
            fprintf (out, "\t\tMOD\n");
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
            tmp = FindVar (N->data.lexem, N->data.lexem_len);
            if (tmp > 0)
            {
                fprintf (out, "\t\tPUSH\t[ex+%d]\n", tmp);
            }
            else
            {
                fprintf (out, "\t\tPUSH\t[%d]\n", -tmp);
            }
        break;

        case CHAR:
            fprintf (out, "\t\tPUSH\t%d\n", *(N->data.lexem));
        break;

        case STRING:
            for (size_t i = node->data.lexem_len - 1; i >= 0; i--)
            {
                fprintf (out, "\t\tPUSH\t%d", *(N->data.lexem + i));
            }
        break;

        case CALL:
            GetCall (node);
        break;
    
        default:
            fprintf (out, "\t\tPUSH\t%s\n", node->data.lexem);
        break;
    }
}

int Translator::FindVar (const char * var_id,  const size_t len)
{
    PRINT ("find var\n");

    if (curr_func)
    {
        for (int i = 1; i < curr_func->var_cnt; i++)
        {
            if (len == curr_func->local_var[i].len &&
                !strncmp (var_id, curr_func->local_var[i].id, len))
            {
                return (i);
            }
        }
    }

    for (int i = 1; i < var_cnt; i++)
    {
        if (len == global_var[i].len &&
            !strncmp (var_id, global_var[i].id, len))
        {
            return (-i);
        }
    }

    return (0);
}

int Translator::FindFunc (const char * func_id, const size_t len)
{
    PRINT ("find func\n");

    for (int i = 1; i < func_cnt; i++)
    {
        if (len == functions[i].name_len &&
            !strncmp (func_id, functions[i].name, len))
        {
            PRINT ("FOUND\n");
            return (i);
        }
    }

    PRINT ("NOT FOUND\n");
    return (0);
}