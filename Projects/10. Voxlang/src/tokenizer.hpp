#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "head.hpp"
#include "CreateNode.hpp"

#define CHECK( ch, code )               \
        case (ch):                      \
            cur++;                      \
            if (*cur == ':')            \
            {                           \
                cur++;                  \
                SkipSpaces ();          \
                code                    \
            }                           \

class Tokenizer
{
    const String * code;
    const char * cur;
    size_t curr_line;
    size_t n_lines;

    const size_t N_MAX = 10000;
    Node<Token> ** names;
    size_t n_num;

    const size_t T_MAX = 10000;
    Node<Token> ** tokens;
    size_t t_num;

    const size_t MAX_NAME = 1000;
    const size_t MAX_NUM = 10;

    void ParseName ()
    {
        char * new_name = (char *) calloc (MAX_NAME, sizeof (*new_name));
        size_t name_len = 0;

        sscanf (cur, "%[^,.!?:;\0]%n", new_name, &name_len);

        names[n_num++] = SetNode (ID, new_name, name_len);

        cur += name_len + 1;
    }

    void ParseNum ()
    {
        char * num = (char *) calloc (MAX_NUM, sizeof (*num));
        size_t num_len = 0;

        sscanf (cur, "%[0-9]%n", num, &num_len);

        tokens[t_num++] = SetNode (NUM, num, num_len);

        cur += num_len;
    }

    void ParseStave ()
    {
        while (*cur)
        {
            switch (*cur)
            {
                case '[':
                /* code */
                break;
            
            default:
                break;
            }
        }
    }

    void Proceed ()
    {
        while (curr_line < n_lines)
        {
            SkipSpaces ();

            switch (*cur)
            {
                CHECK ('W', {
                                while (*cur)
                                {
                                    ParseName ();
                                    SkipSpaces ();
                                }
                            })
                break;

                CHECK ('T', {
                                tokens[t_num++] = SetNode (DEF_FUNC, "def", 3);
                                ParseName ();
                            })
                break;

                CHECK ('X', {
                                ParseNum ();
                            })
                break;
            
                default:
                    if (*(cur + 1) != ':')
                    {
                        ParseStave ();
                    }
                break;
            }

            cur = code[++curr_line].begin;
        }
    }

    void SkipSpaces ()
    {
        while (*cur == ' ' || *cur == '\t' || *cur == '\r')
        {
            cur++;
        }
        
    }

    public:

    Node<Token> ** tokenize (const String * text, size_t num_of_lines)
    {
        tokens = (Node<Token> **) calloc (T_MAX, sizeof (*tokens));

        code = text;
        n_lines = num_of_lines;

        cur = code[0].begin;
        curr_line = 0;

        Proceed ();

        return (tokens);
    }
};

#endif // !TOKENIZER_HPP