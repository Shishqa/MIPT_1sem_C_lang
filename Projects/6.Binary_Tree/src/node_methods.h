#ifndef NODE_METHODS_H_
#define NODE_METHODS_H_

#include <stdio.h>
#include <stdlib.h>

#include "tree_class.h"

template <typename elem_t>
bool Node<elem_t>::init ()
{
    this->left  = nullptr;
    this->right = nullptr;

    this->parent = nullptr;

    return (true);
}

template <typename elem_t>
bool Node<elem_t>::init (const elem_t data)
{
    this->init ();

    this->data = data;

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
    fprintf (log, "{");

    fwrite (&this->data, sizeof (this->data), 1, log);

    if (this->left)
    {
        this->left->print (log);
    }
    else if (this->right)
    {
        fprintf (log, "$");
    }

    if (this->right)
    {
        this->right->print (log);
    }

    fprintf (log, "}");
    
    return (true);
}

template <typename elem_t>
bool Node<elem_t>::fprint (FILE * log, const char * format)
{
    fprintf (log, "{");

    fprintf (log, format, this->data);

    if (this->left)
    {
        this->left->fprint (log, format);
    }
    else if (this->right)
    {
        fprintf (log, "$");
    }

    if (this->right)
    {
        this->right->fprint (log, format);
    }

    fprintf (log, "}");
    
    return (true);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::setChild ()
{
    Node<elem_t> * child = (Node<elem_t> *) calloc (1, sizeof (*this->left));

    child->init ();

    child->parent = this;

    return (child);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::addLeft (const elem_t data)
{
    this->setLeft ();

    this->left->data = data;

    return (this->left);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::setLeft ()
{
    this->left = this->setChild ();

    return (this->left);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::addRight (const elem_t data)
{
    this->setRight ();

    this->right->data = data;

    return (this->right);
}

template <typename elem_t>
Node<elem_t> * Node<elem_t>::setRight ()
{
    this->right = this->setChild ();

    return (this->right);
}

template <typename elem_t>
bool Node<elem_t>::printDot (FILE * log, const char * format)
{
    fprintf (log, "\tnode%p [shape = \"record\", label = \"{<f1>data: '", 
            this);

    fprintf (log, format, this->data);

    fprintf (log, "'|{<f2>left: %p|<f3>right: %p}}\"];\n", 
            this->left, this->right);

    if (this->left)
    {
        this->left->printDot (log, format);

        fprintf (log, "\tnode%p:f2 -> node%p\n", this, this->left);
    }

    if (this->right)
    {
        this->right->printDot (log, format);

        fprintf (log, "\tnode%p:f3 -> node%p\n", this, this->right);
    }
    
    return (true);
}

#endif // !NODE_METHODS_H_


