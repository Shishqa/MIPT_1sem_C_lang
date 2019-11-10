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
    
    bool quit = false;

    while (!quit)
    {
        GuessSession (&questions);

        quit = true;
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

    printf ("phrase about guessing\n");

    AskQuestion (questions->root);
}

bool AskQuestion (Node<String> * question)
{
    if (!question->left && !question->right)
    {
        return (TryAnswer (question));
    }

    printf ("your personage %s?", question->data);

    if (GetAns ())
    {
        return (AskQuestion (question->right));
    }

    return (AskQuestion (question->left));
}

bool TryAnswer (Node<String> * question)
{
    printf ("is this %s?\n", question->data);

    if (GetAns ())
    {
        printf ("As I knew!\n");
        return (true);
    }

    printf ("so sad :(\n");
    return (false);
}

bool GetAns ()
{
    printf ("[y/n]: ");

    char ans = 0;

    scanf ("%c", &ans);

    return (ans == 'y');
}