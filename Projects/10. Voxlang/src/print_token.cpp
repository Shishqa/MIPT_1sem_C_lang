#include "head.hpp"

void PrintToken (FILE * out, const void * data)
{
    Token * token = (Token *) data;

    if (*(token->lexem) == '|' ||       // Экранирует спец.символы Dot 
        *(token->lexem) == '<' ||
        *(token->lexem) == '>' )
    {
        fprintf (out, "\\");
    }

    fprintf (out, "%s", token->lexem);
}