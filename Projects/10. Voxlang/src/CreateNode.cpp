#include "head.hpp"
#include "CreateNode.hpp"

Node<Token> * SetNode (char type, int data, const char * var,
                       Node<Token> * left, Node<Token> * right)
{
    Node<Token> * node = (Node<Token> *) calloc (1, sizeof (*node));

    node->init ();

    node->data.type      = type;
    node->data.lexem     = lexem;
    node->data.lexem_len = lexem_len;

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

