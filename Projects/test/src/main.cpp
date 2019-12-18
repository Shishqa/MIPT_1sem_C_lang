#include <cstdio>
#include <cstdlib>

int main ()
{
    system ("espeak -g 0 -s 200 -p 0 \"hello friend\"");
    system ("espeak -g 0 -s 200 -p 20 see");
    system ("espeak -g 0 -s 150 -p 30 trees");
    system ("espeak -g 0 -s 150 -p 30 of");
    system ("espeak -g 0 -s 50  -p 70 green");

    return (0);
}