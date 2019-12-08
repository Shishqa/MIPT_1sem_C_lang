#include "Tokenizer.hpp"
#include "libraries.hpp"

Node<Token> ** Tokenizer::tokenize (const char * str)
{
    cur = str;

    res = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*res));
    n_tokens = 0;

    Proceed ();
}

void Tokenizer::Proceed ()
{
    while (*cur)
    {
        SkipSpaces ();

        if (isdigit (*cur))
        {
            ParseNum ();
        }

        if (isalpha (*cur))
        {
            ParseId ();
        }

        ParseOp ();
    }
}

void Tokenizer::ParseNum()
{
    int num = 0;
    size_t num_len = 0;

    sscanf (cur, "%d%n", &num, &num_len);

    
}

void Tokenizer::SkipSpaces ()
{
    while (*cur == ' ' || *cur == '\n' || *cur == '\t' || *cur == '\r')
    {
        cur++;
    }
}