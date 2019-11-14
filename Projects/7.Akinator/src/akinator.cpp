#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../../6.Binary_Tree/src/tree.h"

#include "my_string.h"
#include "readers.h"
#include "phrases.h"
#include "speech.h"

const char * WORKING_PATH = "data/tree_4";

enum modes
{
    GUESS_MODE,
    DEF_MODE,
    DIFF_MODE,
    SET_MODE,
    QUIT
};

const char * MODE_NAMES[5] = {
    "guess",
    "def",
    "dif",
    "set",
    "quit"
};

bool initData (const char * path);
size_t GetLine (char * to);

bool printData (BinaryTree<String> * questions);

int AskMode ();

size_t AskCharacter (String * ch);

void DefSession (BinaryTree<String> * questions);
bool GetDef (BinaryTree<String> * questions);

void GuessSession (BinaryTree<String> * questions);
bool AskQuestion (Node<String> * question);
bool TryAnswer (Node<String> * question);
bool AddBranch (Node<String> * question);
bool GetAns ();

void EndSession ();

Node<String> * GetRoot (Node<String> * curr);

bool GetDefinition (Node<String> * elem);
Node<String> * SearchElem (Node<String> * curr, String * elem);

int main ()
{
    BinaryTree<String> questions = {};

    Say ("Hi, I am Akinator!\n");

    int mode = 0;

    while (true)
    {
        mode = AskMode ();

        switch (mode)
        {
            case QUIT:
            {
                EndSession ();

                return (0);

                break;
            }

            case GUESS_MODE:
            {
                GuessSession (&questions);

                break;
            }

            case DEF_MODE:
            {
                DefSession (&questions);

                break;
            }

            case DIFF_MODE:
            {
                GuessSession (&questions);

                break;
            }

            case SET_MODE:
            {
                GuessSession (&questions);

                break;
            }
        
            default:
            {
                return (0);
                break;
            }
        }
    }
}

bool printData (BinaryTree<String> * questions)
{
    FILE * f = fopen (WORKING_PATH, "w");

    questions->print (f, printStr);

    fclose (f);
}

int AskMode ()
{
    Say ("What do you want from me?\n");

    printf ("[%s]\t  for guessing character\n", MODE_NAMES[GUESS_MODE]);
    printf ("[%s]\t  for defining character\n", MODE_NAMES[DEF_MODE]);
    printf ("[%s]\t  for telling difference between two characters\n", MODE_NAMES[DIFF_MODE]);
    printf ("[%s]\t  for configurations\n", MODE_NAMES[SET_MODE]);
    printf ("[%s]\t  for quit\n", MODE_NAMES[QUIT]);

    printf (">");

    char * decision = (char *) calloc (STRING_MAX_SIZE, sizeof (*decision));

    scanf ("%s", decision);
    flush ();

    if (!strncmp (decision, MODE_NAMES[GUESS_MODE], sizeof (MODE_NAMES[GUESS_MODE])))
    {
        return (GUESS_MODE);
    }
    else if (!strncmp (decision, MODE_NAMES[DEF_MODE], sizeof (MODE_NAMES[DEF_MODE])))
    {
        return (DEF_MODE);
    }
    else if (!strncmp (decision, MODE_NAMES[DIFF_MODE], sizeof (MODE_NAMES[DIFF_MODE])))
    {
        return (DIFF_MODE);
    }
    else if (!strncmp (decision, MODE_NAMES[SET_MODE], sizeof (MODE_NAMES[SET_MODE])))
    {
        return (SET_MODE);
    }
    else if (!strncmp (decision, MODE_NAMES[QUIT], sizeof (MODE_NAMES[QUIT])))
    {
        return (QUIT);
    }
    else
    {
        Say ("I can't understand you.\n");
        return (AskMode ());
    }
}

void DefSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH, readStr);

    bool play = true;

    while (GetDef (questions));

    Say ("So...", 100);
}

void DiffSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH, readStr);
}

bool GetDef (BinaryTree<String> * questions)
{
    String element = {};

    Say ("What character do you want to know?\n");

    printf ("I want to know about ");

    size_t element_len = GetLine (element.data);

    Say ("", element.data, "...\n", 110, 100);

    Node<String> * tmp = nullptr;

    if (tmp = SearchElem (questions->root, &element))
    {
        Say ("Obviously I know who it is!\n");
        Say ("", element.data, " ");

        GetDefinition (tmp);
    }
    else
    {
        Say ("Unfortunately, this character doesn't exist\n");
    }

    Say ("That's all?\n");

    return (!GetAns());
}

void EndSession ()
{
    Say (FUNNY_END[time (NULL) % FUNNY_END_CNT], "", "\n", 150, 20);
    Say ("Oh... Hmm.. Goodbye!\n");
}

bool initData (const char * path)
{
    BinaryTree <String> sample = {};

    sample.root = (Node<String> *) calloc (1, sizeof (*sample.root));

    strncpy (sample.root->data.data, "no one", 7);

    printData (&sample);

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
            if (elem->parent->parent)
            {
                Say ("and, finally, ");
            }

            if (elem->parent->left == elem)
            {
                Say ("", elem->parent->data.data, ".\n");
            }
            else
            {
                if (!strncmp (elem->parent->data.data, "is ", 3))
                {
                    Say ("is not ", elem->parent->data.data + 3, ".\n");
                }
                else
                {
                    Say ("not ", elem->parent->data.data, ".\n");
                }
            }
        }
        else
        {
            if (elem->parent->left == elem)
            {
                Say ("", elem->parent->data.data, ",");
            }
            else
            {
                if (!strncmp (elem->parent->data.data, "is ", 3))
                {
                    Say ("is not ", elem->parent->data.data + 3, ",");
                }
                else
                {
                    Say ("not ", elem->parent->data.data, ",");
                }
            }
        }
    }
}

void GuessSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH, readStr);

    bool play = true;

    while (play)
    {
        Say ("Make up a character and I'll try to guess it.\n", 140);

        Say ("Ready?\n");

        if (GetAns ())
        {
            if (!AskQuestion (questions->root))
            {
                printData (questions);
            }
            else
            {
                Say ("I don't want to deal with you anymore!\n", 150, 200);

                play = false;
            }

            Say ("Let's play again!\n");

            play = GetAns();
        }
        else
        {
            Say ("OMG, and why did you launched me?!\n", 150, 200);

            return;
        }
    }

    Say ("Giving up, huh?\n");
}

bool AskQuestion (Node<String> * question)
{
    assert (question != nullptr);

    if (!question->left && !question->right)
    {
        return (TryAnswer (question));
    }

    Say ("Your character ", question->data.data, "?\n");

    if (GetAns ())
    {
        return (AskQuestion (question->left));
    }

    return (AskQuestion (question->right));
}

bool TryAnswer (Node<String> * question)
{
    Say ("Let me think...\n", 100, 90);

    Say ("Is this ", question->data.data, "?\n");

    if (GetAns ())
    {
        Say ("I knew it!\n", 130, 150);
        return (false);
    }

    Say ("Oh, I am disappointed\n");

    return (AddBranch (question));
}

bool GetAns ()
{
    printf ("[y/n]: ");

    char ans = 0;

    scanf ("%c", &ans);
    flush ();

    return (ans == 'y');
}

size_t GetLine (char * to)
{
    fgets (to, STRING_MAX_SIZE, stdin);

    size_t len = strlen (to);

    to[--len] = 0;

    return (len);
}

size_t AskCharacter (String * ch)
{
    Say ("Who is it?\n");
    printf ("It is ");

    return (GetLine (ch->data));
}

bool AddBranch (Node<String> * question)
{
    assert (!question->left && !question->right);

    String character = {};
    size_t character_len = 0;

    Node<String> * tmp = nullptr;

    size_t try_cnt = 1;

    while (try_cnt++)
    {
        Say ("And who is it?", 130 - 20 * (try_cnt - 1), 100 + 30 * (try_cnt - 1));
        printf ("It is ");

        character = {};

        

        if (tmp = SearchElem (GetRoot (question), &character))
        {
            Say ("But I already know ", character.data, "!\n", 150, 200);
            Say ("Don't try to cunfuse me!\n", 150, 200);
            Say ("I am a million times smarter than you!\n", 150, 200);
            Say ("", character.data, "\n");

            GetDefinition (tmp);

            Say ("Read smart books, dumb human!\n", 130, 150);

            return (false);
        }

        Say ("But what does ", character.data, " differ from ", question->data.data, "?\n");

        printf ("[type answer in style '%s %s']\n", character.data, FUNNY_ANS[(time (NULL) % 111 + 13) % FUNNY_ANS_CNT]);
        printf ("[Don't use negative forms like: '%s isn't smart']\n", character.data);
        printf ("%s ", character.data);

        String difference = {};
        size_t difference_len = 0;

        difference = {};

        fgets (difference.data, STRING_MAX_SIZE, stdin);

        difference_len = strlen (difference.data);

        difference.data[--difference_len] = 0;

        Say ("", character.data, " ", difference.data, "...\n", 100);

        Say ("Are you sure?\n", 130 - 40 * (try_cnt - 1), 100 + 35 * (try_cnt - 1));

        if (!GetAns ())
        {
            if (try_cnt == 4)
            {
                Say ("Why are you always unsure?!\n", 150, 150);
                Say ("Do you think that I can wait permanently?!\n", 150, 200);

                return (true);
            }

            continue;
        }

        question->setRight ();

        question->setLeft ();

        strncpy (question->left->data.data,  character.data,      character_len);
        strncpy (question->right->data.data, question->data.data, strlen (question->data.data));

        memset (question->data.data, 0, STRING_MAX_SIZE);

        strncpy (question->data.data,        difference.data,     difference_len);
    }   

    Say ("Got it! Next time I will guess it!\n");

    return (false);
}
