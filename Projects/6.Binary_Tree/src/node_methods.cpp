#include <stdio.h>
#include <stdlib.h>
#include "../include/tree.h"

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
    if (this->left  != nullptr ||
        this->right != nullptr)
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