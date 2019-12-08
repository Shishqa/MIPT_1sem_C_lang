// #include "libraries.hpp"
// #include "Parser.hpp"
// #include "Decompiler.hpp"

// #define N node
// #define L node->left
// #define R node->right

// #define STOP( err_code )                    \
//     {                                       \
//         error = err_code;                   \
//         return;                             \
//     }                                   

// bool Decompiler::GetCode (BinaryTree<Token> * code, const char * output_path)
// {
//     FILE * f = fopen (output_path, "w");

//     fprintf (f, "$$ ##      THIS FILE IS GENERATED AUTOMATICALLY      ##\n");
//     fprintf (f, "$$ ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##\n");

//     prog = code;

//     Proceed (f);
    
//     fclose (f);

//     return (true);
// }

// void Decompiler::Proceed (FILE * f)
// {
//     out = f;

//     GetBlocks (prog->root);
// }

// void Decompiler::GetBlocks (Node<Token> * node)
// {
//     PRINT ("blocks\n");

//     if (!N)
//     {
//         return;
//     }

//     GetBlocks (L);

//     if (R->data.type == DEF_FUNC)
//     {
//         DefFunc (R);
//     }
//     else if (R->data.type == DEF_VAR)
//     {
//         DefVar (R);
//     }
// }

// void Decompiler::DefFunc (Node<Token> * node)
// {
//     PRINT ("func\n");

//     fprintf (out, "def %s (", R->data.lexem);

//     Node<Token> * curr_arg = L;

//     while (curr_arg)
//     {
//         fprintf (out, "var %s", curr_arg->right->left->data.lexem);

//         curr_arg = curr_arg->left;

//         if (curr_arg)
//         {
//             fprintf (out, ", ");
//         }
//     }

//     fprintf (out, ")\n{\n");

//     GetOperators (R->right);

//     fprintf (out, "}\n");
// }

// void Decompiler::GetOperators (Node<Token> * node)
// {
//     PRINT ("operator\n");

//     if (!N)
//     {
//         return;
//     }

//     if (N->data.type == BLOCK)
//     {
//         GetOperators (R);
//         return;
//     }

//     GetOperators (L);

//     if (R->data.type == IF)
//     {
//         GetIf (R);
//     }
//     else if (R->data.type == WHILE)
//     {
//         GetWhile (R);
//     }
//     else if (R->data.type == DEF_VAR)
//     {
//         DefVar (R);
//     }
//     else if (R->data.type == ASSIGN)
//     {
//         GetAssign (R);
//     }
//     else if (R->data.type == CALL)
//     {
//         GetCall (R);
//     }

//     fprintf (out, "\n");
// }

// void Decompiler::GetAssign (Node<Token> * node)
// {
//     PRINT ("assn\n");

//     fprintf (out, "%s = ", L->data.lexem);

//     Calculate (R);
// }

// void Decompiler::DefVar (Node<Token> * node)
// {
//     PRINT ("var\n");

//     fprintf (out, "var ");

//     if (R)
//     {
//         GetAssign (N);
//     }
//     else
//     {
//         fprintf (out, "%s\n", L->data.lexem);
//     }
// }

// void Decompiler::GetCall (Node<Token> * node)
// {
//     PRINT ("call\n");

//     fprintf (out, "%s(", L->data.lexem);

//     Node<Token> * curr_arg = R;

//     while (curr_arg)
//     {
//         Calculate (curr_arg->right);

//         if (curr_arg->left)
//         {
//             fprintf (out, ", ");
//         }

//         curr_arg = curr_arg->left;
//     }

//     fprintf (out, ")");
// }

// void Decompiler::GetIf (Node<Token> * node)
// {
//     PRINT ("if\n");

//     fprintf (out, "if (");

//     Calculate (N->left);

//     fprintf (out, ")\n{\n");

//     GetOperators (R->right);

//     fprintf (out, "}");
    
//     if (R->left)
//     {
//         fprintf (out, "\nelse\n{\n");

//         GetOperators (R->left);

//         fprintf (out, "}");
//     }
// }

// void Decompiler::GetWhile (Node<Token> * node)
// {
//     PRINT ("while\n");

//     fprintf (out, "while (");

//     Calculate (L);

//     fprintf (out, ")\n{\n");
    
//     GetOperators (R);

//     fprintf (out, "}");
// }

// void Decompiler::Calculate (Node<Token> * node)
// {
//     PRINT ("calculate\n");

//     if (L && N->data.type != CALL)
//     {
//         Calculate (L);
//     }

//     switch (node->data.type)
//     {
//         case ADD:
//             fprintf (out, "+");
//         break;

//         case SUB:
//             fprintf (out, "-");
//         break;

//         case MUL:
//             fprintf (out, "*");
//         break;

//         case DIV:
//             fprintf (out, "/");
//         break;

//         case MOD:
//             fprintf (out, "%");
//         break;
        
//         case AND:
//             fprintf (out, "&");
//         break;

//         case OR:
//             fprintf (out, "|");
//         break;

//         case NUM:
//             fprintf (out, "%s", N->data.lexem);
//         break;

//         case LESS:
//             fprintf (out, "<");
//         break;

//         case GREATER:
//             fprintf (out, ">");
//         break;

//         case LEQ:
//             fprintf (out, "<=");
//         break;

//         case GEQ:
//             fprintf (out, ">=");
//         break;

//         case EQ:
//             fprintf (out, "==");
//         break;

//         case NEQ:
//             fprintf (out, "!=");
//         break;

//         case ID:
//             fprintf (out, "%s", N->data.lexem);
//         break;

//         case CHAR:
//             fprintf (out, "'%s'", N->data.lexem);
//         break;

//         case STRING:
//             fprintf (out, "\"%s\"", N->data.lexem);
//         break;

//         case CALL:
//             GetCall (node);
//         break;
    
//         default:
//             fprintf (out, "NULL");
//         break;
//     }

//     if (R && N->data.type != CALL)
//     {
//         Calculate (R);
//     }
// }
