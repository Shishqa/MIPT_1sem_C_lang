#include "head.hpp"
#include "parser.hpp"

int main () 
{
    SongParser p = {};

    p.Parse ("test/song_1");

    return (0);
}