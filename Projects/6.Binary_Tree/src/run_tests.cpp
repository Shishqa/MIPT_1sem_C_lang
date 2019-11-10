#include <stdio.h>
#include "tree.h"

int main ()
{
    BinaryTree<int> test = {};

    test.init (111);

    Node<int> * node1 = test.root->addLeft (12);
    Node<int> * node2 = test.root->addRight (125);

    node1->addLeft (228);
    node2->addRight (100);
    
    test.print (stdout);

    test.dotDump ();

    printf ("\n");

    test.deleteSubtree (node1);

    test.dotDump ();

    FILE * f = fopen ("test", "w"); 
    test.print (f);
    fclose (f);

    BinaryTree<int> test1 = {};

    test1.init ("test");

    return (0);
}