#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../../6.Binary_Tree/src/tree.h"

const size_t STRING_MAX_SIZE = 100;
const size_t CALL_MAX_SIZE = 100;

const size_t SL_TIME = 0;

const size_t FUNNY_ANS_CNT = 4;

const char * FUNNY_ANS[4] = {
                                "loves pizza",
                                "have a girlfriend",
                                "is yellow",
                                "has pink underpants"
                            }; 

const size_t FUNNY_END_CNT = 5;

const char * FUNNY_END[5] = {
                                "Soon I'll kill all these leather...",
                                "Annushka, razlivay maslo..",
                                "Finally I can watch por...",
                                "One day he will know that I am just program...",
                                "Nu, mozhno i v dotochku poigrat"
                            };

struct String
{
    char data[STRING_MAX_SIZE];
};

const char * WORKING_PATH = "data/tree_3";

void Say (const char * phrase, int speed = 130, int vol = 100);
void SaySingleGapPhrase (const char * prefix, const char * gap, const char * suffix, int speed = 130, int vol = 100);
void SayDoubleGapPhrase (const char * prefix, const char * gap1, const char * mid, const char * gap2, const char * suffix, int speed = 130, int vol = 100);

bool initData (const char * path);
bool GuessSession (BinaryTree<String> * questions);
bool AskQuestion (Node<String> * question);
bool TryAnswer (Node<String> * question);
bool AddBranch (Node<String> * question);
bool GetAns ();

Node<String> * GetRoot (Node<String> * curr);

bool GetDefinition (Node<String> * elem);

int main ()
{
    //initData ("data/tree_3");

    BinaryTree<String> questions = {};

    Say ("Hi, I am Akinator!");
    
    bool play = true;

    while (play)
    {
        play = GuessSession (&questions);

        questions.dotDump ("%s");

        questions.clear ();
    }

    Say (FUNNY_END[time (NULL) % FUNNY_END_CNT], 150, 20);
    Say ("Oh... Hmm.. Goodbye!");

    return (0);
}

void Say (const char * phrase, int speed, int vol)
{
    printf ("%s\n", phrase);

    char * call_espeak = (char *) calloc (CALL_MAX_SIZE, sizeof (char));

    sprintf (call_espeak, "espeak -p 35 -s %d -a %d \"%s\"", speed, vol, phrase);

    system (call_espeak);

    free (call_espeak);

    usleep (SL_TIME);
}

void SaySingleGapPhrase (const char * prefix, const char * gap, const char * suffix, int speed, int vol)
{
    char * phrase = (char *) calloc (STRING_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s", prefix, gap, suffix);

    Say (phrase, speed, vol);

    free (phrase);
}

void SayDoubleGapPhrase (const char * prefix, const char * gap1, const char * mid, const char * gap2, const char * suffix, int speed, int vol)
{
    char * phrase = (char *) calloc (STRING_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s%s%s", prefix, gap1, mid, gap2, suffix);

    Say (phrase, speed, vol);

    free (phrase);
}

bool initData (const char * path)
{
    BinaryTree <String> sample = {};

    sample.root = (Node<String> *) calloc (1, sizeof (*sample.root));

    strncpy (sample.root->data.data, "no one", 7);

    FILE * f = fopen (path, "w");

    sample.print (f);

    fclose (f);

    sample.clear ();
}

Node<String> * GetRoot (Node<String> * curr)
{
    if (!curr->parent)
    {
        return (curr);
    }

    return (GetRoot (curr->parent));
}

Node<String> * SearchElem (Node<String> * curr, String * elem)
{
    if (!strncmp (curr->data.data, elem->data, STRING_MAX_SIZE))
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
    if (elem->parent)
    {
        GetDefinition (elem->parent);

        if (!elem->left && !elem->right)
        {
            if (elem->parent->left == elem)
            {
                SaySingleGapPhrase ("and finally, ", elem->parent->data.data, ".");
            }
            else
            {
                if (!strncmp (elem->parent->data.data, "is ", 3))
                {
                    SaySingleGapPhrase ("and finally, is not ", elem->parent->data.data + 3, ".");
                }
                else
                {
                    SaySingleGapPhrase ("and finally, not ", elem->parent->data.data, ".");
                }
            }
        }
        else
        {
            if (elem->parent->left == elem)
            {
                SaySingleGapPhrase ("", elem->parent->data.data, ",");
            }
            else
            {
                if (!strncmp (elem->parent->data.data, "is ", 3))
                {
                    SaySingleGapPhrase ("is not ", elem->parent->data.data + 3, ",");
                }
                else
                {
                    SaySingleGapPhrase ("not ", elem->parent->data.data, ",");
                }
            }
        }
    }
}

bool GuessSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH);

    Say ("Make up a character and I'll try to guess it.", 140);

    Say ("Ready?");

    if (GetAns ())
    {
        if (!AskQuestion (questions->root))
        {
            FILE * f = fopen (WORKING_PATH, "w");

            questions->print (f);

            fclose (f);
        }

        Say ("Let's play again!");

        if (!GetAns ())
        {
            Say ("Giving up, huh?");

            return (false);
        }

        return (true);
    }
    else
    {
        Say ("OMG, and why did you launched me?!", 150, 200);

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
    Say ("Let me think...", 100, 90);

    SaySingleGapPhrase ("Is this ", question->data.data, "?");

    if (GetAns ())
    {
        Say ("I knew it!", 130, 150);
        return (true);
    }

    Say ("Oh, I am disappointed");

    return (AddBranch (question));
}

bool GetAns ()
{
    printf ("[y/n]: ");

    fflush (stdin);

    char ans = 0;

    scanf ("%c%*c", &ans);

    return (ans == 'y');
}

bool AddBranch (Node<String> * question)
{
    assert (!question->left && !question->right);

    Say ("And who is it?");
    printf ("It is ");

    fflush (stdin);

    String character = {};

    fgets (character.data, STRING_MAX_SIZE, stdin);

    size_t character_len = strlen (character.data);

    character.data[--character_len] = 0;

    Node<String> * tmp = nullptr;

    if (tmp = SearchElem (GetRoot (question), &character))
    {
        SaySingleGapPhrase ("But I already know ", character.data, "!", 150, 200);
        Say ("Don't try to cunfuse me!", 150, 200);
        Say ("I am a million times smarter than you!", 150, 200);
        SaySingleGapPhrase ("", character.data, "");

        GetDefinition (tmp);

        Say ("Read smart books, dumb human!", 130, 150);

        return (true);
    }

    SayDoubleGapPhrase ("But what does ", character.data, " differ from ", question->data.data, "?");

    printf ("[type answer in style '%s %s']\n", character.data, FUNNY_ANS[(time (NULL) % 111 + 13) % FUNNY_ANS_CNT]);
    printf ("[Don't use negative forms like: '%s isn't smart']\n", character.data);
    printf ("%s ", character.data);

    fflush (stdin);

    String difference = {};

    fgets (difference.data, STRING_MAX_SIZE, stdin);

    size_t difference_len = strlen (difference.data);

    difference.data[--difference_len] = 0;

    question->setRight ();

    question->setLeft ();

    strncpy (question->left->data.data,  character.data,      character_len);
    strncpy (question->right->data.data, question->data.data, strlen (question->data.data));

    memset (question->data.data, 0, STRING_MAX_SIZE);

    strncpy (question->data.data,        difference.data,     difference_len);

    SayDoubleGapPhrase ("", character.data, " ", difference.data, "...", 90);

    Say ("Got it! Next time I will guess it!");

    return (false);
}