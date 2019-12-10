#include "head.hpp"
#include "OK_Tokenizer.hpp"

int main () 
{
    Tokenizer t = {};

    FILE * in = fopen ("test/song_1", "r");

    char * buff = nullptr;
    size_t file_len = Read (&buff, in);

    fclose (in);

    String * code = nullptr;

    size_t num_of_lines = ArrangePointers (buff, file_len, &code);

    //printf ("main:: hello!\n");

    Node<Token> ** arr = t.tokenize (code, num_of_lines);
         
    printf ("SongParse::Parse - Tokenized!!! !\n");

    return (0); 
}