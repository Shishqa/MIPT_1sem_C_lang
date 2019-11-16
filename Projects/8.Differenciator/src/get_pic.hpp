#ifndef GET_PIC_HPP_
#define GET_PIC_HPP_

void getPic   (BinaryTree<Monomial> * expression, const char * name, bool open = false);
void getNodePic (Node<Monomial> * node, FILE * f);

#endif // !GET_PIC_HPP_