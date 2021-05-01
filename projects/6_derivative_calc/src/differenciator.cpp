#include "headers.hpp"

#include "operations.hpp"

#include "node_operations.hpp"

#include "differenciator.hpp"

#include "get_latex.hpp"
#include "get_pic.hpp"

#include "simplify.hpp"

#define PRINT( s )      \
        fprintf (latex, s)

BinaryTree<Token> * Differenciator::DiffWithLaTeX  (const BinaryTree<Token> * exp, const char dvar, const char * latex_path)
{
    expression = exp;
    var = dvar;

    latex = initLaTeX (latex_path);

    PRINT ("\\section*{Стишок о дифференцировании}\\\\\n");

    PRINT ("Продифференцируем $(");
    getNodeLaTeX (expression->root, latex);
    PRINT (")$, ведь мы не деградируем\\\\\n");

    diff_expression = DiffExpression ();

    getPic (diff_expression, "not_simple_diff_pic");

    PRINT ("\\\\Поумерь, дружочек, злобу. Получили зелибобу\\\\ $(");
    getNodeLaTeX (expression->root, latex);
    PRINT (")' = ");
    getNodeLaTeX (diff_expression->root, latex);
    PRINT ("$\\\\\n");

    PRINT ("Чтобы похвастаться тёще, получим штуку попроще\\\\\n");

    Simplifier simplify = {};

    diff_expression->root = simplify.Simplify (diff_expression, diff_expression->root);
    diff_expression->root = simplify.Simplify (diff_expression, diff_expression->root);

    PRINT ("$(");
    getNodeLaTeX (expression->root, latex);
    PRINT (")' = ");
    getNodeLaTeX (diff_expression->root, latex);
    PRINT ("$\\\\\n");

    closeLaTeX (latex);
    compileLaTeX (latex_path);

    sendMail ("jp190429@gmail.com", latex_path);

    return (diff_expression);
}

BinaryTree<Token> * Differenciator::DiffExpression ()
{
    assert (expression != nullptr);

    diff_expression = (BinaryTree<Token> *) calloc (1, sizeof (*diff_expression));

    assert (diff_expression != nullptr);

    diff_expression->init ();

    diff_expression->root = Diff (expression->root);

    return (diff_expression);
}

Node<Token> * Differenciator::Diff (Node<Token> * node)
{
    assert (node  != nullptr);

    Node<Token> * res = nullptr;

    if (TYPE(N) == NUM_TYPE)
    {
        res = n(0);
        PRINT("Ну константа - тривиально, и ничуть не криминально\\\\\n");
    }
    else if (TYPE(N) == VAR_TYPE && node->data.var == var)
    {
        res = n(1);
        PRINT("Знает рыжая лисица, что у нас тут единица\\\\\n");
    }
    else if (TYPE(N) == VAR_TYPE)
    {
        res = DIFF (v(VAR(N)), v(var));
        PRINT("Этот дружочек зашёл не туда, наложим мы штрих на него навсегда\\\\\n");
    }
    else
    {
        switch (OPCODE(N))
        {
            case ADD: // dL + dR
            {
                res = ADD ( d(L), d(R) ); 
                PRINT("Производная суммы, тут ничего не рифмуется\\\\\n");
            }
            break;

            case SUB: // dL - dR
            {
                res = SUB ( d(L), d(R) ); 
                PRINT("Давай раскроем разность, тут не ждет опасность\\\\\n");
            }
            break;

            case MUL: // dL * R + L * dR
            {
                res = ADD ( MUL( d(L), c(R) ), 
                              MUL( c(L), d(R) )); 
                PRINT("Скобок мельтешение - раскрыли умножение\\\\\n");
            }
            break;

            case DIV: // (dL * R - L * dR) / (R ^ 2)
            {
                res = DIV ( SUB ( MUL ( d(L), c(R) ), MUL ( c(L), d(R) )), POW ( c(R), n(2) ) ); 
                PRINT("Производная частного для тебя несчастного\\\\\n");
            }
            break;

            case POW: // (L ^ R) * (dR * ln(L) + (dL / L) * R)
            {
                if (TYPE(R) == NUM_TYPE)
                {
                    res = MUL ( n(DATA(R)), MUL ( POW(c(L), n(DATA(R) - 1)), d(L)) );
                    PRINT("Вниз снеси ты показатель, производной соискатель\\\\\n");
                }
                else
                {
                    res = MUL ( c(N), ADD ( MUL ( d(R), LN( c(L) ) ), MUL ( DIV ( d(L), c(L) ), c(R) ))); 
                    PRINT("Мама Люба раму мыла, щас получим крокодила\\\\\n");
                }
            }
            break;

            case LN: // (1 / R) * dR
            {
                res = MUL ( DIV ( n(1), c(R) ), d(R) ); 
                PRINT("Посчитаем логарифм, тут не надо других рифм\\\\\n");
            }
            break;

            case EXP: // (e ^ R) * dR
            {
                res = MUL ( EXP ( c(R) ), d(R) );
                PRINT("ехехехеххехехехехеххе\\\\\n");
            }
            break;

            case SIN: // cos(R) * dR
            {
                res = MUL ( COS ( c(R) ), d(R) );
                PRINT("Синус быстренько раскроем, а потом полы помоем\\\\\n");
            }
            break;

            case COS: // -1 * sin(R) * dR
            {
                res = MUL ( MUL( n(-1), SIN ( c(R) )), 
                              d(R) );
                PRINT("Это что за мерзкий гнус? Точно, это ж косинус!\\\\\n");
            }
            break;

            case TG: // (1 / cos(R)^2) * dR
            {
                res = MUL ( DIV ( n(1), POW ( COS ( c(R) ), n(2) ) ), d(R) );
                PRINT("Тангенс мы раскроем смело, пока в глазах не потемнело\\\\\n");
            }
            break;

            case CTG: // (-1 / sin(R)^2) * dR
            {
                res = MUL ( DIV ( n(-1), POW ( SIN ( c(R) ), n(2) ) ) , d(R));
                PRINT("Котангенс мы раскроем смело, пока в глазах не просветлело\\\\\n");
            }
            break;

            case ARCSIN: // (1 / sqrt ( 1 - R^2 )) * dR
            {
                res = MUL ( DIV ( n(1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) );
            }
            break;

            case ARCCOS: // (-1 / sqrt ( 1 - R^2 )) * dR
            {
                res = MUL ( DIV ( n(-1), POW ( SUB ( n(1), POW ( c(R), n(2) ) ), DIV (n(1), n(2)) ) ), d(R) );
            }
            break;

            case ARCTG: // (1 / ( 1 + R^2 )) * dR
            {
                res = MUL ( DIV ( n(1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) );
            }
            break;

            case ARCCTG: // (-1 / ( 1 + R^2 )) * dR
            {
                res = MUL ( DIV ( n(-1), ADD ( n(1), POW ( c(R), n(2) )) ), d(R) );
            }
            break;

            case SH: // ch(R) * dR
            {
                res = MUL ( CH ( c(R) ) , d(R) );
            }
            break;

            case CH: // sh(R) * dR
            {
                res = MUL ( SH ( c(R) ) , d(R) );
            }
            break;

            case TH: // (1 / ( ch(R) ^ 2 )) * dR 
            {
                res = MUL ( DIV ( n(1), POW ( CH ( c(R) ), n(2) ) ) , d(R) );
            }
            break;

            case CTH: // (-1 / ( sh(R) ^ 2 )) * dR 
            {
                res = MUL ( DIV ( n(-1), POW ( SH ( c(R) ), n(2) ) ) , d(R) );
            }
            break;

            case ABS: // sign(R) * dR
            {
                res = MUL ( SIGN ( c(R) ), d(R) );
            }
            break;

            case SIGN: // 0
            {
                res = n(0);
            }
            break;
        
            default:
            {
                assert (0);
            }
            break;
        }
    }

    fprintf (latex, "\\begin{math}\n\t(");

    getNodeLaTeX (node, latex);

    fprintf (latex, ")' = ");

    getNodeLaTeX (res, latex);

    fprintf (latex, "\n\\end{math}\\\\\n");

    return (res);
}