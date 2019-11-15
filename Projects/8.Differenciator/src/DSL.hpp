#ifndef DSL_H_
#define DSL_H_

#define L node->left

#define R node->right

#define ADD( elem_l, elem_r )                       \
        CreateNode (ADD, OP_TYPE, elem_r, elem_l)

#define SUB( elem_l, elem_r )                       \
        CreateNode (SUB, OP_TYPE, elem_r, elem_l)

#define MUL( elem_l, elem_r )                       \
        CreateNode (MUL, OP_TYPE, elem_r, elem_l)

#define DIV( elem_l, elem_r )                       \
        CreateNode (DIV, OP_TYPE, elem_r, elem_l)

#define POW( elem_l, elem_r )                       \
        CreateNode (POW, OP_TYPE, elem_r, elem_l)

#define SIN( elem )                                 \
        CreateNode (SIN, OP_TYPE, elem)

#define COS( elem )                                 \
        CreateNode (COS, OP_TYPE, elem)

#define LN( elem )                                  \
        CreateNode (LN, OP_TYPE, elem)

#define c( elem )                                   \
        Copy (elem)

#define d( elem )                                   \
        Diff (elem)

#define v( elem )                                   \
        CreateNode (elem, NUM_TYPE)

#endif // !DSL_H_