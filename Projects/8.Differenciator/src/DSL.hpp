#ifndef DSL_H_
#define DSL_H_

#define L node->left

#define R node->right

#define N node

#define DATA( elem )                                \
        (elem)->data.data

#define TYPE( elem )                                \
        (elem)->data.type

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

#define DIFF( elem, val )                           \
        CreateNode (DIFF, OP_TYPE, val, elem)       

#define c( elem )                                   \
        Copy (elem)

#define d( elem )                                   \
        Diff (elem, var)

#define n( elem )                                   \
        CreateNode (elem, NUM_TYPE)

#define v( elem )                                   \
        CreateNode (elem, VAR_TYPE)

#define type( elem, t )                             \
        (elem)->data.type == t##_TYPE

#define RIGHT( val )                                \
        (TYPE(R) == NUM_TYPE && DATA(R) == (val))

#define LEFT( val )                                 \
        (TYPE(L) == NUM_TYPE && DATA(L) == (val))

#endif // !DSL_H_