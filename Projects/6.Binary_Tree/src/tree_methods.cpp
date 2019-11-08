#include <stdio.h>
#include <stdlib.h>

#include "../include/tree.h"

template <typename elem_t>
bool BinaryTree<elem_t>::init (elem_t data)
{
    this->root = (Node<elem_t> *) calloc (1, sizeof (*this->root));
}