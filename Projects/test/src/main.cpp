#include <iostream>
#include <ms_stack.h>

int main()
{
    Stack_t test = {};
    StackConstruct(&test);

    StackPush(&test, 11);
    StackPush(&test, 12);
    StackPush(&test, 11);
    StackPush(&test, 12);
    StackPush(&test, 11);
    StackPush(&test, 12);

    StackDump(&test);

    return (0);
}