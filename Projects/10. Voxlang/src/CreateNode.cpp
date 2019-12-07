#include "token.hpp"
#include "CreateNode.hpp"

Node<Token> * SetNode (char type, int data, const char * name,
                       Node<Token> * left, Node<Token> * right)
{
    Node<Token> * node = (Node<Token> *) calloc (1, sizeof (*node));

    node->init ();

    node->data.type = type;
    node->data.data = data;
    node->data.name = name;

    if (left)
    {
        node->left = left;
        node->left->parent = node;
    }

    if (right)
    {
        node->right = right;
        node->right->parent = node;
    }

    return (node);
}

