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
    bool init ();
    bool clear ();

    bool print (FILE * log);
    bool fprint (FILE * log, const char * format);
    
    bool printDot (FILE * log, const char * format);

    Node<elem_t> * setChild ();

    Node<elem_t> * setLeft  ();
    Node<elem_t> * setRight ();

    Node<elem_t> * addLeft  (const elem_t data);
    Node<elem_t> * addRight (const elem_t data);

    bool search (const elem_t data);
};


template <typename elem_t>
class BinaryTree
{
private:
    size_t size;
    size_t dump_cnt;

public:
    Node<elem_t> * root;

    static Node<elem_t> * setNode (Node<elem_t> ** ptr);

    bool init (const elem_t data);
    bool init (const char * path);
    static char * initSubtree (Node<elem_t> * node, char * cur);
    bool clear ();

    bool deleteList (Node<elem_t> * node);
    bool deleteSubtree (Node<elem_t> * node);

    Node<elem_t> * search (const elem_t data);
    Node<elem_t> * rec_search (Node<elem_t> * node, const elem_t data);

    bool valid ();

    bool print (FILE * log);
    bool print (FILE * log, const char * format);

    bool dump  (FILE * log);
    bool dotDump (const char * format);
};

#endif // !TREE_CLASS_H_