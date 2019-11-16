#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

Node<Monomial> * CreateNode (const int data, const char type, Node<Monomial> * right, Node<Monomial> * left)
{
    Node<Monomial> * node = (Node<Monomial> *) calloc (1, sizeof (*node));

    assert (node != nullptr);

    node->init ();

    node->data.data = data;
    node->data.type = type;

    node->right = right;
    node->left = left;

    if (node->left)
    {
        node->left->parent = node;
    }

    if (node->right)
    {
        node->right->parent = node;
    }

    assert (node != nullptr);

    return (node);
}

Node<Monomial> * Copy (const Node<Monomial> * origin)
{
    if (origin == nullptr)
    {
        return (nullptr);
    }

    return (CreateNode (origin->data.data, origin->data.type, Copy (origin->right), Copy (origin->left)));
}