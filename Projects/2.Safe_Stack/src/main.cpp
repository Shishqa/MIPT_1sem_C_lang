#include <stdio.h>

#include "assertion.h"
#include "MyStack_Test.h"
#include "MyStack.h"

int main ()
{
    TestValidator ();
    TestPush ();
    TestPop ();

    BadTest3 ();

    return 0;
}




