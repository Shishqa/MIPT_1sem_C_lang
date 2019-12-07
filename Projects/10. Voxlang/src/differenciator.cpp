// #include "DSL.hpp"

// #include "head.hpp"
// #include "operations.hpp"
// #include "token.hpp"

// #include "CreateNode.hpp"

// #include "differenciator.hpp"

// #include "simplify.hpp"

// #define PRINT( s )      \
//         fprintf (latex, s)

// BinaryTree<Token> * Differenciator::Derivative  (const BinaryTree<Token> * exp, const char dvar)
// {
//     expression = exp;
//     var = dvar;

//     diff_expression = DiffExpression ();

//     Simplifier simplify = {};

//     diff_expression->root = simplify.Simplify (diff_expression, diff_expression->root);
//     diff_expression->root = simplify.Simplify (diff_expression, diff_expression->root);

//     return (diff_expression);
// }

// BinaryTree<Token> * Differenciator::DiffExpression ()
// {
//     assert (expression != nullptr);

//     diff_expression = (BinaryTree<Token> *) calloc (1, sizeof (*diff_expression));

//     assert (diff_expression != nullptr);

//     diff_expression->init ();

//     diff_expression->root = Diff (expression->root);

//     return (diff_expression);
// }

// Node<Token> * Differenciator::Diff (Node<Token> * node)
// {
//     assert (node  != nullptr);

//     Node<Token> * res = nullptr;

//     if (TYPE(N) == NUM_TYPE)
//     {
//         res = n(0);
//     }
//     else if (TYPE(N) == VAR_TYPE && node->data.var == var)
//     {
//         res = n(1);
//     }
//     else if (TYPE(N) == VAR_TYPE)
//     {
//         res = DIFF (v(VAR(N)), v(var));
//     }
//     else
//     {
//         switch (OPCODE(N))
//         {
//             case ADD: // dL + dR
//             {
//                 res = ADD ( d(L), d(R) ); 
//             }
//             break;

//             case SUB: // dL - dR
//             {
//                 res = SUB ( d(L), d(R) ); 
//             }
//             break;

//             case MUL: // dL * R + L * dR
//             {
//                 res = ADD ( MUL( d(L), c(R) ), 
//                               MUL( c(L), d(R) )); 
//             }
//             break;

//             case DIV: // (dL * R - L * dR) / (R ^ 2)
//             {
//                 res = DIV ( SUB ( MUL ( d(L), c(R) ), MUL ( c(L), d(R) )), POW ( c(R), n(2) ) ); 
//             }
//             break;

//             case POW: // (L ^ R) * (dR * ln(L) + (dL / L) * R)
//             {
//                 if (TYPE(R) == NUM_TYPE)
//                 {
//                     res = MUL ( n(DATA(R)), MUL ( POW(c(L), n(DATA(R) - 1)), d(L)) );
//                 }
//                 else
//                 {
//                     res = MUL ( c(N), ADD ( MUL ( d(R), LN( c(L) ) ), MUL ( DIV ( d(L), c(L) ), c(R) ))); 
//                 }
//             }
//             break;

//             case LN: // (1 / R) * dR
//             {
//                 res = MUL ( DIV ( n(1), c(R) ), d(R) ); 
//             }
//             break;

//             case EXP: // (e ^ R) * dR
//             {
//                 res = MUL ( EXP ( c(R) ), d(R) );
//             }
//             break;

//             case SIN: // cos(R) * dR
//             {
//                 res = MUL ( COS ( c(R) ), d(R) );
//             }
//             break;

//             case COS: // -1 * sin(R) * dR
//             {
//                 res = MUL ( MUL( n(-1), SIN ( c(R) )), 
//                               d(R) );
//             }
//             break;

//             case TG: // (1 / cos(R)^2) * dR
//             {
//                 res = MUL ( DIV ( n(1), POW ( COS ( c(R) ), n(2) ) ), d(R) );
//             }
//             break;

//             case CTG: // (-1 / sin(R)^2) * dR
//             {
//                 res = MUL ( DIV ( n(-1), POW ( SIN ( c(R) ), n(2) ) ) , d(R));
//             }
//             break;

//             case ARCSIN: // (1 / sqrt ( 1 - R^2 )) * dR
//             {
//                 res = MUL ( DIV ( n(1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) );
//             }
//             break;

//             case ARCCOS: // (-1 / sqrt ( 1 - R^2 )) * dR
//             {
//                 res = MUL ( DIV ( n(-1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) );
//             }
//             break;

//             case ARCTG: // (1 / ( 1 + R^2 )) * dR
//             {
//                 res = MUL ( DIV ( n(1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) );
//             }
//             break;

//             case ARCCTG: // (-1 / ( 1 + R^2 )) * dR
//             {
//                 res = MUL ( DIV ( n(-1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) );
//             }
//             break;

//             case SH: // ch(R) * dR
//             {
//                 res = MUL ( CH ( c(R) ) , d(R) );
//             }
//             break;

//             case CH: // sh(R) * dR
//             {
//                 res = MUL ( SH ( c(R) ) , d(R) );
//             }
//             break;

//             case TH: // (1 / ( ch(R) ^ 2 )) * dR 
//             {
//                 res = MUL ( DIV ( n(1), POW ( CH ( c(R) ), n(2) ) ) , d(R) );
//             }
//             break;

//             case CTH: // (-1 / ( sh(R) ^ 2 )) * dR 
//             {
//                 res = MUL ( DIV ( n(-1), POW ( SH ( c(R) ), n(2) ) ) , d(R) );
//             }
//             break;

//             case ABS: // sign(R) * dR
//             {
//                 res = MUL ( SIGN ( c(R) ), d(R) );
//             }
//             break;

//             case SIGN: // 0
//             {
//                 res = n(0);
//             }
//             break;
        
//             default:
//             {
//                 assert (0);
//             }
//             break;
//         }
//     }

//     return (res);
// }