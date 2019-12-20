#include "tree.cpp"
#include "asm.cpp"
#include "CPU.cpp"
#include "backend.h"

void test_to_misha () {
    node* nd = rec_descent ();
    nd->to_misha ();
    nd->photo ("misha_tree.png");
    nd->save ("to_misha.txt", true);
}

void test_from_misha () {
    node* nd = get_tree ();
    nd->to_mine ();
    nd->photo ();

    front_reverse (nd);

    backend (nd);
    assembler ();
    processor ();
}

void test_mine () {
    node* nd = rec_descent ();
    nd->photo ();

    front_reverse (nd);
    backend (nd);
    assembler ();
    processor ();
}

int main() {
    int mode = 0;

    scanf ("%d", &mode);

    switch (mode) {
        case 1: {
            test_to_misha ();
            break;
        }
        case 2: {
            test_from_misha ();
            break;
        }
        case 3: {
            test_mine ();
            break;
        }
    }

    return 0;
}