#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "../../6.Binary_Tree/src/tree.h"

bool GuessSession (BinaryTree<int> * questions);

int main ()
{
    BinaryTree<int> questions = {};

    printf ("Привет, я Акинатор!\n");
    
    bool quit = false;

    while (!quit)
    {
        GuessSession (&questions);

        quit = true;
    }

    return (0);
}

bool GuessSession (BinaryTree<int> * questions)
{
    questions->init ("../base/questions");

    printf ("Задумайте реального или вымышленного персонажа, а я попытаюсь его угадать!\n");
}