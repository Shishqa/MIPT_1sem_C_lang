#include "libraries.hpp"

#include "Parser.hpp"

#define RESET  "\033[0m"
#define RED    "\033[1;31m"

#define SETCOLOR(color)             \
        printf ("%s", color)        \

void Parser::PrintError ()
{
    SETCOLOR (RED);
    printf ("Error: ");
    SETCOLOR (RESET);

    switch (error)
    {
        case NUM_EXPECT:
        printf ("expected number\n");
        break;

        case PRIMARY_EXPECT:
        printf ("expected primary expression\n");
        break;

        case ID_EXPECT:
        printf ("expected name\n");
        break;

        case UNCLOSED_BRACES:
        printf ("unclosed braces\n");
        break;

        case VAR_FUNC:
        printf ("function defined as variable\n");
        break;

        case NO_END:
        printf ("expected ';'\n");
        break;

        case NO_BLOCK:
        printf ("expected definition of function\n");
        break;

        case EMPTY_BLOCK:
        printf ("block is empty\n");
        break;
    
        default:
        printf ("everything is OK, but developer of this code is bad\n");
        break;
    }

    size_t skip = 0;

    printf ("\nLine %lu: \"%n", line, &skip);

    const char * begin = newline + 1;
    const char * end = strchr (begin, '\n');

    if (end == nullptr)
    {
        printf ("%s", begin);
    }
    else
    {
        fwrite (begin, sizeof (char), end - begin, stdout);
    }

    printf ("\"\n");

    for (int i = 0; i < skip - 1; i++)
    {
        printf (" ");
    }

    for (int i = 0; i < cur - begin; i++)
    {
        printf ("_");
    }

    SETCOLOR (RED);
    printf ("^");
    SETCOLOR (RESET);

    for (int i = 0; i < end - cur - 1; i++)
    {
        printf ("_");
    }

    printf ("\n\n");
}
