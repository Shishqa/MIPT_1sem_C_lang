#ifndef GET_PIC_HPP_
#define GET_PIC_HPP_

void getPic   (BinaryTree<Token> * expression, const char * name, bool open = false);
void getNodePic (Node<Token> * node, FILE * f);

#endif // !GET_PIC_HPP_