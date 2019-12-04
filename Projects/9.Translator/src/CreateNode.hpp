#ifndef CREATE_NODE_HPP_
#define CREATE_NODE_HPP_

#include "libraries.hpp"

#define LINK( node1, node2 )                \
        {                                   \
            (node1)->parent = node2;        \
        }

Node<Token> * SetNode (char type, const char * lexem, const size_t lexem_len,
                       Node<Token> * left = nullptr, Node<Token> * right = nullptr);

#endif // !CREATE_NODE_HPP_