#include <stdio.h>
#include <stdlib.h>

#include "speech.h"

void Say (const char * phrase, int speed, int vol)
{
    printf ("%s\n", phrase);

    char * call_espeak = (char *) calloc (CALL_MAX_SIZE, sizeof (char));

    sprintf (call_espeak, "espeak -ven+m3 -p 70 -s %d -a %d \"%s\"", speed, vol, phrase);

    system (call_espeak);

    free (call_espeak);
}

void Say (const char * prefix, const char * gap, const char * suffix, int speed, int vol)
{
    char * phrase = (char *) calloc (PHRASE_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s", prefix, gap, suffix);

    Say (phrase, speed, vol);

    free (phrase);
}

void Say (const char * prefix, const char * gap1, const char * mid, const char * gap2, const char * suffix, int speed, int vol)
{
    char * phrase = (char *) calloc (PHRASE_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s%s%s", prefix, gap1, mid, gap2, suffix);

    Say (phrase, speed, vol);

    free (phrase);
}