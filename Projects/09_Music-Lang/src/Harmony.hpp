#ifndef HARMONY_HPP
#define HARMONY_HPP

#include <cstdlib>

const size_t MAX_CHORD_NUM = 100;
const size_t MAX_NOTES = 10;

const size_t NOTE_CNT = 42;

static char basic_notes[NOTE_CNT][4] = 
{
    "C,", "^C,",
    "D,", "^D,",
    "E,", "^E,",
    "F,", "^F,",
    "G,", "^G,",
    "A,", "^A,",
    "B,", "^B,",
    "C",  "^C",
    "D",  "^D",
    "E",  "^E",
    "F",  "^F",
    "G",  "^G",
    "A",  "^A",
    "B",  "^B",
    "c",  "^c",
    "d",  "^d",
    "e",  "^e",
    "f",  "^f",
    "g",  "^g",
    "a",  "^a",
    "b",  "^b"
};

struct Chord
{
    size_t num_of_notes;
    int notes[MAX_NOTES];

    int tonic;

    bool init (char ** cur);
    int getNote (char ** cur);

    private:
    bool addNote (int note);
};

class Harmony
{
    public:
    bool is_valid;

    size_t curr_chord;
    size_t chord_cnt;

    Chord chords[MAX_CHORD_NUM];
    
    void init (const char * path);
    void next ();
};

#endif // !HARMONY_HPP
