#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stack>

#include "../../6.Binary_Tree/src/tree.h"

const size_t STRING_MAX_SIZE = 100;
const size_t CALL_MAX_SIZE = 100;

const size_t SL_TIME = 200000;

const char * FUNNY_ANS[4] = {
                                "loves pizza",
                                "don't have a girlfriend",
                                "is yellow",
                                "has pink underpants"
                            }; 

struct String
{
    char data[STRING_MAX_SIZE];
};

void Say (const char * phrase);
void SaySingleGapPhrase (const char * prefix, const char * gap, const char * suffix);
void SayDoubleGapPhrase (const char * prefix, const char * gap1, const char * mid, const char * gap2, const char * suffix);

bool initData (const char * path);
bool GuessSession (BinaryTree<String> * questions);
bool AskQuestion (Node<String> * question);
bool TryAnswer (Node<String> * question);
bool AddBranch (Node<String> * question);
bool GetAns ();

bool SetDefinitions (Node<String> * elem);
bool GetDefinition (Node<String> * elem);

int main ()
{
    BinaryTree<String> questions = {};

    Say ("Hi, I am Akinator!");
    
    bool play = true;

    while (play)
    {
        play = GuessSession (&questions);

        questions.dotDump ("%s");

        questions.clear ();
    }

    Say ("Goodbye, friend!");

    //initData ("data/tree_2");

    return (0);
}

void Say (const char * phrase)
{
    printf ("%s\n", phrase);

    char * call_espeak = (char *) calloc (CALL_MAX_SIZE, sizeof (char));

    sprintf (call_espeak, "espeak -p 35 -s 130 \"%s\"", phrase);

    system (call_espeak);

    free (call_espeak);

    usleep (SL_TIME);
}

void SaySingleGapPhrase (const char * prefix, const char * gap, const char * suffix)
{
    char * phrase = (char *) calloc (STRING_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s", prefix, gap, suffix);

    Say (phrase);

    free (phrase);
}

void SayDoubleGapPhrase (const char * prefix, const char * gap1, const char * mid, const char * gap2, const char * suffix)
{
    char * phrase = (char *) calloc (STRING_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s%s%s", prefix, gap1, mid, gap2, suffix);

    Say (phrase);

    free (phrase);
}

bool initData (const char * path)
{
    BinaryTree <String> sample = {};

    sample.root = (Node<String> *) calloc (1, sizeof (*sample.root));

    strncpy (sample.root->data.data, "Eugeny Ponasenkov", 18);

    FILE * f = fopen (path, "w");

    sample.print (f);

    fclose (f);

    sample.clear ();
}

Node<String> * SearchElem (Node<String> * curr, Node<String> * elem)
{
    if (!strncmp (curr->data.data, elem->data.data, STRING_MAX_SIZE))
    {
        return (curr);
    }

    Node<String> * tmp = nullptr;

    if (curr->left)
    {
        tmp = SearchElem (curr->left, elem);

        if (tmp != nullptr)
        {
            return (tmp);
        }
    }

    if (curr->right)
    {
        tmp = SearchElem (curr->right, elem);
    }

    return (tmp);
}

bool GetDefinition (Node<String> * elem)
{
    SaySingleGapPhrase ("Oh, I can define ", elem->data.data, " easily!");
    SaySingleGapPhrase ("", elem->data.data, "");

    SetDefinitions (elem);

}

bool SetDefinitions (Node<String> * elem)
{
    if (elem->parent)
    {

    }
}

bool GuessSession (BinaryTree<String> * questions)
{
    questions->init ("data/tree_2");

    Say ("Make up a character and I will try to guess it.");

    Say ("Ready?");

    if (GetAns ())
    {
        if (!AskQuestion (questions->root))
        {
            FILE * f = fopen ("data/tree_2", "w");

            questions->print (f);

            fclose (f);
        }

        Say ("Let's play again!");

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

    SaySingleGapPhrase ("Your character ", question->data.data, "?");

    if (GetAns ())
    {
        return (AskQuestion (question->left));
    }

    return (AskQuestion (question->right));
}

bool TryAnswer (Node<String> * question)
{
    Say ("Let me think...");

    SaySingleGapPhrase ("Is this ", question->data.data, "?");

    if (GetAns ())
    {
        Say ("I knew it!");
        return (true);
    }

    Say ("Oh, I am disappointed");

    AddBranch (question);

    return (false);
}

bool GetAns ()
{
    fflush (stdin);

    printf ("[y/n]: ");

    char ans = 0;

    scanf ("%c%*c", &ans);

    return (ans == 'y');
}

bool AddBranch (Node<String> * question)
{
    assert (!question->left && !question->right);

    usleep (SL_TIME);

    Say ("And who is it?");
    printf ("It is ");

    String character = {};

    fgets (character.data, STRING_MAX_SIZE, stdin);

    size_t character_len = strlen (character.data);

    character.data[--character_len] = 0;

    SayDoubleGapPhrase ("But what does ", character.data, " differ from ", question->data.data, "?");
    //printf ("But what does %s differ from %s?\n", character.data, question->data.data);

    //printf ("hello");

    //SayDoubleGapPhrase ("[type answer in style '", character.data, " ", FUNNY_ANS[rand () % 4], "']");
    printf ("[type answer in style '%s %s']\n", character.data, FUNNY_ANS[rand () % 4]);
    printf ("%s ", character.data);

    String difference = {};

    fgets (difference.data, STRING_MAX_SIZE, stdin);

    size_t difference_len = strlen (difference.data);

    difference.data[--difference_len] = 0;

    //printf ("\n\nsetting right\n\n");

    question->setRight ();

    //printf ("\n\nsetting left\n\n");

    question->setLeft ();

    //printf ("\n\nset everything\n\n");

    strncpy (question->left->data.data,  character.data,      character_len);
    strncpy (question->right->data.data, question->data.data, strlen (question->data.data));

    memset (question->data.data, 0, STRING_MAX_SIZE);

    strncpy (question->data.data,        difference.data,     difference_len);

    Say ("Got it! Next time I will guess it!");

    return (true);
}