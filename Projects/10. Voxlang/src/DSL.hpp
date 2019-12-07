// #ifndef DSL_HPP_
// #define DSL_HPP_

// #define L node->left

// #define R node->right

// #define N node       

// #define P parent

// #define TYPE( elem )                                \
//         (elem)->data.type

// #define DATA( elem )                                \
//         (elem)->data.data

// #define OPCODE( elem )                              \
//         operations[(elem)->data.op_id].opcode

// #define VAR( elem )                                 \
//         (char) (elem)->data.data

// #define ADD( elem_l, elem_r )                       \
//         CreateNode (-1, ADD, '#', OP_TYPE, elem_r, elem_l)

// #define SUB( elem_l, elem_r )                       \
//         CreateNode (-1, SUB, '#', OP_TYPE, elem_r, elem_l)

// #define MUL( elem_l, elem_r )                       \
//         CreateNode (-1, MUL, '#', OP_TYPE, elem_r, elem_l)

// #define DIV( elem_l, elem_r )                       \
//         CreateNode (-1, DIV, '#', OP_TYPE, elem_r, elem_l)

// #define POW( elem_l, elem_r )                       \
//         CreateNode (-1, POW, '#', OP_TYPE, elem_r, elem_l)

// #define SIN( elem )                                 \
//         CreateNode (-1, SIN, '#', OP_TYPE, elem)

// #define COS( elem )                                 \
//         CreateNode (-1, COS, '#', OP_TYPE, elem)

// #define TG( elem )                                 \
//         CreateNode (-1, TG, '#', OP_TYPE, elem)

// #define CTG( elem )                                 \
//         CreateNode (-1, CTG, '#', OP_TYPE, elem)

// #define ARCSIN( elem )                                 \
//         CreateNode (-1, ARCSIN, '#', OP_TYPE, elem)

// #define ARCCOS( elem )                                 \
//         CreateNode (-1, ARCCOS, '#', OP_TYPE, elem)

// #define ARCTG( elem )                                 \
//         CreateNode (-1, ARCTG, '#', OP_TYPE, elem)

// #define ARCCTG( elem )                                 \
//         CreateNode (-1, ARCCTG, '#', OP_TYPE, elem)

// #define LN( elem )                                  \
//         CreateNode (-1, LN, '#', OP_TYPE, elem)

// #define EXP( elem )                                  \
//         CreateNode (-1, EXP, '#', OP_TYPE, elem)

// #define ABS( elem )                                  \
//         CreateNode (-1, ABS, '#', OP_TYPE, elem)

// #define SIGN( elem )                                  \
//         CreateNode (-1, SIGN, '#', OP_TYPE, elem)

// #define SH( elem )                                  \
//         CreateNode (-1, SH, '#', OP_TYPE, elem)

// #define CH( elem )                                  \
//         CreateNode (-1, CH, '#', OP_TYPE, elem)

// #define TH( elem )                                  \
//         CreateNode (-1, TH, '#', OP_TYPE, elem)

// #define CTH( elem )                                  \
//         CreateNode (-1, CTH, '#', OP_TYPE, elem)

// #define DIFF( elem, val )                           \
//         CreateNode (-1, DIFF, '#', OP_TYPE, val, elem)       

// #define c( elem )                                   \
//         Copy (elem)

// #define d( elem )                                   \
//         Diff (elem)

// #define n( elem )                                   \
//         CreateNode (elem, 0, '#', NUM_TYPE)

// #define v( elem )                                   \
//         CreateNode (-1, 0, elem, VAR_TYPE)

// #define RIGHT( val )                                \
//         (TYPE(R) == NUM_TYPE && DATA(R) == (val))

// #define LEFT( val )                                 \
//         (TYPE(L) == NUM_TYPE && DATA(L) == (val))

// #endif // !DSL_HPP_