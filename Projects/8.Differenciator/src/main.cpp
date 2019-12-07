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

    Differenciator diff = {};

    BinaryTree<Token> * diff_expression = diff.DiffWithLaTeX (&expression, 'x', working_with);

    expression.dotDump (PrintToken, 2);
    diff_expression->dotDump (PrintToken, 3);

    diff_expression->dotDump (PrintToken, 4);

    getPic (&expression, "exp_pic");
    getPic (diff_expression, "diff_pic");

    expression.clear();
    diff_expression->clear();

    return (0);
}
