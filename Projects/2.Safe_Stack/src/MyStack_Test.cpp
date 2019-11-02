#include "MyStack_Test.h"
#include "MyStack.h"


bool TestValidator ()
{
    printf ("\n------------------------------------------------------------------\n");
    printf ("Testing validator:\n\n");

    Stack_t stk_test = {};
    StackConstruct (&stk_test);

    int test_flag = 1;

    printf ("Test size_err: \t\t");

    stk_test.mem_size = 100;

    for (int i = -100; i < 200; ++i)
    {
        stk_test.size = i;

        if ((i >= stk_test.mem_size && StackValid (&stk_test) == SIZE_ERR_CODE) || i < stk_test.mem_size)
        {
            continue;
        }

        test_flag = 0;
    }

    printf("%s", (test_flag) ? "PASSED\n" : "NOT PASSED\n");

    test_flag = 1;

    printf ("Test mem_err: \t\t");

    stk_test.size = 3;
    stk_test.is_empty = false;
    stk_test.err = 0;

    for (int i = 4; i < 50; ++i)
    {
        stk_test.mem_size = i;

        if (((stk_test.size < i / RESIZE_GISTERESIS_RATIO) && StackValid (&stk_test) == MEM_SIZE_ERR_CODE) || stk_test.size >= i / RESIZE_GISTERESIS_RATIO)
        {
            continue;
        }
        //printf ("%d %d\n", i, stk_test.err);

        test_flag = 0;
    }

    printf("%s", (test_flag) ? "PASSED\n" : "NOT PASSED\n");

    printf ("Test non_init_err: \t");

    stk_test.data = nullptr;

    printf ("%s", (StackValid (&stk_test) == NON_INIT_ERR_CODE) ? "PASSED\n" : "NOT PASSED\n");

    printf ("Test destructed_err: \t");

    StackDestruct (&stk_test);

    printf ("%s", (StackValid (&stk_test) == DESTRUCTED_ERR_CODE) ? "PASSED\n" : "NOT PASSED\n");

    printf ("Test OK condition: \t");

    StackConstruct (&stk_test);

    printf ("%s", (StackValid (&stk_test) == OK_CODE) ? "PASSED\n" : "NOT PASSED\n");

    StackDestruct (&stk_test);

    printf ("------------------------------------------------------------------\n");
}


bool TestPush ()
{
    printf ("\n------------------------------------------------------------------\n");
    printf ("Testing PUSH: \t\t");

    Stack_t stk_test = {};
    StackConstruct (&stk_test);

    int test_flag = 1;
    Elem_t test_rec = 0;

    for (int i = 0; i < 100; ++i)
    {
        StackPush (&stk_test, i);
        if (StackTop (&stk_test, &test_rec) != OK_CODE || test_rec != i)
        {
            test_flag = 0;
        }
    }

    StackDestruct (&stk_test);

    printf("%s", (test_flag) ? "PASSED\n" : "NOT PASSED\n");

    printf ("------------------------------------------------------------------\n");
}


bool TestPop ()
{
    printf ("\n------------------------------------------------------------------\n");
    printf ("Testing POP: \t\t");

    Stack_t stk_test = {};
    StackConstruct (&stk_test);

    int test_flag = 1;
    Elem_t test_rec = 0;

    for (int i = 0; i <= 100; ++i)
    {
        StackPush (&stk_test, i);
    }

    for (int i = 100; i >= 0; --i)
    {
        if (StackPop (&stk_test, &test_rec) != OK_CODE || test_rec != i)
        {
            test_flag = 0;
        }
    }

    StackDestruct (&stk_test);

    printf("%s", (test_flag) ? "PASSED\n" : "NOT PASSED\n");

    printf ("------------------------------------------------------------------\n\n");
}


bool BadTest1 ()
{
    // лезем в стек сбоку

    Stack_t stk = {};
    StackConstruct (&stk);

    StackDump (&stk);

    for (int i = 0; i <= 10; ++i)
    {
        *((char*)(&stk) + i) = 1;
    }

    StackPush (&stk, 1);

    printf ("досюда не дойдет");

    StackDump (&stk);
}


bool BadTest2 ()
{
    // длина имени больше, чем константный массив
    // решение - strlen :)
    // плохое решение - выделять очень много памяти на имя - растет размер структуры

    Stack_t stkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstk = {};
    StackConstruct (&stkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstk);

    for (int i = 0; i <= 100; ++i)
    {
        StackPush (&stkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstk, i);
    }

    StackDump (&stkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstkstk);
}


bool BadTest3 ()
{
    //ehehehhehehehhehehehehhe

    Stack_t stk = {};
    StackConstruct (&stk);

    Elem_t eheheh = 0;

    StackPop (&stk, &eheheh);
}


bool BadTest4 ()
{
    // уязвимость - указатель на динамический массив не защищен, т.к. при проверке хеша мы по нему переходим
    // решение - раздельный подсчет хеша в 2 переменные

    Stack_t stk = {};
    StackConstruct (&stk);

    *((char*)&stk.data + 2) = 13;

    StackPush (&stk, 1);
}
