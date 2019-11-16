#ifndef NODE_OPERATIONS_HPP_
#define NODE_OPERATIONS_HPP_

Node<Monomial> * CreateNode (const int data, const char type, Node<Monomial> * right = nullptr, 
                                                              Node<Monomial> * left  = nullptr);
Node<Monomial> * Copy (const Node<Monomial> * origin);

#endif // !NODE_OPERATIONS_HPP_