#ifndef NODE_METHODS_H_
#define NODE_METHODS_H_

#include <stdio.h>
#include <stdlib.h>

#include "tree_class.h"

template <typename elem_t>
bool Node<elem_t>::init (const elem_t data)
{
    this->data = data;

    this->left  = nullptr;
    this->right = nullptr;

    this->parent = nullptr;

    return (true);
}

template <typename elem_t>
bool Node<elem_t>::clear ()
{
    this->data = 0;

    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;

    return (true);
}

template <typename elem_t>
bool Node<elem_t>::print (FILE * log)
{
    fprintf (log, "{%d", this->data);

    if (this->left)
    {
        this->left->print (log);
    }
    else if (this->right)
    {
        fprintf (log, "n");
    }

    if (this->right)
    {
        this->right->print (log);
    }

    fprintf (log, "}");
    
    return (true);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::addLeft (const elem_t data)
{
    this->left = (Node<elem_t> *) calloc (1, sizeof (*this->left));

    this->left->init (data);

    this->left->parent = this;

    return (this->left);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::addRight (const elem_t data)
{
    this->right = (Node<elem_t> *) calloc (1, sizeof (*this->right));

    this->right->init (data);

    this->right->parent = this;

    return (this->right);
}

template <typename elem_t>
bool Node<elem_t>::deleteList ()
{
    if (this->left || this->right)
    {
        return (false);
    }

    if (this->parent->right == this)
    {
        this->parent->right = nullptr;
    }
    else
    {
        this->parent->left  = nullptr;
    }

    this->clear();
    free (this);

    return (true);
}

#endif // !NODE_METHODS_H_


