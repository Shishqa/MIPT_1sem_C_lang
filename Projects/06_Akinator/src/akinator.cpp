#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <linux/limits.h>

#include "tree.h"

#include "my_string.h"
#include "readers.h"
#include "phrases.h"

const size_t CALL_MAX_SIZE = 300;
const size_t PHRASE_MAX_SIZE = 300;

void Say (const char * phrase, 
          int speed = 140, 
          int vol = 100);
void Say (const char * prefix, 
          const char * gap, 
          const char * suffix, 
          int speed = 140, 
          int vol = 100);
void Say (const char * prefix, 
          const char * gap1, 
          const char * mid, 
          const char * gap2, 
          const char * suffix, 
          int speed = 140, 
          int vol = 100);

void flush ();

int  SILENT = 0;
char WORKING_PATH[PATH_MAX]  = {};

char SUBJ[STRING_MAX_SIZE]   = {};
char SINGLE[STRING_MAX_SIZE] = {};

enum modes
{
    GUESS_MODE,
    DEF_MODE,
    DIFF_MODE,
    SET_MODE,
    QUIT,
    SECRET
};

const char * MODE_NAMES[6] = {
    "guess",
    "def",
    "comp",
    "set",
    "quit",
    "$$$"
};

void initAkinator ();
void saveConfig ();

bool initData ();
size_t GetLine (char * to);

bool printData (BinaryTree<String> * questions);

int AskMode ();

const char ADM_PSW[STRING_MAX_SIZE] = "0110";

void Setup (BinaryTree<String> * questions);
bool AskPass ();

void DefSession (BinaryTree<String> * questions);
bool GetDef (BinaryTree<String> * questions);

void DiffSession (BinaryTree<String> * questions);
bool GetDiff (BinaryTree<String> * questions);
Node<String> * GetCross (Node<String> * curr, Node<String> * elem1, Node<String> * elem2);

bool GuessSession (BinaryTree<String> * questions);
bool AskQuestion (Node<String> * question);
bool TryAnswer (Node<String> * question);
bool AddBranch (Node<String> * question);
void SetQuestion (Node<String> * question, char * new_char, size_t char_len, 
                                           char * diff,     size_t diff_len);
bool GetAns ();

void EndSession ();

Node<String> * GetRoot (Node<String> * curr);

bool GetDefinition (Node<String> * elem, Node<String> * stop_parent = nullptr, Node<String> * start_node = nullptr);
Node<String> * SearchElem (Node<String> * curr, String * elem);

int main ()
{
    srand (time (NULL));

    BinaryTree<String> questions = {};

    initAkinator ();

    Say ("Hi, I am Akinator!\n");

    int mode = 0;

    while (true)
    {
        mode = AskMode ();

        system ("clear");

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
                if (!GuessSession (&questions))
                {
                    return (0);
                }

                questions.dotDump (printStrDot);

                break;
            }

            case DEF_MODE:
            {
                DefSession (&questions);

                break;
            }

            case DIFF_MODE:
            {
                DiffSession (&questions);

                break;
            }

            case SET_MODE:
            {
                Setup (&questions);

                break;
            }

            case SECRET:
            {
                Say ("That's the secret:\n");
                Say ("Clearing your disk in 5 seconds\n");
                Say ("1\n", 140);
                Say ("2\n", 120);
                Say ("3..\n", 110);
                Say ("4...\n", 80);
                Say ("Ha-ha, it's a joke!\n", 150, 200);
            }
            break;
        
            default:
            {
                return (0);
                break;
            }
        }
    }
}

void initAkinator ()
{
    FILE * conf = fopen ("config/config", "r");

    fscanf (conf, "SUBJ: \"%[^\"]\"\n", SUBJ);
    fscanf (conf, "SINGLE: \"%[^\"]\"\n", SINGLE);

    fscanf (conf, "SILENT: %d\n", &SILENT);

    fscanf (conf, "WORKING_PATH: \"%[^\"]\"", WORKING_PATH);

    fclose (conf);
}

void saveConfig ()
{
    FILE * conf = fopen ("config/config", "w");

    fprintf (conf, "SUBJ: \"%s\"\n", SUBJ);
    fprintf (conf, "SINGLE: \"%s\"\n", SINGLE);

    fprintf (conf, "SILENT: %d\n", SILENT);

    fprintf (conf, "WORKING_PATH: \"%s\"\n", WORKING_PATH);

    fprintf (conf, "\n#don't change the order of lines!");

    fclose (conf);
}

bool printData (BinaryTree<String> * questions)
{
    FILE * f = fopen (WORKING_PATH, "w");

    questions->print (f, 'p', printStr);

    fclose (f);
}

int AskMode ()
{
    Say ("What do you want now?\n");

    while (true)
    {
        printf ("[%s]\t  for guessing %s\n", MODE_NAMES[GUESS_MODE], SUBJ);
        printf ("[%s]\t  for defining %s\n", MODE_NAMES[DEF_MODE], SUBJ);
        printf ("[%s]\t  for telling difference between two %ss\n", MODE_NAMES[DIFF_MODE], SUBJ);
        printf ("[%s]\t  for configurations\n", MODE_NAMES[SET_MODE]);
        printf ("[%s]\t  for secret thing\n", MODE_NAMES[SECRET]);
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
        else if (!strncmp (decision, MODE_NAMES[SECRET], sizeof (MODE_NAMES[SECRET])))
        {
            return (SECRET);
        }
        else
        {
            Say ("I can't understand you.\n");
        }
    }
}

bool AskPass ()
{
    printf ("password: ");

    char psw[STRING_MAX_SIZE] = {};

    scanf ("%s", psw);
    flush ();

    return (!strncmp (psw, ADM_PSW, 4));
}

void Setup (BinaryTree<String> * questions)
{
    Say ("Trying to hack me?\n");
    Say ("Enter password first!\n");

    if (!AskPass ())
    {
        Say ("Access denied!\n");
        return;
    }

    int c = 0;
    char path[PATH_MAX] = {};

    while (c != 'q')
    {
        printf (">");

        scanf ("%c", &c);

        flush ();

        switch (c)
        {
            case 'i':
            {
                initData ();

                printf ("New tree created at %s\n", WORKING_PATH);
            }
            break;

            case 'w':
            {
                scanf ("%s", WORKING_PATH);
                flush ();

                printf ("Working path is now %s\n", WORKING_PATH);
            }
            break;

            case 's':
            {
                SILENT = (SILENT + 1) % 2;
                printf ("Akinator is now %s\n", ((SILENT) ? "silent" : "not silent"));
            }
            break;

            case 'v':
            {
                questions->init (WORKING_PATH, readStr);

                questions->dotDump (printStrDot, -1);

                system ("xdg-open 'log/dump-1.png'");

                questions->clear ();

                sleep (2);

                system ("clear");
            }
            break;

            case 'q':
            {
                break;
            }
            break;
            
            default:
            {
                printf ("Unknown command\n");
            }
            break;
        }
    }

    saveConfig ();
}

void DefSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH, readStr);

    bool play = true;

    while (GetDef (questions));

    Say ("So...", 100);
}

Node<String> * GetCross (Node<String> * curr, Node<String> * elem1, Node<String> * elem2)
{
    Node <String> * tmp1 = SearchElem (curr->left, &elem1->data);
    Node <String> * tmp2 = SearchElem (curr->left, &elem2->data);

    if ((!tmp1 && tmp2) || (tmp1 && !tmp2))
    {
        return (curr);
    }
    else if (tmp1 && tmp2)
    {
        return (GetCross (curr->left, elem1, elem2));
    }

    return (GetCross (curr->right, elem1, elem2));
}

bool GetDiff (BinaryTree<String> * questions)
{
    Say ("What two ", SUBJ, "s you want to compare?\n");

    String char1 = {};
    String char2 = {};

    printf (">");
    GetLine (char1.data);

    printf (">");
    GetLine (char2.data);

    Say (char1.data, " and ", char2.data, "", "...\n", 100);

    if (!strncmp (char1.data, char2.data, STRING_MAX_SIZE))
    {
        Say ("But they are similar!\n");

        Say ("That's all?\n");

        return (!GetAns ());
    }

    Node<String> * ch1 = nullptr;
    Node<String> * ch2 = nullptr;

    if (!(ch1 = SearchElem (questions->root, &char1)))
    {
        Say ("I don't know ", SINGLE, " is ", char1.data, ".\n");

        return (false);
    }
    else if (!(ch2 = SearchElem (questions->root, &char2)))
    {
        Say ("I don't know ", SINGLE, " is ", char2.data, ".\n");

        return (false);
    }

    Node<String> * cross = GetCross (questions->root, ch1, ch2);

    Say ("the cross is ", cross->data.data, "\n");

    Say ("I can tell the difference between them easily!\n");

    Say (char1.data, "", " ");

    GetDefinition (ch1, cross->parent);

    Say ("On the contrary,\n");

    Say (char2.data, "", " ");

    GetDefinition (ch2, cross->parent);

    if (cross != questions->root)
    {
        Say ("But they both have such characteristics as\n");

        GetDefinition (cross);
    }
    else
    {
        Say ("And they don't have similar characteristics.\n");
    }

    Say ("That's all?\n");

    return (!GetAns ());
}


void DiffSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH, readStr);

    bool play = true;

    while (GetDiff (questions));

    Say ("So...", 100);
}

bool GetDef (BinaryTree<String> * questions)
{
    Say ("What ", SUBJ, " do you want to know?\n");

    printf ("I want to know about ");

    String element = {};
    size_t element_len = GetLine (element.data);

    Say ("", element.data, "...\n", 110, 100);

    Node<String> * tmp = nullptr;

    if (tmp = SearchElem (questions->root, &element))
    {
        Say ("Obviously I know ", SINGLE, " it is!\n");

        Say (element.data, " ", "");

        GetDefinition (tmp);
    }
    else
    {
        Say ("I assume, this ", SUBJ, " doesn't exist\n");
    }

    Say ("That's all?\n");

    return (!GetAns());
}

void EndSession ()
{
    Say (FUNNY_END[rand () % FUNNY_END_CNT], "", "\n", 150, 20);
    Say ("Oh... Hmm.. Goodbye!\n");
}

bool initData ()
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

bool GetDefinition (Node<String> * elem, Node<String> * stop_parent, Node<String> * start_node)
{
    if (elem->parent != stop_parent)
    {
        if (!start_node)
        {
            GetDefinition (elem->parent, stop_parent, elem);
        }
        else
        {
            GetDefinition (elem->parent, stop_parent, start_node);
        }

        if (!start_node && elem->parent->parent != stop_parent)
        {
            Say (CONNECT_END[rand () % CONNECT_END_CNT]);
        }
        else if (elem->parent->parent != stop_parent)
        {
            Say (CONNECT[rand () % CONNECT_CNT], 120);
        }

        if (elem->parent->left == elem)
        {
            Say (elem->parent->data.data);
        }
        else
        {
            if (!strncmp (elem->parent->data.data, "is ", 3))
            {
                Say ("is not ", elem->parent->data.data + 3, "");
            }
            else
            {
                Say ("not ", elem->parent->data.data, "");
            }
        }

        if (!start_node)
        {
            printf (".\n");
        }
        else
        {
            printf (", ");
        }
    }
}

bool GuessSession (BinaryTree<String> * questions)
{
    questions->init (WORKING_PATH, readStr);

    bool play = true;

    while (play)
    {
        Say ("Make up a ", SUBJ, " and I'll try to guess it.\n", 140);

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

                return (false);
            }

            Say ("Let's play again!\n");

            play = GetAns();
        }
        else
        {
            Say ("OMG, and why did you launched me?!\n", 150, 200);

            return (true);
        }
    }

    Say ("Giving up, huh?\n");

    return (true);
}

bool AskQuestion (Node<String> * question)
{
    assert (question != nullptr);

    if (!question->left && !question->right)
    {
        return (TryAnswer (question));
    }

    Say ("Your ", SUBJ, " ", question->data.data, "?\n");

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

bool AddBranch (Node<String> * question)
{
    assert (!question->left && !question->right);

    String character = {};
    size_t character_len = 0;

    String difference = {};
    size_t difference_len = 0;

    Node<String> * tmp = nullptr;

    size_t try_cnt = 0;

    while (++try_cnt)
    {
        character = {};

        Say ("", SINGLE, " is it?\n");
        printf ("It is ");

        character_len = GetLine (character.data);

        if (tmp = SearchElem (GetRoot (question), &character))
        {
            Say ("But I already know ", character.data, "!\n", 150, 200);
            Say ("Don't try to cunfuse me!\n", 150, 200);
            Say ("I am a million times smarter than you!\n", 150, 200);
            Say ("", character.data, " ");

            GetDefinition (tmp);

            Say ("Read smart books, dumb human!\n", 130, 150);

            return (false);
        }

        Say ("But what does ", character.data, " differ from ", question->data.data, "?\n");

        printf ("[type answer in style '%s %s']\n", character.data, FUNNY_ANS[rand () % FUNNY_ANS_CNT]);
        printf ("[Don't use negative forms like: '%s isn't smart']\n", character.data);
        printf ("%s ", character.data);

        difference = {};

        difference_len = GetLine (difference.data);

        printf ("\n");

        Say ("OK, you're trying to say that ", character.data, " ", difference.data, "...\n", 100);

        printf ("\n");

        Say ("Are you sure?\n");

        if (!GetAns ())
        {
            if (try_cnt == 2)
            {
                Say ("Why are you always unsure?!\n", 150, 150);
                Say ("Do you think that I can wait permanently?!\n", 150, 200);

                return (true);
            }

            continue;
        }
        else
        {
            break;
        }
    }   

    SetQuestion (question, character.data, character_len, difference.data, difference_len);

    Say ("Ah, right, I knew it, ", FUNNY_RSN[rand () % FUNNY_RSN_CNT], "\n");

    return (false);
}

void SetQuestion (Node<String> * question, char * new_char, size_t char_len, char * diff, size_t diff_len)
{
    question->setRight ();

    question->setLeft ();

    strncpy (question->left->data.data, new_char, char_len);
    strncpy (question->right->data.data, question->data.data, strlen (question->data.data));

    memset (question->data.data, 0, STRING_MAX_SIZE);

    strncpy (question->data.data, diff, diff_len);
}

void Say (const char * phrase, int speed, int vol)
{
    printf ("%s", phrase);

    fflush (stdout);

    char * call_espeak = (char *) calloc (CALL_MAX_SIZE, sizeof (char));

    sprintf (call_espeak, "espeak %s -ven+m3 -p 70 -s %d -a %d \"%s\"", ((SILENT) ? "-q" : ""),
             speed, vol, phrase);

    system (call_espeak);

    free (call_espeak);
}

void Say (const char * prefix, const char * gap, const char * suffix, int speed, int vol)
{
    char * phrase = (char *) calloc (PHRASE_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s", prefix, gap, suffix);

    Say (phrase, speed, vol);

    free (phrase);
}

void Say (const char * prefix, const char * gap1, const char * mid, const char * gap2, const char * suffix, int speed, int vol)
{
    char * phrase = (char *) calloc (PHRASE_MAX_SIZE, sizeof (char));

    sprintf (phrase, "%s%s%s%s%s", prefix, gap1, mid, gap2, suffix);

    Say (phrase, speed, vol);

    free (phrase);
}

void flush ()
{
    char c = 0;
    while ((c = getchar ()) != '\n' && c != EOF);
}
