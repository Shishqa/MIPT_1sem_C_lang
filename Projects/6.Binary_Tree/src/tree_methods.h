#ifndef TREE_METHODS_H_
#define TREE_METHODS_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../../../Include/ms_stream.h"

#include "tree_class.h"

template <typename elem_t>
bool BinaryTree<elem_t>::init (const elem_t data)
{
    setNode (&this->root);

    this->root->init (data);

    return (true);
}

template <typename elem_t>
bool BinaryTree<elem_t>::init (const char * path)
{
    FILE* f = fopen (path, "r");

    char * buffer = nullptr;

    size_t sof_code = Read (&buffer, f);

    fclose (f);

    char * cur = buffer;
    size_t s_len = 0;

    int brac_cnt = 0;

    elem_t data = 0;
    char marker = 0;
    bool no_left = false;

    sscanf (cur, "%d%n", &data, &s_len);

    cur += s_len;

    setNode (&this->root);
    this->root->init (data);

    Node<elem_t> * curr_node   = nullptr;
    Node<elem_t> * curr_parent = this->root;

    while (sscanf (cur, "%c%n", &marker, &s_len) &&
            cur - buffer <= sof_code)
    {
        cur += s_len;

        if (marker == '{')
        {
            brac_cnt++;

            if (curr_parent->left || no_left)
            {
                curr_parent->addRight (0);
                curr_node = curr_parent->right;

                no_left = false;
            }
            else
            {
                curr_parent->addLeft (0);
                curr_node = curr_parent->left;
            }

            sscanf (cur, "%d%n", &data, &s_len);

            cur += s_len;

            curr_node->init (data);
            curr_node->parent = curr_parent;

            curr_parent = curr_node;
        }
        else if (marker == 'n')
        {
            curr_node = curr_parent->right;

            no_left = true;
        }
        else if (marker == '}')
        {
            brac_cnt--;

            curr_parent = curr_parent->parent;

            assert (curr_node != nullptr);
        }
        else
        {
            printf ("parsing error: %d | unknown marker: %c\n", cur - buffer, marker);
            return (false);
        }
    }

    return (true);
}

template <typename elem_t>
Node<elem_t> * BinaryTree<elem_t>::setNode (Node<elem_t> ** ptr)
{
    *ptr = (Node<elem_t> *) calloc (1, sizeof (Node<elem_t>));

    return (*ptr);
}

template <typename elem_t>
bool BinaryTree<elem_t>::clear ()
{
    return (true);
}

template <typename elem_t>
bool BinaryTree<elem_t>::print (FILE * log)
{
    fprintf (log, "%d", this->root->data);

    this->root->left->print (log);
    this->root->right->print (log);

    return (true);
}

#endif // !TREE_METHODS_H_