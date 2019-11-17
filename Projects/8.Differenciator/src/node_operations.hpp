#ifndef NODE_OPERATIONS_HPP_
#define NODE_OPERATIONS_HPP_

Node<Token> * CreateNode (const double data, const int opcode, const char type, 
                             Node<Token> * right = nullptr, Node<Token> * left  = nullptr);
                             
Node<Token> * Copy (const Node<Token> * origin);

#endif // !NODE_OPERATIONS_HPP_