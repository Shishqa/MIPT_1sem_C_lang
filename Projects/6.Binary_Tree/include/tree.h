#ifndef TREE_H_
#define TREE_H_

template <typename elem_t>
struct Node
{
    elem_t data;
    Node<elem_t> * left;
    Node<elem_t> * right;

    Node<elem_t> * parent;

    bool init (const elem_t data);
    bool clear ();

    Node<elem_t> * addLeft  (const elem_t data);
    Node<elem_t> * addRight (const elem_t data);

    bool deleteList ();


    static bool deleteSubtree (const Node<elem_t> * node);
};


template <typename elem_t>
class BinaryTree
{
private:
    size_t size;

public:
    Node<elem_t> * root;

    bool init (const elem_t data);
    bool clear ();

    Node<elem_t> * search (const elem_t data);

    bool valid ();

    bool print (const FILE* log);
    bool dump  (const FILE* log);
    bool dotDump ();
};

#endif // !TREE_H_
