#ifndef CREATE_NODE_HPP_
#define CREATE_NODE_HPP_

#include "head.hpp"

#define LINK( node1, node2 )                \
        {                                   \
            (node1)->parent = node2;        \
        }

Node<Token> * SetNode (char type, int data, const char * name,
                       Node<Token> * left = nullptr, Node<Token> * right = nullptr);

#endif // !CREATE_NODE_HPP_