#include "libraries.hpp"
#include "Tokenizer.hpp"
#include "SetNode.hpp"
#include "Operators.hpp"

Node<Token> ** Tokenizer::tokenize (const char * str)
{
    cur = str;

    tokens = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*tokens));
    n_tokens = 0;

    printf ("Tokenizer:: got started\n");

    Proceed ();

    return (tokens);
}

void Tokenizer::Proceed ()
{
    while (*cur)
    {
        SkipSpaces ();

        if (*cur == '%')
        {
            while (*cur != '\n' && *cur != '\0')
            {
                cur++;
            }
            if (*cur == '\n')
            {
                cur++;
            }
        }
        else if (isdigit (*cur))
        {
            ParseNum ();
        }
        else if (isalpha (*cur))
        {
            ParseId ();
        }
        else
        {
            ParseOp ();
        }
    }
}

void Tokenizer::ParseNum ()
{
    int num = 0;
    size_t num_len = 0;

    sscanf (cur, "%d%n", &num, &num_len);

    tokens[n_tokens++] = SetNode (NUM_TYPE, num);

    cur += num_len;
}

void Tokenizer::ParseId ()
{
    char * name = (char *) calloc (MAX_NAME_LEN, sizeof (*name));
    int name_len = 0;

    bool is_operator = false;

    sscanf (cur, "%[a-zA-Z0-9_]%n", name, &name_len);

    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (name_len == operators[i].name_len &&
            !strncmp (name, operators[i].name, name_len))
        {
            tokens[n_tokens++] = SetNode (OP_TYPE, operators[i].opcode);

            is_operator = true;
            break;
        }
    }

    if (!is_operator)
    {
        tokens[n_tokens++] = SetNode (ID_TYPE, name_len, name);
    }

    cur += name_len;
}

void Tokenizer::ParseOp ()
{
    for (size_t i = 0; i < OP_CNT; i++)
    {
        if (!strncmp (cur, operators[i].name, operators[i].name_len))
        {
            tokens[n_tokens++] = SetNode (MATH_TYPE, operators[i].opcode);

            cur += operators[i].name_len;
            return;
        }
    }
}

void Tokenizer::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\n' || *cur == '\t' || *cur == '\r')
    {
        cur++;
    }
}