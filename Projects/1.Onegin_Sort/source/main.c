/**
 * @file main.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include "assertion.h"
#include "streamlib.h"


void Test ();
int t_CharReplace ();
int t_CmpFront ();
int t_CmpBack ();
int t_Swap ();
int t_QuickSort ();
int t_CopyBytes ();


void StdUse ();
void InitFileNames (char* file_in, char* file_out);
int ArrangePointers (char* buffer, const int num_of_symbols, String** pointer);
void QuickSort (void* begin_ptr, void* end_ptr, int (*cmp)(const void*, const void*), size_t type_size);


int main (int argc, char** argv)
{
    if (argc <= 1)
    {
        printf ("Syntax: %s\n\t -s -- Standart mode of use\n\t -t -- Test mode\n\n", argv[0]);

        return 0;
    }
    if (!StrCmp (argv[1], "-s"))
    {
        printf ("Standart mode:\n\n");
        StdUse ();

        return 0;
    }
    if (!StrCmp (argv[1], "-t"))
    {
        printf ("Test mode:\n\n");
        Test ();

        return 0;
    }

    printf ("Unknown argument: %s\n", argv[1]);

    return 0;
}


/**
 * @brief Test mode
 */
void Test ()
{
    int WA_meter = 0;

    //.....................................................
    printf ("TESTING CopyBytes():\n\n");

    WA_meter = t_CopyBytes();

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''

    //.....................................................
    printf ("TESTING Swap():\n\n");

    WA_meter = t_Swap();

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''

    //.....................................................
    printf ("TESTING comparator:\n\n");

    int a = 3;
    int b = 30;

    if (cmp_int (&a, &b) < 0 && cmp_int(&b, &a) > 0)
    {
        printf ("%d > %d : %d\n", a, b, cmp_int(&a, &b));
        printf ("%d > %d : %d\n", b, a, cmp_int(&b, &a));
        WA_meter = 0;
    }
    else
    {
        WA_meter = 1;
    }

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''

    //.....................................................
    printf ("TESTING CmpFront():\n\n");

    WA_meter = t_CmpFront ();

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''

    //.....................................................
    printf ("TESTING CmpBack():\n\n");

    WA_meter = t_CmpBack ();

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''

    //.....................................................
    printf ("TESTING QuickSort():\n\n");

    WA_meter = t_QuickSort ();

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''

    //.....................................................
    printf ("TESTING CharReplace():\n\n");

    WA_meter = t_CharReplace ();

    if (WA_meter)
    {
        printf ("\nNOT OK\n\n");
    }
    else
    {
        printf ("\nOK\n\n");
    }
    //'''''''''''''''''''''''''''''''''''''''''''''''''''''
}


/**
 * @brief Unit-test of CopyBytes()
 */
int t_CopyBytes ()
{
    const int NUM_OF_TESTS = 10;
    int err_cnt = 0;

    int test_int[10] = {1234567890, 432, 0, -4, 890, 94832, '0', -123123123, 999999, 10000};
    int* test = (int*) calloc (NUM_OF_TESTS + 1, sizeof(*test));
    int* p = test_int;

    CopyBytes(test_int, test, NUM_OF_TESTS, sizeof(*test_int));

    for (int* ptr = test; ptr < test + NUM_OF_TESTS; ++ptr)
    {
        if (*ptr == *p)
        {
            printf ("\033[32mOK: %d -> %d\033[0m\n", *p, *ptr);
        }
        else
        {
            printf ("\033[31mWR: %d =/= %d (expected)\033[0m\n", *ptr, *p);
            ++err_cnt;
        }

        ++p;
    }

    free (test);

    return err_cnt;
}


/**
 * @brief Unit-test of Swap()
 */
int t_Swap ()
{
    int err_cnt = 0;

    const int NUM_OF_TESTS = 10;
    const int DIR_LIM = 100000;

    String test1 = { NULL,
                     NULL };
    String test2 = { NULL,
                     NULL };

    String expect1 = { NULL,
                       NULL };
    String expect2 = { NULL,
                       NULL };

    for (int i = 0; i < NUM_OF_TESTS; ++i)
    {
        test1.begin = (char*)(rand() % DIR_LIM);
        test1.end   = (char*)(rand() % DIR_LIM);

        test2.begin = (char*)(rand() % DIR_LIM);
        test2.end   = (char*)(rand() % DIR_LIM);

        CopyBytes (&test1, &expect2, 1, sizeof (String));
        CopyBytes (&test2, &expect1, 1, sizeof (String));

        Swap (&test1, &test2, sizeof (String));

        if ( test1.begin == expect1.begin &&
             test1.end   == expect1.end   &&
             test2.begin == expect2.begin &&
             test2.end   == expect2.end   )
        {
            printf("\033[32m");
            printf ("OK: t1 {%d, %d} \t<-> \tt2 {%d, %d}\n", test1.begin, test1.end, test2.begin, test2.end);
            printf("\033[0m");
        }
        else
        {
            printf("\033[31m");
            printf ("WR: t1.b: %d t1.e: %d, \texpected: %d %d\n", test1.begin, test1.end, expect1.begin, expect1.end);
            printf ("    t2.b: %d t2.e: %d, \texpected: %d %d\n", test2.begin, test2.end, expect2.begin, expect2.end);
            printf("\033[0m");
            ++err_cnt;
        }
    }

    return err_cnt;
}


/**
 * @brief Unit-test of CmpFront()
 */
int t_CmpFront()
{
    int err_cnt = 0;

    const int NUM_OF_TESTS = 10;
    const int SIZE_OF_TEST = 20;

    char test[10][20] =   { "  aaaaaaaaaaaaaaaaa\0",   //0
                            "aaaaaaaaaaaaaaaaa\0  ",   //1
                            "AAAAAAAAAAAAAAAAA\0  ",   //2
                            "AAAAAAA AAAAAAAAAAA\0",   //3
                            "bbbbbbbbbbbbbbbbbbb\0",   //4
                            "Bcbbbbbbbbbbbbbbbbb\0",   //5
                            "bdaaaaaaaaaaaaaaaaa\0",   //6
                            "xyz                \0",   //7
                            "God,help me, please\0",   //8
                            "                   \0" }; //9


                             // 0   1   2   3   4   5   6   7   8   9
    int relations[10][10] = { { 0,  0,  0,  1, -1, -1, -1, -1, -1,  1},  //0
                              { 0,  0,  0,  1, -1, -1, -1, -1, -1,  1},  //1
                              { 0,  0,  0,  1, -1, -1, -1, -1, -1,  1},  //2
                              {-1, -1, -1,  0, -1, -1, -1, -1, -1,  1},  //3
                              { 1,  1,  1,  1,  0, -1, -1, -1, -1,  1},  //4
                              { 1,  1,  1,  1,  1,  0, -1, -1, -1,  1},  //5
                              { 1,  1,  1,  1,  1,  1,  0, -1, -1,  1},  //6
                              { 1,  1,  1,  1,  1,  1,  1,  0,  1,  1},  //7
                              { 1,  1,  1,  1,  1,  1,  1, -1,  0,  1},  //8
                              {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0} };//9

    String a = { NULL,
                 NULL };
    String b = { NULL,
                 NULL };

    for (int i = 0; i < NUM_OF_TESTS; ++i)
    {
        for (int j = 0; j < NUM_OF_TESTS; ++j)
        {
            a.begin = test[i];
            a.end =   test[i] + SIZE_OF_TEST - 1;

            b.begin = test[j];
            b.end =   test[j] + SIZE_OF_TEST - 1;

            int res = CmpFront (a, b);

            if (res >  0 && relations[i][j] >  0 ||
                res == 0 && relations[i][j] == 0 ||
                res <  0 && relations[i][j] <  0 )
            {
                printf ("\033[32m");
                printf ("%s \t%d \t%s\n", a.begin, relations[i][j], b.begin);
                printf ("\033[0m");
            }
            else
            {
                printf ("\033[31m");
                printf ("%s \t%d \t%s\t\texpected:%d\n", a.begin, res, b.begin, relations[i][j]);
                printf ("\033[0m");
                ++err_cnt;
            }
        }
    }

    return err_cnt;
}


/**
 * @brief Unit-test of CmpBack()
 */
int t_CmpBack()
{
    int err_cnt = 0;

    const int NUM_OF_TESTS = 10;
    const int SIZE_OF_TEST = 20;

    char test[10][20] =   { "  aaaaaaaaaaaaaaaaa\0",   //0
                            "aaaaaaaaaaaaaaaaa  \0",   //1
                            "AAAAAAAAAAAAAAAAA  \0",   //2
                            "AAAAAAA AAAAAAAAAAA\0",   //3
                            " bbbbbbbbbbbbbbbbbb\0",   //4
                            "Bbbbbbbbbbbbbbbb,bB\0",   //5
                            "bdaaaaaaaaaaaaaaaaa\0",   //6
                            "xyz                \0",   //7
                            "God,help me, please\0",   //8
                            "                   \0" }; //9


                             // 0   1   2   3   4   5   6   7   8   9
    int relations[10][10] = { { 0,  1,  1, -1, -1, -1, -1, -1, -1,  1},  //0
                              {-1,  0,  0, -1, -1, -1, -1, -1, -1,  1},  //1
                              {-1,  0,  0, -1, -1, -1, -1, -1, -1,  1},  //2
                              { 1,  1,  1,  0, -1, -1, -1, -1, -1,  1},  //3
                              { 1,  1,  1,  1,  0,  1,  1, -1, -1,  1},  //4
                              { 1,  1,  1,  1, -1,  0,  1, -1, -1,  1},  //5
                              { 1,  1,  1,  1, -1, -1,  0, -1, -1,  1},  //6
                              { 1,  1,  1,  1,  1,  1,  1,  0,  1,  1},  //7
                              { 1,  1,  1,  1,  1,  1,  1, -1,  0,  1},  //8
                              {-1, -1, -1, -1, -1, -1, -1, -1, -1,  0} };//9

    String a = { NULL,
                 NULL };
    String b = { NULL,
                 NULL };

    for (int i = 0; i < NUM_OF_TESTS; ++i)
    {
        for (int j = 0; j < NUM_OF_TESTS; ++j)
        {
            a.begin = test[i];
            a.end =   test[i] + SIZE_OF_TEST - 1;

            b.begin = test[j];
            b.end =   test[j] + SIZE_OF_TEST - 1;

            int res = CmpBack (a, b);

            if (res >  0 && relations[i][j] >  0 ||
                res == 0 && relations[i][j] == 0 ||
                res <  0 && relations[i][j] <  0 )
            {
                printf ("\033[32m");
                printf ("%s \t%d \t%s\n", a.begin, relations[i][j], b.begin);
                printf ("\033[0m");
            }
            else
            {
                printf ("\033[31m");
                printf ("%s \t%d \t%s\texpected:%d\n", a.begin, res, b.begin, relations[i][j]);
                printf ("\033[0m");
                ++err_cnt;
            }
        }
    }

    return err_cnt;
}


/**
 * @brief Unit-test of QuickSort()
 */
int t_QuickSort ()
{
    int err_cnt = 0;

    const int NUM_OF_TESTS = 10;
    const int NUM_OF_ELEMS = 10;

    int* arr    = NULL;
    int* expect = NULL;

    for (int i = 1; i <= NUM_OF_TESTS; ++i)
    {
        printf ("\n#test %d\n\n", i);

        arr    = (int*) calloc (NUM_OF_ELEMS, sizeof(int));
        expect = (int*) calloc (NUM_OF_ELEMS, sizeof(int));

        int* ptr_e = expect;

        for (int* ptr = arr; ptr < arr + NUM_OF_ELEMS; ++ptr, ++ptr_e)
        {
            *ptr = rand();
            *ptr_e = *ptr;
        }

        qsort (expect, NUM_OF_ELEMS, sizeof (int), cmp_int);
        QuickSort (arr, arr + NUM_OF_ELEMS - 1, cmp_int, sizeof (int));

        ptr_e = expect;

        for (int* ptr = arr; ptr < arr + NUM_OF_ELEMS; ++ptr, ++ptr_e)
        {
            if (*ptr == *ptr_e)
            {
                printf ("\033[32m");
                printf ("%d \t== \t%d\n", *ptr, *ptr_e);
                printf ("\033[0m");
            }
            else
            {
                printf ("\033[31m");
                printf ("%d \t=/= \t%d\n", *ptr, *ptr_e);
                printf ("\033[0m");
                ++err_cnt;
            }
        }

        free (arr);
        free (expect);
    }

    return err_cnt;
}


/**
 * @brief Unit-test of CharReplace()
 */
int t_CharReplace ()
{
    int err_cnt = 0;

    const int NUM_OF_TESTS = 6;
    const int SIZE_OF_TEST = 20;

    char test[6][20] =   { "aaaaaaaaaaaaaaaaaaa\0",
                           "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\0",
                           "wabcdefghijklmnopqr\0",
                           "qweqweFqweqweqweqwe\0",
                           "qwertyuiopasdfghjkl\0",
                           "4593-&1=-012-(*&^*^\0" };

    char from[6] = {'3', '\t', 'i', 'e', 'f', '&'};
    char to[6]   = {'1', '0', 'q', 'E', '%', 'v'};

    char expect[6][20] = { "aaaaaaaaaaaaaaaaaaa\0",
                           "0000000000000000000\0",
                           "wabcdefghqjklmnopqr\0",
                           "qwEqwEFqwEqwEqwEqwE\0",
                           "qwertyuiopasd%ghjkl\0",
                           "4593-v1=-012-(*v^*^\0" };

    for (int i = 0; i < NUM_OF_TESTS; ++i)
    {
        CharReplace (test[i], SIZE_OF_TEST, from[i], to[i]);
        if (StrCmp (test[i], expect[i]) == 0)
        {
            printf ("\033[32m");
            printf ("%s -> %s\n", test[i], expect[i]);
            printf ("\033[0m");
        }
        else
        {
            printf ("\033[31m");
            printf ("NOT OK:\n");
            printf ("t: %s\ne: %s\n", test[i], expect[i]);
            printf ("\033[0m");

            ++err_cnt;
        }
    }

    return err_cnt;
}


/**
 * @brief Standart use
 */
void StdUse ()
{
    char in_file_name[PATH_MAX]  = {};
    char out_file_name[PATH_MAX] = {};

    //.............................................
    PRINTF ("# initiation\n\n");
    //'''''''''''''''''''''''''''''''''''''''''''''

    InitFileNames (in_file_name, out_file_name);

    FILE* stream_in = fopen (in_file_name, "r");

    char* buffer = NULL;
    int num_of_symbols = Read (&buffer, stream_in);

    fclose (stream_in);

    String* text = NULL;
    int num_of_lines = ArrangePointers (buffer, num_of_symbols, &text);

    //.............................................
    PRINTF ("# front sort started\n");
    //'''''''''''''''''''''''''''''''''''''''''''''

    QuickSort (text, text + num_of_lines - 1, comp_front, sizeof(String));

    WriteLines (text, out_file_name, "a");

    //.............................................
    PRINTF ("# front sort written\n\n");
    PRINTF ("# back sort started\n");
    //'''''''''''''''''''''''''''''''''''''''''''''

    QuickSort (text, text + num_of_lines - 1, comp_back, sizeof(String));

    WriteLines (text, out_file_name, "a");

    //.............................................
    PRINTF ("# back sort written\n\n");
    PRINTF ("# writing orig\n");
    //'''''''''''''''''''''''''''''''''''''''''''''

    FILE* stream_out = fopen (out_file_name, "a");

    CharReplace (buffer, num_of_symbols, '\0', '\n');

    Write (buffer, stream_out);

    fclose (stream_out);

    //.............................................
    PRINTF ("# orig written\n\n");
    //'''''''''''''''''''''''''''''''''''''''''''''

    free (text);
    free (buffer);

    printf ("You can check sorted text in %s\n", out_file_name);
}


/**
 * @brief Asks user filenames
 *
 * Function writes info to the console
 * and asks user to write names input
 * and output files, then writes them
 * to intended string variables
 *
 * @param file_in pointer to string with name of input file
 * @param file_out pointer to string with name of output file
 */
void InitFileNames (char* file_in, char* file_out)
{
    ASSERT (file_in != NULL);
    ASSERT (file_out != NULL);

    printf ("Program sorts lines of your text\n\n");
    printf ("Please, enter the name of file with text: ");

    scanf ("%s", file_in);

    ASSERT (*file_in != 0);

    printf ("Now, please, enter the name of output file: ");

    scanf ("%s", file_out);

    ASSERT (*file_out != 0);

    printf ("\n");
}


/**
 * @brief Puts pointers of begin and end of string to each String
 *
 * Allocates memory for String array and matches begin and end of
 * each line to relevant String pointers
 *
 * @param buffer pointer to buffer
 * @param num_of_symbols number of symbols in buffer
 * @param pointer pointer to an array of Strings
 *
 * @return number of lines in text
 */
int ArrangePointers (char* buffer, const int num_of_symbols, String** pointer)
{
    ASSERT (buffer != NULL);
    ASSERT (pointer != NULL);

    const int num_of_lines = CharReplace (buffer, num_of_symbols, '\n', '\0');

    ASSERT (num_of_lines > 0);

    *pointer = (String*) calloc (num_of_lines + 1, sizeof (**pointer)); // +1 чтобы сделать последний указатель NULL

    String* curr = *pointer;

    while (curr - *pointer < num_of_lines)
    {
        curr->begin = buffer;

        while (*buffer != '\0')
        {
            ++buffer;
        }

        curr->end = buffer;

        ++buffer;
        ++curr;
    }

    curr->begin = NULL;
    curr->end = NULL;

    ASSERT (curr - *pointer == num_of_lines);

    return num_of_lines;
}


/**
 * @brief Sorts array
 *
 * @param begin_ptr pointer to beginning of the array
 * @param end_ptr pointer to begin of the last element of the array
 * @param cmp pointer to comparator function
 * @param type_size size of each element of array in bytes
 */
void QuickSort (void* begin_ptr, void* end_ptr, int (*cmp)(const void*, const void*), size_t type_size)
{
    ASSERT (begin_ptr != NULL);
    ASSERT (end_ptr != NULL);
    ASSERT (cmp != NULL);
    ASSERT (type_size != 0);

    const int SINGLE = 1;

    if (begin_ptr >= end_ptr)
    {
        return;
    }

    const int num_of_elems = ((char*)end_ptr - (char*)begin_ptr) / type_size + 1;
    const int rand_key = rand() % num_of_elems;

    void* pivot = calloc (SINGLE, type_size);

    CopyBytes ((char*)begin_ptr + rand_key * type_size, pivot, SINGLE, type_size);

    Swap (end_ptr, (char*)begin_ptr + rand_key * type_size, type_size);

    char* less_end = begin_ptr;

    for (char* ptr = begin_ptr; ptr < (char*)end_ptr; ptr += type_size)
    {
        if ((*cmp)(pivot, ptr) >= 0)
        {
            Swap(less_end, ptr, type_size);
            less_end += type_size;
        }
    }

    Swap (end_ptr, less_end, type_size);

    QuickSort (begin_ptr                  , (char*)less_end - type_size, cmp, type_size);
    QuickSort ((char*)less_end + type_size, end_ptr                    , cmp, type_size);
}


