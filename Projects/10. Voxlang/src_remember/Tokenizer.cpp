// #include "libraries.hpp"
// #include "Tokenizer.hpp"
// #include "SetNode.hpp"
// #include "Operators.hpp"

// Node<Token> ** Tokenizer::tokenize (const char * str)
// {
//     error = TOKENIZER_OK;

//     cur = str;
//     line = 1;

//     tokens = (Node<Token> **) calloc (MAX_TOKENS, sizeof (*tokens));
//     n_tokens = 0;

//     Proceed ();

//     if (error != TOKENIZER_OK)
//     {
//         PrintError ();
//         free (tokens);
//         return (nullptr);
//     }

//     return (tokens);
// }

// void Tokenizer::PrintError ()
// {
//     SETCOLOR (RED);
//     printf ("Error: ");
//     SETCOLOR (RESET);

//     switch (error)
//     {
//         case UNKNOWN_OPERATOR:
//             printf ("unknown operator");
//         break;
    
//         default:
//             printf ("strange error");
//         break;
//     }

//     printf ("\nLine %lu: ", line);
//     SETCOLOR (RED);
//     printf ("%c (%d)", *cur, *cur);
//     SETCOLOR (RESET);
//     printf ("\n");
// }

// #define CHECK_TAG( tag, code )      \
//     {                               \
//         case tag:                   \
//             cur++;                  \
//             if (*cur == ':')        \
//             {                       \
//                 cur++;              \
//                 code                \
//             }                       \
//             SkipLine ();            \
//         break;                      \
//     }   

// void Tokenizer::Proceed ()
// {
//     SkipSpaces ();

//     while (*cur)
//     {
//         printf ("line: %lu\n", line);
//         switch (*cur)
//         {
//             CHECK_TAG ('X', {
//                 printf ("X\n");
//                 tokens[n_tokens++] = SetNode (OP_TYPE, DEF_FUNC);
//             })

//             CHECK_TAG ('T', {
//                 printf ("T\n");
//                 ParseId ();
//             })

//             CHECK_TAG ('W', {
//                 printf ("W\n");
//                 ParseId ();
//             })

//             case '[':
//                 while (*cur == '[')
//                 {
//                     printf ("OP ");
//                     ParseOp ();
//                     SkipSpaces ();
//                 }
//             break;
        
//             default:
//                 printf ("%s\n", cur);
//                 printf ("SKIP\n");
//             break;
//         }

//         SkipLine ();

//         if (error != TOKENIZER_OK)
//         {
//             return;
//         }
//     }
// }

// void Tokenizer::ParseId ()
// {
//     char * new_name = (char *) calloc (MAX_NAME_LEN, sizeof (char));
//     size_t len = 0;

//     sscanf (cur, "%[^\n]%n", new_name, &len);

//     if (len != 0)
//     {
//         tokens[n_tokens++] = SetNode (ID_TYPE, len, new_name);
//     }
// }

// void Tokenizer::ParseOp ()
// {
//     printf ("Parse op %c\n", *cur);

//     char set[3] = {};
//     size_t count = 0;

//     while (*cur && *cur != ']' && *cur != '&' && *cur != '|')
//     {
//         printf ("%c", *cur);
//         fflush (stdout);
//         if (isalpha (*cur))
//         {
//             set[count++] = *cur;
//             if (count == 3)
//             {
//                 break;
//             }
//         }
//         cur++;
//     }
//     while (*cur && *cur != ']' && *cur != '&' && *cur != '|')
//     {
//         cur++;
//     }

//     printf ("%c\n", *cur);
//     fflush (stdout);

//     if (count < 3)
//     {
//         printf ("Not enough notes in a chord\n");
//         return;
//     }
//     else if (*cur == '&' || *cur == '|' || !*cur)
//     {
//         printf ("Unclosed chord!\n");
//         return;
//     }

//     cur++;

//     int op = 0;

//     while (*cur != '&' && *cur != '|')
//     {
//         printf ("%c", *cur);
//         fflush (stdout);

//         if (*cur == set[0])
//         {
//             op = 3 * op + 0;
//         }
//         else if (*cur == set[1])
//         {
//             op = 3 * op + 1;
//         }
//         else if (*cur == set[2])
//         {
//             op = 3 * op + 2;
//         }

//         cur++;
//     }

//     printf ("%s\n", operators[op].name);
//     fflush (stdout);

//     if (op == NUM_REQ)
//     {
//         while (*cur && *cur != '&')
//         {
//             cur++;
//         }
//         cur++;
//         ParseNum (set);
//     }
//     else
//     {
//         while (*cur && *cur != '|')
//         {
//             cur++;
//         }
//         if (*cur)
//         {
//             cur++;
//         }
//     }
// }

// void Tokenizer::ParseNum (char * set)
// {
//     int res = 0;

//     while (*cur != '|')
//     {
//         SkipSpaces ();
//         for (size_t i = 0; i < 3; i++)
//         {
//             if (*cur == set[i] || *cur == tolower(set[i]))
//             {
//                 res = res * 3 + (i + 1);
//                 break;
//             }
//             else if (*cur == 'z')
//             {
//                 res = res * 3;
//                 break;
//             }
//         }
//         cur++;
//         if (*cur == '/')
//         {
//             cur++;
//         }
//         else
//         {
//             res = 
//         }

//     }
// }

// void Tokenizer::SkipSpaces ()
// {
//     while (*cur == ' ' || *cur == '\n' || *cur == '\t' || *cur == '\r')
//     {
//         if (*cur == '\n')
//         {
//             line++;
//         }

//         cur++;
//     }
// }

// void Tokenizer::SkipLine ()
// {

//     while (*cur && *cur != '\n')
//     {
//         cur++;
//     }

//     if (*cur == '\n')
//     {
//         cur++;
//         line++;
//     }
// }