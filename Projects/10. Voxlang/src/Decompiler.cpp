#include "libraries.hpp"
#include "Parser.hpp"
#include "Decompiler.hpp"
#include "Operators.hpp"

#define N node
#define L node->left
#define R node->right

#define STOP( err_code )                    \
    {                                       \
        error = err_code;                   \
        return;                             \
    }                                   

#define NEXT()                      \
    {                               \
        hrmny.next();               \
    }

void Harmony::init (const char * path)
{
    curr_chord = 0;
    FILE * in = fopen (path, "r");

    if (!in)
    {
        printf ("can't open %s\n", path);
        return;
    }
    
    char * buf = nullptr;
    Read (&buf, in);
    fclose (in);

    chord_cnt = 0;
    size_t curr_note = 0;

    int skip = 0;

    while (*buf == '[')
    {
        curr_note = 0;
        buf++;
        while (*buf != ']')
        {
            sscanf (buf, "%m[^.]%n", &(chords[chord_cnt].bass[curr_note]), &skip);
            buf += skip + 1;
            curr_note++;
        }

        chords[chord_cnt].num_of_notes = curr_note;
        buf++;

        curr_note = 0;
        while (*buf && *buf != '|' && *buf != '\n' && *buf != ' ')
        {
            sscanf (buf, "%m[^.]%n", &(chords[chord_cnt].high[curr_note]), &skip);
            buf += skip + 1;
            curr_note++;
        }

        if (curr_note != chords[chord_cnt].num_of_notes)
        {
            printf ("jopa %s\n", buf);
            return;
        }

        chord_cnt++;

        while (*buf == ' ' || *buf == '\n' || *buf == '\r' || *buf == '\t' || *buf == '|')
        {
            buf++;
        }
    }
}

void Harmony::next ()
{
    curr_chord = (curr_chord + 1) % chord_cnt;
}

bool Decompiler::GetCode (BinaryTree<Token> * code, const char * output_path, const char * harmony_path)
{
    hrmny.init (harmony_path);

    printf ("got harmony\n");

    FILE * f = fopen (output_path, "w");

    fprintf (f, "%% ##      THIS FILE IS GENERATED AUTOMATICALLY      ##\n");
    fprintf (f, "%% ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##\n");

    prog = code;

    offset = 0;
    func_cnt = 1;
    Proceed (f);
    
    fclose (f);

    return (true);
}

void Decompiler::Proceed (FILE * f)
{
    out = f;

    Calculate (prog->root);
}

void Decompiler::Calculate (Node<Token> * node)
{
    if (!node)
    {
        return;
    }

    switch (node->data.type)
    {
        case NUM_TYPE:
            PrintNum (node);
        break;

        case ID_TYPE:
            PrintId (node);
        break;

        default:
            GetOperator (node);
        break;
    }
}

void Decompiler::GetOperator (Node<Token> * node)
{
    printf ("op %d\n", node->data.data);

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
            fprintf (out, "X:%lu\n", func_cnt++);
            fprintf (out, "T:%s\n", node->right->data.name);
            fprintf (out, "M:4/4\nL:1/4\nQ:1/4=212\nK:C\n");
            Calculate (node->left);
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
            PrintCall (node);
        break;

        case RET:
            PrintOp (RET);
            PrintOp (LEFT_B);
            Calculate (node->left);
            PrintOp (RIGHT_B);
        break;

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
            fprintf (out, "\n");
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
    
        default:
            Calculate (node->left);
            PrintOp (node->data.data);
            Calculate (node->right);
        break;
    }

    if (node->data.type == MATH_TYPE && 
        operators[node->data.data].priority < 
        operators[node->parent->data.data].priority)
    {
        PrintOp (RIGHT_BL);
    }
}

void Decompiler::PrintCall (Node<Token> * node)
{
    printf ("gonna print call %s\n", node->right->data.name);
    fprintf (out, "%% call %s\n", node->right->data.name);

    NEXT ();

    for (size_t i = 0; i < 3; i++)
    {
        fprintf (out, "%s", hrmny.chords[hrmny.curr_chord].bass[i]);
    }
    fprintf (out, "%s & ", hrmny.chords[hrmny.curr_chord].bass[2]);
}

void Decompiler::PrintNum (Node<Token> * node)
{
    printf ("gonna print num %d\n", node->data.data);
    fprintf (out, "%% num %d\n", node->data.data);

    NEXT ();

    for (size_t i = 0; i < 3; i++)
    {
        fprintf (out, "%s", hrmny.chords[hrmny.curr_chord].bass[i]);
    }
    fprintf (out, "%s & ", hrmny.chords[hrmny.curr_chord].bass[0]);

    int num = node->data.data + 127;
    assert (num >= 0 && num <= 256);

    char seq[4] = {};

    for (int i = 3; i >= 0; i--)
    {
        seq[i] = num % 4;
        num /= 4;
    } 

    for (int i = 0; i < 4; i++)
    {
        printf ("%d", seq[i]);
        fprintf (out, "%s", ((seq[i]) ? hrmny.chords[hrmny.curr_chord].high[seq[i] - 1] : "z"));
    }

    fprintf (out, " |\n"); 
    printf ("num printed\n");
}

void Decompiler::PrintId (Node<Token> * node)
{
    printf ("gonna print id %s\n", node->data.name);
    fprintf (out, "%% id %s\n", node->data.name);

    NEXT ();

    for (size_t i = 0; i < 3; i++)
    {
        fprintf (out, "%s", hrmny.chords[hrmny.curr_chord].bass[i]);
    }
    fprintf (out, "z & ");

    for (size_t i = 0; i < 3; i++)
    {
        fprintf (out, "%s", hrmny.chords[hrmny.curr_chord].high[i]);
    }
    fprintf (out, "z |\n");

    fprintf (out, "W: %s\n", node->data.name);
    printf ("id printed\n");
}

void Decompiler::PrintOp (size_t opcode)
{
    printf ("gonna print op %s\n", operators[opcode].name);
    fprintf (out, "%% op %s\n", operators[opcode].name);

    NEXT ();

    for (size_t i = 0; i < 3; i++)
    {
        fprintf (out, "%s", hrmny.chords[hrmny.curr_chord].bass[i]);
    }
    fprintf (out, "%s & ", hrmny.chords[hrmny.curr_chord].bass[1]);

    size_t ptr = 0;

    for (size_t i = 0; i < 4; i++)
    {
        ptr = operators[opcode].sequence[i];
        fprintf (out, "%s", ((ptr == 0) ? "z" : hrmny.chords[hrmny.curr_chord].high[ptr - 1]));
    }

    fprintf (out, " |\n");

    printf ("op printed\n");
}