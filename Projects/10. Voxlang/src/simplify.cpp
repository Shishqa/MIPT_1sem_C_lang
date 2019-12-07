// #include "headers.hpp"

// #include "operations.hpp"

// #include "node_operations.hpp"

// #include "simplify.hpp"

// Node<Token> * Simplifier::Simplify (BinaryTree<Token> * exp, Node<Token> * node)
// {
//     if (L)
//     {
//         L = Simplify (exp, L);
//         L->P = N;
//     }

//     if (R)
//     {
//         R = Simplify (exp, R);
//         R->P = N;
//     }

//     if (TYPE(N) != OP_TYPE)
//     {
//         return (N);
//     }

//     Node<Token> * tmp = nullptr;
//     int op_id = 0;
//     double l_val = 0;
//     double r_val = 0;

//     if ((R && TYPE(R) == NUM_TYPE) && (L && TYPE(L) == NUM_TYPE))
//     {
//         op_id = node->data.op_id;
//         l_val = DATA(L);
//         r_val = DATA(R);
//         exp->deleteSubtree (N);
//         return (n(operations[op_id].op (l_val, r_val)));
//     }

//     if ((R && TYPE(R) == NUM_TYPE) && !L)
//     {
//         op_id = node->data.op_id;
//         r_val = DATA(R);
//         exp->deleteSubtree (N);
//         return (n(operations[op_id].op (0, r_val)));
//     }

//     //ZERO
//     if ((OPCODE(N) == ADD || OPCODE(N) == SUB) && RIGHT (0))
//     {
//         tmp = c(L);
//         exp->deleteSubtree (N);
//         return (tmp);
//     }
//     if (OPCODE(N) == ADD && LEFT (0))
//     {
//         tmp = c(R);
//         exp->deleteSubtree (N);
//         return (tmp);
//     }
//     if (OPCODE(N) == SUB && LEFT (0))
//     {
//         tmp = c(R);
//         exp->deleteSubtree (N);
//         return (MUL( n(-1), tmp ));
//     }
//     if ((OPCODE(N) == MUL || OPCODE(N) == DIV) && (LEFT (0) || RIGHT (0)))
//     {
//         exp->deleteSubtree (N);
//         return (n(0));
//     }
//     if (OPCODE(N) == POW && LEFT (0))
//     {
//         exp->deleteSubtree (N);
//         return (n(0));
//     }
//     if (OPCODE(N) == POW && RIGHT (0))
//     {
//         exp->deleteSubtree (N);
//         return (n(1));
//     }

//     //ONE

//     if (OPCODE(N) == MUL && LEFT (1))
//     {
//         tmp = c(R);
//         exp->deleteSubtree (N);
//         return (tmp);
//     }
//     if (OPCODE(N) == MUL && RIGHT (1))
//     {
//         tmp = c(L);
//         exp->deleteSubtree (N);
//         return (tmp);
//     }
//     if (OPCODE(N) == DIV && RIGHT (1))
//     {
//         tmp = c(L);
//         exp->deleteSubtree (N);
//         return (tmp);
//     }
//     if (OPCODE(N) == POW && LEFT (1))
//     {
//         exp->deleteSubtree (N);
//         return (n(1));
//     }
//     if (OPCODE(N) == POW && RIGHT (1))
//     {
//         tmp = c(L);
//         exp->deleteSubtree (N);
//         return (tmp);
//     }

//     // VARIABLES
//     if (OPCODE(N) == ADD && TYPE(L) == VAR_TYPE && 
//         TYPE(R) == VAR_TYPE && VAR(L) == VAR(R))
//     {
//         tmp = c(L);
//         exp->deleteSubtree (N);
//         return (MUL (n(2), tmp));
//     }

//     if (OPCODE(N) == SUB && TYPE(L) == VAR_TYPE && 
//         TYPE(R) == VAR_TYPE && VAR(L) == VAR(R))
//     {
//         exp->deleteSubtree (N);
//         return (n(0));
//     }

//     if (OPCODE(N) == MUL && TYPE(L) == VAR_TYPE && 
//         TYPE(R) == VAR_TYPE && VAR(L) == VAR(R))
//     {
//         tmp = c(R);
//         exp->deleteSubtree (N);
//         return (POW(tmp, n(2)));
//     }

//     if (OPCODE(N) == DIV && TYPE(L) == VAR_TYPE && 
//         TYPE(R) == VAR_TYPE && VAR(L) == VAR(R))
//     {
//         exp->deleteSubtree (N);
//         return (n(1));
//     }

//     //

//     if (((OPCODE(N) == ADD || OPCODE(N) == MUL) && L && R && 
//           TYPE(L) == OP_TYPE && TYPE(R) == OP_TYPE && OPCODE(L) < OPCODE(R)) ||
//          (OPCODE(N) == MUL && TYPE(R) == NUM_TYPE && TYPE(L) != NUM_TYPE))
//     {
//         tmp = L;
//         L = R;
//         R = tmp;

//         return (N);
//     }

//     return (N);
// }

// double Simplifier::Count (const int op_id, const double l, const double r)
// {
//     return (operations[op_id].op (l, r));
// }

// int Simplifier::BinaryPow (int x, int a)
// {
//     if (a < 0)
//     {
//     	return 0;
//     }
//     if (a == 0)
//     {
//     	return 1;
//     }

//     int tmp = BinaryPow (x, a / 2);

//     return ((a % 2 == 0) ? (tmp * tmp) : (tmp * tmp * x));
// }