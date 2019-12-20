#ifndef FRONTEND_REV
#define FRONTEND_REV

#include "tree.h"

#define flag printf ("\033[32;1m flag \033[0m\n\n");

#define error(code) fprintf(for_errors,"front_rev::%s (%s)\n", __PRETTY_FUNCTION__, #code);\
                    return false;



struct front_rev {
    node* root;
    FILE* dist;
    FILE* for_errors;
    int tabs;

    explicit front_rev (node* nodes = nullptr, const char* filename = "program_reversed.txt");

    bool print_tabs (int how_many = -1);

    bool global (node* nd);
    bool def_func (node* nd);
    bool call_func (node* nd);
    bool arguments (node* nd);

    bool instruction (node* nd);
    bool command (node* nd);

    bool equation (node* nd);
};

front_rev::front_rev(node* nodes, const char* filename) {
    root = nodes;
    dist = fopen (filename, "w");
    for_errors = fopen ("errors_in_front_rev.txt", "w");
    if (dist == nullptr) {
        err_info ("front_rev::front_rev (1)\n");
    }
    tabs = 0;
}

bool front_rev::print_tabs(int how_many) {
    if (how_many == -1) {
        for (int i = 0; i < tabs; ++i) {
            fprintf (dist, "\t");
        }
    }
    else {
        for (int i = 0; i < how_many; ++i) {
            fprintf (dist, "\t");
        }
    }

    return true;
}

bool front_rev::global(node* nd) {
    printf ("global:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);
    bool all_right = true;

    if (nd->type != BLOCK) {
        err_info ("front_rev::global (1)\n");
        return false;
    }

    if (nd->left != nullptr) {
        all_right = all_right && global (nd->left);
    }

    if (nd->right == nullptr) {
        err_info ("front_rev::global (2)\n");
    }

    if (nd->right->type == DEFINITION) {
        all_right = all_right && def_func (nd->right);
    }
    else if (nd->right->type == VARIABLE) {
        fprintf (dist, "person %s\n", nd->right->right->data);
    }
    else {
        err_info ("front_rev::global (3)\n");
    }

    return all_right;
}

bool front_rev::def_func(node *nd) {
    printf ("def_func:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);
    bool all_right = true;

    if (nd->type != DEFINITION) {
        error (1)
    }

    if (nd->is_left()) {
        fprintf (dist, "dictate %s of ", nd->right->data);
        arguments (nd->left);
        fprintf (dist, ":\n");
    }
    else {
        fprintf (dist, "dictate %s :\n", nd->right->data);
    }

    if (nd->right->is_right()) {
        ++tabs;
        instruction (nd->right->right);
        --tabs;
    }
    print_tabs ();
    fprintf (dist, ".\n");

    return all_right;
}

bool front_rev::call_func(node *nd) {
    printf ("call_fucn:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);

    bool all_right = true;

    if (nd->type != CALL) {
        error (1)
    }

    if (nd->is_left()) {
        fprintf (dist, "cause %s of ", nd->right->data);
        arguments (nd->left);
        fprintf (dist, "\n");
    }
    else {
        fprintf (dist, "cause %s\n", nd->right->data);
    }

    if (nd->right->is_right()) {
        instruction (nd->right->right);
    }

    return all_right;
}

bool front_rev::arguments(node *nd) {
    if (nd != nullptr) {
        printf ("arguments:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);

        if (nd->left != nullptr) {
            arguments (nd->left);
        }

        if (nd->right != nullptr) {
            fprintf (dist, "%s ", nd->right->data);
        }
    }
    else {
        return true;
    }
}

bool front_rev::instruction(node *nd) {

    if (nd != nullptr) {
        printf ("instrution:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);
        /*
        if (nd->type != INSTRUCTION) {
            error (1)
        }
         */
        instruction (nd->left);
        command (nd->right);
    }

    return true;

}

bool front_rev::command(node *nd) {
    if (nd == nullptr) {
        return false;
    }
    printf ("command:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);
    print_tabs ();

    switch (nd->type) {
        case VARIABLE: {
            fprintf(dist, "person %s\n", nd->left->data);
            if (nd->is_right()) {
                print_tabs ();
                print_tabs ();
                fprintf (dist, "%s now ", nd->right->data);
                equation (nd->right);
                fprintf (dist, "\n");
            }
            break;
        }
        case ID: {
            fprintf(dist, "%s ", nd->data);
            break;
        }
        case INCR: {
            fprintf(dist, "honor %s\n", nd->right->data);
            break;
        }
        case DECR: {
            fprintf(dist, "punish %s\n", nd->right->data);
            break;
        }
        case CALL: {
            call_func (nd);
            break;
        }
        case RETURN: {
            if (nd->is_right()) {
                fprintf(dist, "summon %s\n", nd->right->data);
            }
            else {
                fprintf(dist, "summon\n");
            }
            break;
        }
        case ASSIGN: {
            fprintf (dist, "%s now ", nd->left->data);
            equation (nd->right);
            fprintf (dist, "\n");
            break;
        }
        case OUT: {
            fprintf (dist, "know %s\n", nd->right->data);
            break;
        }
        case IF: {
            switch (nd->left->type) {
                case IS_EQUAL: {
                    fprintf (dist, "if \" %s is %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                case IS_BIGGER: {
                    fprintf (dist, "if \" %s better %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                case IS_LESS: {
                    fprintf (dist, "if \" %s worse %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                case NOT_EQUAL: {
                    fprintf (dist, "if \" %s not %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                default: {
                    error (1)
                }
            }

            ++tabs;
            if (nd->right->type == CONDITION) {
                instruction (nd->right->left);
            }
            else {
                instruction (nd->right);
            }
            --tabs;

            print_tabs ();
            fprintf (dist, ".\n");

            if (nd->right->type == CONDITION) {
                command (nd->right->right);
            }

            break;
        }
        case WHILE: {
            switch (nd->left->type) {
                case IS_EQUAL: {
                    fprintf (dist, "while \" %s is %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                case IS_BIGGER: {
                    fprintf (dist, "while \" %s better %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                case IS_LESS: {
                    fprintf (dist, "while \" %s worse %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                case NOT_EQUAL: {
                    fprintf (dist, "while \" %s not %s \" : \n", nd->left->left->data, nd->left->right->data);
                    break;
                }
                default: {
                    error (2)
                }
            }

            ++tabs;
            instruction (nd->right);
            --tabs;

            print_tabs ();
            fprintf (dist, ".\n");
            break;
        }
        case INSTRUCTION: {
            command (nd->right);
            break;
        }
        case CONDITION: {
            command (nd->left);
            break;
        }
        default: {
            fprintf(dist, "UNKNOWN\n");
        }
    }
}

bool front_rev::equation (node *nd) {
    printf ("equation:%15s\t\t%s\n\n", tokens[nd->type].name, nd->data);
    if (nd->left != nullptr && nd->type != CALL) {
        equation (nd->left);
    }

    switch (nd->type) {
        case NUMBER: {
            fprintf (dist, "%s", nd->data);
            break;
        }
        case ID: {
            fprintf (dist, "%s", nd->data);
            break;
        }
        case MULT: {
            fprintf (dist, " * ");
            break;
        }
        case CALL: {
            call_func (nd);
            break;
        }
        case DIVIDE: {
            fprintf (dist, " / ");
            break;
        }
        case PLUS: {
            fprintf (dist, " + ");
            break;
        }
        case MINUS: {
            fprintf (dist, " - ");
            break;
        }
    }

    if (nd->right != nullptr && nd->type != CALL) {
        equation (nd->right);
    }
}

bool front_reverse (node* nd) {
    printf ("\n\n\n\n\n\n\n\n\nfrontend-1\n\n");

    if (nd->type == PROGRAMM) {
        if (nd->is_left()) {
            nd = nd->left;
        }
        else {
            nd = nd->right;
        }
    }

    front_rev* rev = new front_rev (nd);

    bool all_right = true;
    all_right = all_right && rev->global (nd);

    fclose (rev->dist);

    return all_right;
}

#endif