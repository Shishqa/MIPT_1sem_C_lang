#ifndef CREATE_NODE_HPP_
#define CREATE_NODE_HPP_

#include "libraries.hpp"

#define LINK_P( p, n )      \
        (n)->parent = p     \

#define LINK_L( p, n )      \
        {                   \
            p->left = n;    \
            LINK_P (p, n);  \
        }

#define LINK_R( p, n )      \
        {                   \
            p->right = n;   \
            LINK_P (p, n);  \
        }

Node<Token> * SetNode (char type, int data, const char * name = nullptr,
                       Node<Token> * left = nullptr, Node<Token> * right = nullptr);

#endif // !CREATE_NODE_HPP_