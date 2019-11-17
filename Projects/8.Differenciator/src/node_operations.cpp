#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

Node<Token> * CreateNode (const double data, const int op_id, const char var, const char type, 
                          Node<Token> * right, Node<Token> * left)
{
    Node<Token> * node = (Node<Token> *) calloc (1, sizeof (*node));

    assert (node != nullptr);

    node->init ();

    node->data.data  = data;
    node->data.op_id = op_id;
    node->data.type  = type;
    node->data.var   = var;

    node->right = right;
    node->left  = left;

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

Node<Token> * Copy (const Node<Token> * origin)
{
    if (origin == nullptr)
    {
        return (nullptr);
    }

    return (CreateNode (origin->data.data, origin->data.op_id, origin->data.var, origin->data.type, 
                        Copy (origin->right), Copy (origin->left)));
}