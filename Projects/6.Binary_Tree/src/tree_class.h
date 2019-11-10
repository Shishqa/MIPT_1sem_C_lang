#ifndef TREE_CLASS_H_
#define TREE_CLASS_H_

template <typename elem_t>
struct Node
{
    elem_t data;
    Node<elem_t> * left;
    Node<elem_t> * right;

    Node<elem_t> * parent;

    bool init (const elem_t data);
    bool clear ();

    bool print (FILE * log);

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

    static Node<elem_t> * setNode (Node<elem_t> ** ptr);

    bool init (const elem_t data);
    bool init (const char * path);
    bool clear ();

    Node<elem_t> * search (const elem_t data);

    bool valid ();

    bool print (FILE * log);
    bool dump  (FILE * log);
    bool dotDump ();
};

#endif // !TREE_CLASS_H_