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
            case 'e':
                return (9);
            break;

            case 'd':
                return (8);
            break;

            case 'c':
                return (7);
            break;

            case 'B':
                return (6);
            break;

            case 'A':
                return (5);
            break;

            case 'G':
                return (4);
            break;

            case 'F':
                return (3);
            break;

            case 'E':
                return (2);
            break;

            case 'D':
                return (1);
            break;
        
            default:
                return (0);
            break;
        }
    }

    void ParseName ()
    {
        SkipSpaces ();

        //printf ("ParseName :: HI :: %c\n", *cur);

        char * new_name = (char *) calloc (MAX_NAME, sizeof (*new_name));
        int name_len = 0;

        sscanf (cur, "%[^,.?!]%n", new_name, &name_len);

        //printf ("ParseName :: %lu\n", n_num);

        names[n_num++] = SetNode (ID_TYPE, name_len, new_name);

        //printf ("ParseName :: BYE\n");

        cur += name_len;

        if (*cur != '\0')
        {
            cur++;
        }
    }

    void ParseOperator ()
    {
        //printf ("ParseOp :: hi, friends! %lu\n", curr_line);

        bool chord[10] = {};
        int len = 0;

        while (*cur != '\0' && len < 4)
        {
            while (!isalpha (*cur) && *cur != '|' && *cur != '\0')
            {
                cur++;
            }

            if (*cur == '|')
            {
                break;
            }

            chord[Interpret ()] = true;
            cur++;
            len++;
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

        //printf ("ParseOp :: I got %lu %lu!\n", max_dist, min_dist);

        for (size_t i = 0; i < OP_CNT; i++)
        {
            if (max_dist == operators[i].max_dist &&
                min_dist == operators[i].min_dist)
            {
                tokens[t_num++] = SetNode (operators[i].type, operators[i].opcode, operators[i].name);
                return;
            } 
        }
    }

    void ParseNumber ()
    {
        int num = 0;
        bool got_in = false;

        while (*cur != '|' && *cur != '\0' && *cur != '\r')
        {
            num = num * 10 + Interpret ();
            cur++;
            got_in = true;
        }

        if (got_in)
        {
            tokens[t_num++] = SetNode (NUM_TYPE, num, nullptr);
        }
    }

    void ParseStave ()
    {
        //printf ("ParseStave :: Hi! :: %lu\n", curr_line);

        while (*cur && *cur != '\r')
        {
            SkipSpaces ();
            switch (*cur)
            {
                case '|':
                    cur++;
                    ParseOperator ();
                break;
            
                default:
                    if (!isalpha (*cur))
                    {
                        cur++;
                    }
                    ParseNumber ();
                break;
            }
        }
    }

    void Proceed ()
    {
        while (curr_line < n_lines)
        {
            //printf ("Proceed :: %lu %c\n", curr_line, *cur);

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
                                tokens[t_num++] = SetNode (OP_TYPE, DEF_FUNC, "DEF");
                                ParseName ();
                            })
                break;

                case '%':
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
        assert (text != nullptr);

        tokens = (Node<Token> **) calloc (T_MAX, sizeof (*tokens));
        t_num  = 0;

        names = (Node<Token> **) calloc (N_MAX, sizeof (*tokens));
        n_num = 0;

        code = text;
        n_lines = num_of_lines;

        cur = code[0].begin;
        curr_line = 0;

        Proceed ();

        //printf ("tokenize:: HELLO\n\n\n");
        for (size_t i = 0; i < t_num; i++)
        {
            printf ("token : %d %d %s\n\n", tokens[i]->data.type, tokens[i]->data.data, tokens[i]->data.name);
        }
        for (size_t i = 0; i < n_num; i++)
        {
            printf ("name : %d %d %s\n\n", names[i]->data.type, names[i]->data.data, names[i]->data.name);
        }

        return (nullptr);
    }
};

#endif // !TOKENIZER_HPP