#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../6.Binary_Tree/src/tree.h"

const size_t STRING_MAX_SIZE = 50;

const size_t SL_TIME = 500000;

struct String
{
    char data[STRING_MAX_SIZE];
};

bool initData (const char * path);
bool GuessSession (BinaryTree<String> * questions);
bool AskQuestion (Node<String> * question);
bool TryAnswer (Node<String> * question);
bool AddBranch (Node<String> * question);
bool GetAns ();

int main ()
{
    BinaryTree<String> questions = {};

    printf ("Hi, I am Akinator!\n");

    usleep (SL_TIME);
    
    bool play = true;

    while (play)
    {
        play = GuessSession (&questions);
    }

    //initData ("data/tree_1");

    return (0);
}

bool initData (const char * path)
{
    BinaryTree <String> sample = {};

    sample.root = (Node<String> *) calloc (1, sizeof (*sample.root));

    strncpy (sample.root->data.data, "Eugeny Ponasenkov", 18);

    FILE * f = fopen (path, "w");

    sample.print (f);

    fclose (f);
}

bool GuessSession (BinaryTree<String> * questions)
{
    questions->init ("data/tree_1");

    printf ("Make up a character and I'll try guess it\n");

    usleep (SL_TIME);

    printf ("Ready? ");

    if (GetAns ())
    {
        if (!AskQuestion (questions->root))
        {
            FILE * f = fopen ("data/tree_1", "w");

            questions->print (f);

            fclose (f);
        }

        return (true);
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

    printf ("Your character %s?\n", question->data.data);

    if (GetAns ())
    {
        return (AskQuestion (question->left));
    }

    return (AskQuestion (question->right));
}

bool TryAnswer (Node<String> * question)
{
    printf ("Let me think...\n");

    usleep (2 * SL_TIME);

    printf ("Is this %s?\n", question->data.data);

    if (GetAns ())
    {
        usleep (SL_TIME);
        printf ("As I knew!\n");
        return (true);
    }

    usleep (SL_TIME);
    printf ("So sad :(\n");

    AddBranch (question);

    return (false);
}

bool GetAns ()
{
    printf ("[y/n]: ");

    char ans = 0;

    scanf ("%c%*c", &ans);

    return (ans == 'y');
}

bool AddBranch (Node<String> * question)
{
    assert (!question->left && !question->right);

    usleep (SL_TIME);

    printf ("And who is it? [Это кто (who)?]\n");
    printf ("It is ");

    String character = {};

    fgets (character.data, STRING_MAX_SIZE, stdin);

    printf ("But what does %s differ from %s?\n", character.data, question->data.data);
    printf ("[type answer in style '%s loves pizza']\n", character.data);
    printf ("%s ", character.data);

    String difference = {};

    fgets (difference.data, STRING_MAX_SIZE, stdin);

    question->setRight ();
    question->setLeft ();

    strncpy (question->left->data.data,  character.data,      strlen (character.data) - 2);
    strncpy (question->right->data.data, question->data.data, strlen (question->data.data));

    memset (question->data.data, 0, STRING_MAX_SIZE);

    strncpy (question->data.data,        difference.data,     strlen (difference.data) - 1);

    printf ("I got it! Next time I'll guess him!\n");

    return (true);
}