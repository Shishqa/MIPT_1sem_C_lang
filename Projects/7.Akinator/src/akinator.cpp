#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "../../6.Binary_Tree/src/tree.h"

const size_t QUESTION_MAX_SIZE = 30;

struct String
{
    char data[QUESTION_MAX_SIZE];
};

bool initBase (const char * path);
bool GuessSession (BinaryTree<String> * questions);
bool AskQuestion (Node<String> * question);
bool TryAnswer (Node<String> * question);
bool GetAns ();

int main ()
{
    BinaryTree<String> questions = {};

    printf ("Hi, I am Akinator!\n");
    
    bool play = true;

    while (play)
    {
        play = GuessSession (&questions);
    }

    initBase ("base/data");

    return (0);
}

bool initBase (const char * path)
{
    BinaryTree <String> sample = {};

    sample.root = (Node<String> *) calloc (1, sizeof (*sample.root));

    strncpy (sample.root->data.data, "animal", 7);

    FILE * f = fopen (path, "w");

    sample.print (f);

    fclose (f);
}

bool GuessSession (BinaryTree<String> * questions)
{
    questions->init ("base/data");

    printf ("Make up a character and I'll try guess it\n");
    printf ("Ready? ");

    if (GetAns ())
    {
        return (AskQuestion (questions->root));
    }
    else
    {
        return (false);
    }
}

bool AskQuestion (Node<String> * question)
{
    if (!question->left && !question->right)
    {
        return (TryAnswer (question));
    }

    printf ("Your character %s?", question->data);

    if (GetAns ())
    {
        return (AskQuestion (question->right));
    }

    return (AskQuestion (question->left));
}

bool TryAnswer (Node<String> * question)
{
    printf ("Is this %s?\n", question->data);

    if (GetAns ())
    {
        printf ("As I knew!\n");
        return (true);
    }

    printf ("So sad :(\n");
    return (false);
}

bool GetAns ()
{
    printf ("[y/n]: ");

    char ans = 0;

    scanf ("%c%*c", &ans);

    return (ans == 'y');
}