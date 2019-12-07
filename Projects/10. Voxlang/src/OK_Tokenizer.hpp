#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include "head.hpp"
#include "OK_CreateNode.hpp"

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

    int Interpret ()
    {
        switch (*cur)
        {
            case 'A':
                return (9);
            break;

            case 'B':
                return (8);
            break;

            case 'c':
                return (7);
            break;

            case 'd':
                return (6);
            break;

            case 'e':
                return (5);
            break;

            case 'f':
                return (4);
            break;

            case 'g':
                return (3);
            break;

            case 'a':
                return (2);
            break;

            case 'b':
                return (1);
            break;
        
            default:
                return (0);
            break;
        }
    }

    void ParseName ()
    {
        char * new_name = (char *) calloc (MAX_NAME, sizeof (*new_name));
        size_t name_len = 0;

        sscanf (cur, "%[^,.!?:;\0]%n", new_name, &name_len);

        names[n_num++] = SetNode (ID_TYPE, 0, new_name);

        cur += name_len + 1;
    }

    void ParseOperator ()
    {
        assert (*cur == '[');
        cur++;

        bool chord[10] = {};

        while (*cur != ']' || *cur != '\0')
        {
            chord[Interpret ()] = true;
            cur++;
        }

        size_t min_dist = 100;
        size_t max_dist = 0;

        const size_t UNDEF = 100;

        size_t first = UNDEF;
        size_t last  = UNDEF;

        for (size_t i = 1; i < 10; i++)
        {
            if (chord[i])
            {
                if (first == UNDEF)
                {
                    first = i;
                }

                max_dist = i - first;

                if (last != UNDEF && min_dist > i - last)
                {   
                    min_dist = i - last;
                }

                last = i;
            }
        }

        for (size_t i = 0; i < MATH_OP_CNT; i++)
        {
            if (max_dist == math_op[i].max_dist &&
                min_dist == math_op[i].min_dist)
            {
                tokens[t_num++] = SetNode (MATH_TYPE, math_op[i].opcode, nullptr);
                return;
            }
        }

        for (size_t i = 0; i < OP_CNT; i++)
        {
            if (max_dist == operators[i].max_dist &&
                min_dist == math_op[i].min_dist)
            {
                tokens[t_num++] = SetNode (MATH_TYPE, math_op[i].opcode, nullptr);
                return;
            } 
        }
    }

    void ParseNumber ()
    {
        int num = 0;

        while (*cur != '[' && *cur != '\0')
        {
            num = num * 10 + Interpret ();
            cur++;
        }

        tokens[t_num++] = SetNode (NUM_TYPE, num, nullptr);
    }

    void ParseStave ()
    {
        while (*cur)
        {
            switch (*cur)
            {
                case '[':
                    ParseOperator ();
                break;
            
                default:
                    ParseNumber ();
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
                                tokens[t_num++] = SetNode (OP_TYPE, DEF_FUNC, "def");
                                ParseName ();
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