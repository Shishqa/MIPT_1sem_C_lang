#include "headers.hpp"

#include "parser.hpp"

#include "node_operations.hpp"

#include "differenciator.hpp"
#include "get_pic.hpp"
#include "get_latex.hpp"
#include "simplify.hpp"

int main ()
{
    const char working_with[PATH_MAX] = "exp1";

    BinaryTree<Token> expression = {};

    char init_path[PATH_MAX] = "";

    sprintf (init_path, "data/%s", working_with);

    RecursiveDescentParser Parser = {};

    Parser.parse (&expression, init_path);

    expression.dotDump (PrintToken, 1);

    BinaryTree<Token> * diff_expression = DiffWithLaTeX (&expression, 'x', working_with);

    expression.dotDump (PrintToken, 2);
    diff_expression->dotDump (PrintToken, 3);

    getPic (diff_expression, "not_simple_diff_pic");

    diff_expression->root = Simplify (diff_expression, diff_expression->root);

    diff_expression->dotDump (PrintToken, 4);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    //getLaTeX (&expression, diff_expression, working_with);

    expression.clear();
    diff_expression->clear();

    return (0);
}
