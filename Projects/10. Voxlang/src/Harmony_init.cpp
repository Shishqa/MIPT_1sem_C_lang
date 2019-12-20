#include <cstdio>
#include <cstdlib>

#include "libraries.hpp"
#include "Harmony.hpp"

void Harmony::init (const char * path)
{
    is_valid = false;
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
    char * cur = buf;

    chord_cnt = 0;
    size_t curr_note = 0;

    int skip = 0;

    while (*cur == '[')
    {
        if (!(chords[chord_cnt].init (&cur)))
        {
            printf ("Can't init chords in %s\n", path);
            return;
        }

        if (chords[chord_cnt].num_of_notes > 5)
        {
            printf ("Warning: chord %lu is too big! you may get bad voice!\n", chord_cnt + 1);
        }

        chord_cnt++;

        while (*cur == ' ' || *cur == '\n' || *cur == '\r' || *cur == '\t' || *cur == '|')
        {
            cur++;
        }
    }

    free (buf);
    is_valid = true;
}

void Harmony::next ()
{
    curr_chord = (curr_chord + 1) % chord_cnt;
}

int Chord::getNote (char ** cur)
{
    int res = 0;

    if (**cur == '^')
    {
        res += 1;
        (*cur)++;
    }

    switch (**cur)
    {
        case 'C': res += 14; break;
        case 'D': res += 16; break;
        case 'E': res += 18; break;
        case 'F': res += 20; break;
        case 'G': res += 22; break;
        case 'A': res += 24; break;
        case 'B': res += 26; break;
        case 'c': res += 28; break;
        case 'd': res += 30; break;
        case 'e': res += 32; break;
        case 'f': res += 34; break;
        case 'g': res += 36; break;
        case 'a': res += 38; break;
        case 'b': res += 40; break;
    
        default:
            printf ("Unknown note '%c'!\n", **cur);
            return (-1);
        break;
    }
    (*cur)++;

    if (**cur == ',')
    {
        res -= 14;
        (*cur)++;
    }

    return (res);
}

bool Chord::init (char ** cur)
{
    if (**cur != '[')
    {
        printf ("Expected chord initialization!\n");
        return (false);
    }
    (*cur)++;

    tonic = 42;

    while (**cur != ']')
    {
        if (num_of_notes == MAX_NOTES)
        {
            printf ("Overfull of chord! Try to use less notes\n");
            return (false);
        }
        if (!addNote (getNote (cur)))
        {
            return (false);
        }
    }
    (*cur)++;

    return (true);
}

bool Chord::addNote (int note)
{
    if (note < 0 && note > 41)
    {
        return (false);
    }

    if (num_of_notes && note <= notes[num_of_notes - 1])
    {
        printf ("%s vs %s\n", basic_notes[note], basic_notes[notes[num_of_notes - 1]]);
        return (false);
    }

    notes[num_of_notes++] = note;

    if (note < tonic)
    {
        tonic = note;
    }

    return (true);
}