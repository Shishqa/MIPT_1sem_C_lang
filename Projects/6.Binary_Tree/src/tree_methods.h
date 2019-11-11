#ifndef TREE_METHODS_H_
#define TREE_METHODS_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../../../Include/ms_stream.h"

#include "tree_class.h"

template <typename elem_t>
bool BinaryTree<elem_t>::init (const elem_t data)
{
    setNode (&this->root);

    this->root->init (data);

    this->dump_cnt = 0;

    return (true);
}

template <typename elem_t>
bool BinaryTree<elem_t>::init (const char * path)
{
    FILE* f = fopen (path, "r");

    assert (f != nullptr);

    char * buffer = nullptr;

    size_t sof_code = Read (&buffer, f);

    fclose (f);

    char * cur = buffer;

    assert (cur != nullptr);

    if (*cur != '{' || sof_code < sizeof (elem_t) + 2)
    {
        return (false);
    }

    setNode (&this->root);

    initSubtree (this->root, cur);
}

template <typename elem_t>
char * BinaryTree<elem_t>::initSubtree (Node<elem_t> * node, char* cur)
{
    if (cur == nullptr)
    {
        return (nullptr);
    }

    cur += 1;

    memcpy (&(node->data), cur, sizeof (node->data));

    cur += sizeof (node->data);

    if (*cur == '{')
    {
        node->setLeft ();

        assert (node->left->parent == node);

        cur = initSubtree (node->left, cur);

        if (*cur == '{')
        {
            node->setRight ();

            assert (node->right->parent == node);

            cur = initSubtree (node->right, cur);
        }
    }
    else if (*cur == '$')
    {
        if (*(cur + 1) != '{')
        {
            printf ("expected { after $\n");
            return (nullptr);
        }

        node->setRight ();

        assert (node->right->parent == node);

        cur = initSubtree (node->right, ++cur);
    }

    if (*cur != '}')
    {
        printf ("unclosed braces: %d\n", *cur);
        return (nullptr);
    }

    return (cur + 1);
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
    this->deleteSubtree (this->root);
}

template <typename elem_t>
bool BinaryTree<elem_t>::deleteList (Node<elem_t> * node)
{
    if (node->left || node->right)
    {
        return (false);
    }

    if (node->parent->right == node)
    {
        node->parent->right = nullptr;
    }
    else
    {
        node->parent->left  = nullptr;
    }

    free (node);

    return (true);
}

template <typename elem_t>
bool BinaryTree<elem_t>::deleteSubtree (Node<elem_t> * node)
{
    if (node->left)
    {
        this->deleteSubtree (node->left);
    }

    if (node->right)
    {
        this->deleteSubtree (node->right);
    }

    this->deleteList (node);

    return (true);
}

template <typename elem_t>
bool BinaryTree<elem_t>::print (FILE * log)
{
    this->root->print (log);

    return (true);
}

template <typename elem_t>
bool BinaryTree<elem_t>::print (FILE * log, const char * format)
{
    this->root->fprint (log, format);

    return (true);
}

template <typename elem_t>
Node<elem_t> * BinaryTree<elem_t>::search (const elem_t data)
{
    return (this->rec_search (this->root, data));
}

template <typename elem_t>
Node<elem_t> * BinaryTree<elem_t>::rec_search (Node<elem_t> * node, const elem_t data)
{
    Node<elem_t> * res = nullptr;

    if (node->data == data)
    {
        return (node);
    }

    if (node->left)
    {
        res = this->rec_search (node->left, data);

        if (res)
        {
            return (res);
        }
    }

    if (node->right)
    {
        res = this->rec_search (node->right, data);

        if (res)
        {
            return (res);
        }
    }

    return (res);
}

template <typename elem_t>
bool BinaryTree<elem_t>::dotDump (const char * format)
{
    system("mkdir -p tmp");

	FILE *log = fopen("tmp/tempautogeneratedlog.gv", "w");

    fprintf (log, "digraph dump\n{\n");

    this->root->printDot (log, format);

    fprintf (log, "\n}\n");

    fclose(log);

	system("mkdir -p log");

	char call_dot[100] = "";

	sprintf(call_dot, "dot -Tpng -olog/dump%d.png tmp/tempautogeneratedlog.gv", ++this->dump_cnt);

	system(call_dot);

    return (true);
}

#endif // !TREE_METHODS_H_