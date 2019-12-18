#include "libraries.hpp"
#include "Tokenizer.hpp"
#include "SetNode.hpp"
#include "Operators.hpp"

Node<Token> ** Tokenizer::tokenize (char * str, size_t str_len)
{
    error = TOKENIZER_OK;

    n_lines = ArrangePointers (str, str_len, &code);
    line = 0;

    tokens = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*tokens));
    n_tokens = 0;

    cur = code[0].begin;

    Proceed ();

    if (error != TOKENIZER_OK)
    {
        PrintError ();
        free (tokens);
        return (nullptr);
    }

    return (tokens);
}

void Tokenizer::PrintError ()
{
    SETCOLOR (RED);
    printf ("Error: ");
    SETCOLOR (RESET);

    switch (error)
    {
        case UNKNOWN_OPERATOR:
            printf ("unknown operator");
        break;
    
        default:
            printf ("strange error");
        break;
    }

    printf ("\nLine %lu: ", line);
    SETCOLOR (RED);
    printf ("%c (%d)", *cur, *cur);
    SETCOLOR (RESET);
    printf ("\n");
}

void Tokenizer::Proceed ()
{
    while (line < n_lines)
    {
        ParseLine ();
        SkipSpaces ();
    }
}

#define CHECK_TAG( tag, code )          \
    case tag:                           \
    {                                   \
        cur += 2;                       \
        code                            \
    }                                   \
    break;

void Tokenizer::ParseLine ()
{
    if (*cur == '%')
    {
        cur = code[line].end;
        return;
    }

    printf ("line %lu\n", line);
    printf ("%s\n", code[line].begin);

    printf ("%c %c\n", *cur, *(cur + 1));

    switch (*(cur + 1))
    {
        case ':':
            switch (*cur)
            {
                CHECK_TAG ('X', {
                    printf ("got X\n");
                    tokens[n_tokens++] = SetNode (OP_TYPE, DEF_FUNC);
                })

                CHECK_TAG ('T', {
                    printf ("got T\n");
                    tokens[n_tokens++] = SetNode (ID_TYPE, code[line].end - cur, cur);
                })

                CHECK_TAG ('W', {
                    printf ("got W\n");
                    tokens[n_tokens++] = SetNode (ID_TYPE, code[line].end - cur, cur);
                })
            }
        break;
    
        default:
            ParseChords ();
        break;
    }

    cur = code[line].end;
}

void Tokenizer::ParseChords ()
{

}

void Tokenizer::ParseOp ()
{
    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (!strncmp (cur, operators[i].name, operators[i].name_len))
        {
            tokens[n_tokens++] = SetNode (operators[i].type, operators[i].opcode);

            assert (operators[i].name_len > 0); 

            cur += operators[i].name_len;
            return;
        }
    }

    error = UNKNOWN_OPERATOR;
}

void Tokenizer::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\t' || *cur == '\r' || *cur == '\0')
    {
        if (*cur == '\0')
        {
            ++line;
        }
        cur++;
    }
}

char Tokenizer::FindLetter ()
{
    while (!isalpha (*cur) && *cur != '|')
    {
        if (cur == code[line].end)
        {
            error = EXPECTED_LINE;
            return (0);
        }
        cur++;
    }

    switch (*cur)
    {
        case '|':
            return (0);
        break;
    
        default:
            return (*cur);
        break;
    }
}