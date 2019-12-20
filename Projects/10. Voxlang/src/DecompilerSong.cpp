#include "libraries.hpp"
#include "DecompilerSong.hpp"
#include "Operators.hpp"

#define N node
#define L node->left
#define R node->right

#define STOP( err_code )                    \
    {                                       \
        error = err_code;                   \
        return;                             \
    }                                   

bool SongDecompiler::GetCode (BinaryTree<Token> * code, const char * output_path, const char * harmony)
{
    if (!code)
    {
        printf ("Decompilation failed!\n");
        return (false);
    }

    h.init (harmony);
    if (!h.is_valid)
    {
        printf ("Decompilation failed!\n");
        return (false);
    }

    for (size_t i = 0; i < h.chord_cnt; i++)
    {
        printf ("chord with tonic %s\n", basic_notes[h.chords[i].tonic]);
        for (size_t j = 0; j < h.chords[i].num_of_notes; j++)
        {
            printf ("%s ", basic_notes[h.chords[i].notes[j]]);
        }
        printf ("\n");
    }

    FILE * f = fopen (output_path, "w");

    fprintf (f, "%% ##      THIS FILE IS GENERATED AUTOMATICALLY      ##\n");
    fprintf (f, "%% ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##\n");

    prog = code;

    Proceed (f);

    fclose (f);

    printf ("Decompiled!\n");
    return (true);
}

void SongDecompiler::Proceed (FILE * f)
{
    out = f;

    fprintf (out, "X:1\nM:4/4\nL:1/16\nQ:1/4=100\nK:C\n");

    Calculate (prog->root);
}

void SongDecompiler::Calculate (Node<Token> * node)
{
    if (!node)
    {
        return;
    }

    switch (node->data.type)
    {
        case NUM_TYPE:
            printf ("num %d\n", node->data.data);
            GetNum (node);
        break;

        case ID_TYPE:
            printf ("id %s\n", node->data.name);
            GetId (node);
        break;
    
        default:
            printf ("op %s\n", operators[node->data.data].name);
            GetOp (node);
        break;
    }
}

#define NOTE( i )                   \
    basic_notes[h.chords[h.curr_chord].notes[i]]

#define NEXT()                                                                                  \
    {                                                                                           \
        h.next();                                                                               \
        fprintf (out, "%s", NOTE(0));    \
    }                                   

void SongDecompiler::GetNum (Node<Token> * node)
{
    int data = node->data.data;
    NEXT ();

    if (data < 0)
    {
        fprintf (out, "2%s", NOTE (0));
    }
    else
    {
        fprintf (out, "2%s", NOTE (1));
    }

    data = abs (data);

    while (data)
    {
        fprintf (out, "%s", ((data % 3) ? "z" : NOTE (data % 3 - 1)));
        data /= 3;
    }

    fprintf (out, "%s", NOTE (2));
}

void SongDecompiler::GetId (Node<Token> * node)
{
    NEXT ();

    fprintf (out, "4\nW:%s\n", node->data.name);

    if (node->right)
    {
        Calculate (node->right);
    }
}

#define CHECK_FUNC( opcode )        \
    case opcode:                    \
        PrintOp (opcode);           \
        PrintOp (LEFT_B);           \
        Calculate (node->left);     \
        PrintOp (RIGHT_B);          \
    break;


void SongDecompiler::GetOp (Node<Token> * node)
{
    if (node->data.type == MATH_TYPE && 
        operators[node->data.data].priority < 
        operators[node->parent->data.data].priority)
    {
        PrintOp (LEFT_B);
    }

    switch (node->data.data)
    {
        case NOT:
            PrintOp (NOT);
            Calculate (node->right);
        break;

        case DEF_FUNC:
            PrintOp (DEF_FUNC);
            Calculate (node->right);
            PrintOp (LEFT_B);
            Calculate (node->left);
            PrintOp (RIGHT_B);
            Calculate (node->right->right);
        break;

        case DEF_VAR:
            PrintOp (DEF_VAR);
            Calculate (node->left);
            if (node->right)
            {
                PrintOp (ASSIGN);
                Calculate (node->right);
            }
        break;

        case CALL:
            Calculate (node->right);
            PrintOp (LEFT_B);
            Calculate (node->left);
            PrintOp (RIGHT_B);
        break;

        CHECK_FUNC (RET)
        CHECK_FUNC (PRNT)
        CHECK_FUNC (INPUT)
        CHECK_FUNC (PUTC)

        case IF:
            PrintOp (IF);
            PrintOp (LEFT_B);
            Calculate (node->left);
            PrintOp (RIGHT_B);
            Calculate (node->right);
        break;

        case WHILE:
            PrintOp (WHILE);
            PrintOp (LEFT_B);
            Calculate (node->left);
            PrintOp (RIGHT_B);
            Calculate (node->right);
        break;

        case DEF:
            Calculate (node->left);
            Calculate (node->right);
        break;

        case LINK:
            Calculate (node->right);
            if (node->left)
            {
                PrintOp (ELSE);
                Calculate (node->left);
            }
        break;

        case OPERATOR:
            Calculate (node->left);
            Calculate (node->right);
        break;

        case BLOCK:
            PrintOp (LEFT_BL);
            Calculate (node->right);
            PrintOp (RIGHT_BL);
        break;

        case COMMA:
            Calculate (node->right);
            if (node->left)
            {
                PrintOp (COMMA);
                Calculate (node->left);
            }
        break;

        default:
            Calculate (node->left);
            PrintOp   (node->data.data);
            Calculate (node->right);
        break;
    }

    if (node->data.type == MATH_TYPE && 
        operators[node->data.data].priority < 
        operators[node->parent->data.data].priority)
    {
        PrintOp (RIGHT_B);
    }
}

void SongDecompiler::PrintOp (size_t opcode)
{
    NEXT ();

    for (size_t i = 0; i < operators[opcode].seq_len; i++)
    {
        fprintf (out, "%s", ((i == 0) ? "z" : NOTE(operators[opcode].seq[i] - 1)));
    }
}



