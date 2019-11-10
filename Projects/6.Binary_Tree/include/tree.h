#ifndef TREE_H_
#define TREE_H_

template <typename elem_t>
struct Node
{
    elem_t data;
    Node<elem_t> * left;
    Node<elem_t> * right;

    Node<elem_t> * parent;

    bool init (elem_t data);
    bool clear ();

    Node<elem_t> * addLeft (elem_t data);
    Node<elem_t> * addRight (elem_t data);

    bool deleteList ();
    bool deleteSubtree ();
};


template <typename elem_t>
class BinaryTree
{
private:
    size_t size;

public:
    Node<elem_t> * root;

    bool init (elem_t data);
    bool clear ();

    Node<elem_t> * search (elem_t data);

    bool valid ();

    bool print (FILE* log);
    bool dump (FILE* log);
    bool dotDump ();
};

#endif // !TREE_H_
