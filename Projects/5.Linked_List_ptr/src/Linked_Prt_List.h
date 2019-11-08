#ifndef LINKED_PTR_LIST_H_
#define LINKED_PTR_LIST_H_

#include <stdio.h>

template <typename elem_t>
struct pNode
{
public:
    elem_t val;
    pNode *next;
    pNode *prev;

    void init(elem_t val, pNode *next, pNode *prev);
    void clear();
};

template <typename elem_t>
class pLinkedList
{
private:
    size_t size;
    pNode *head;
    pNode *tail;

public:
    pLinkedList *init();
    void clear();

    Node *insertFront(elem_t val);
    Node *insertBack(elem_t val);
    Node *insertAfter(Node *ptr, elem_t val);
    Node *insertBefore(Node *ptr, elem_t val);

    Node *findIndex(size_t index);
};

#endif LINKED_PTR_LIST_H_