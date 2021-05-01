/**
 * @file streamlib.c
 */


#include <stdio.h>
#include <stdlib.h>
#include "assert.h"


const int IGNORE_REGISTER = 1;


/**
 * @brief String
 *
 * String structure that contains pointers
 * to the begin of line and to the end of line ('\0')
 *
 * @param begin pointer to the begin of line
 * @param end pointer to the end of line
 */
typedef struct
{
    char* begin;
    char* end;
} String;


/**
 * @brief Reads text from file to buffer
 *
 * Function allocates memory in buffer needed
 * to store all symbols from file and then copy
 * symbols from file to buffer
 *
 * fgets() + memory allocate analog
 *
 * @param buffer pointer to buffer
 * @param stream FILE* pointer to opened stream
 *
 * @return number of symbols in buffer
 */
int Read (char** buffer, FILE* stream)
{
    ASSERT (buffer != NULL);
    ASSERT (stream != NULL);

    fseek (stream, 0L, SEEK_END);            //Курсор в конец потока
    int file_len = ftell (stream);           //Положение курсора == размер потока
    fseek (stream, 0L, SEEK_SET);            //курсор в начало потока

    *buffer = (char*) calloc (file_len + 2, sizeof (**buffer));    //file: a, b, c, EOF [file_len == 3] || mem: a, b, c, \n, EOF
    char* curr = *buffer;

    while (curr - *buffer < file_len)
    {
        *curr = fgetc (stream);   //Считывание символов из потока

        if (curr == *buffer + file_len - 1 && curr[0] != '\n')   //Обработка случая, когда последний символ - не перенос
        {
            curr[1] = '\n';   // _ _ _ _ a b [0]c    EOF
            ++file_len;       // _ _ _ _ a b    c [1]\n  EOF
            ++curr;
        }

        ++curr;
    }

    *curr = EOF;

    return file_len; // *(buffer + file_len) показывает на последний символ перед EOF
}


/**
 * @brief Writes text from buffer to file
 *
 * Function copy all symbols from buffer to file.
 * Read to '\0' or EOF
 *
 * fputs() analog
 *
 * @param buffer pointer to buffer
 * @param stream pointer to opened file
 *
 * @return number of symbols written or EOF in case of error
 */
int Write (char* buffer, FILE* stream)
{
    ASSERT (buffer != NULL);
    ASSERT (stream != NULL);

    char* curr = buffer;

    while (*curr && *curr != EOF) // Закончится когда придет в '\0' или EOF
    {
        if (fputc (*curr, stream) == EOF)
        {
            return EOF;
        }
        ++curr;
    }
    fputc ('\n', stream);

    return curr - buffer;
}


/**
 * @brief Replaces specific symbols in string to another
 *
 * Function changes every symbol in buffer that is equal to
 * from_sample to symbol to_sample
 *
 * @param buffer buffer pointer
 * @param len number of symbols to check
 * @param from_sample replaceable symbol
 * @param to_sample symbol for which we replace
 *
 * @return number of replacements
 */
int CharReplace (char* buffer, const int len, char from_sample, char to_sample)
{
    ASSERT (buffer != NULL);

    int replaced_cnt = 0;
    char* ptr = buffer;

    while (ptr < buffer + len)
    {
        ASSERT (*ptr != EOF);

        if (*ptr == from_sample)
        {
            *ptr = to_sample;
            ++replaced_cnt;
        }
        ++ptr;

        ASSERT (ptr <= buffer + len);
    }

    return replaced_cnt;
}


/**
 * @brief Swaps two variables
 *
 * Function swaps two variables
 *
 * @param buf1 pointer to the start of the first variable
 * @param buf2 pointer to the start of the second variable
 * @param type_size size of types of variables in bytes
 */
void Swap (void* buf1, void* buf2, size_t type_size)
{
    ASSERT (buf1 != NULL);
    ASSERT (buf2 != NULL);
    ASSERT (type_size > 0);

    const int SINGLE = 1;

    if (buf1 == buf2)
    {
        return;
    }

    char* tmp = (char*) calloc (SINGLE, sizeof (*tmp));
    char* ptr2 = (char*) buf2;

    for (char* ptr1 = (char*) buf1; ptr1 < (char*) buf1 + type_size; ++ptr1, ++ptr2)
    {
        *tmp  = *ptr1;
        *ptr1 = *ptr2;
        *ptr2 = *tmp;
    }

    free (tmp);
}


/**
 * @brief Check if symbol is letter or digit
 *
 * Function checks if char symbol is letter or digit
 *
 * @param c checkable character
 *
 * @return 1 if character is sortable
 *         0 if character is non-sortable
 */
int IsSortable (int c)
{
    return ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') );

            /*
            (c >= 'а' && c <= 'я') ||
            (c >= 'А' && c <= 'Я') );
            */
}


/**
 * @brief Turns upper case to lower case
 *
 * If function gets a letter in upper case, returns
 * this letter in lower case, else does nothing
 *
 * tolower() analog
 *
 * @param c character
 *
 * @return character in lower case
 */
char ToLowerCase (char c)
{
    if (!IGNORE_REGISTER)
    {
        return c;
    }

    if ('A' <= c && c <= 'Z')
    {
        return (char) (c + ' ');
    }
    return c;
}


/**
 * @brief Compares two Strings
 *
 * Function compares two Strings lexicographically
 * from beginnings
 *
 * @param str1 the first string
 * @param str2 the second string
 *
 * @return positive number if str1 > str2,
 *         zero if str1 == str2,
 *         negative number if str1 < str2
 */
int CmpFront (String str1, String str2)
{
    ASSERT (str1.begin != NULL);
    ASSERT (str1.end != NULL);
    ASSERT (str2.begin != NULL);
    ASSERT (str2.end != NULL);

    if (str1.begin == str2.begin) // Если указатели на начало совпадают => это одна и та же строка
    {
        return 0;
    }

    while ((*str1.begin == ' ' || *str1.begin == '\t') && str1.begin < str1.end) // Игнорируем пробелы и табы в начале строки
    {
        ++str1.begin;
    }

    while ((*str2.begin == ' ' || *str2.begin == '\t') && str2.begin < str2.end)
    {
        ++str2.begin;
    }

    while (ToLowerCase (*str1.begin) == ToLowerCase (*str2.begin))
    {
        if (*str1.begin == '\0' || *str2.begin == '\0')
        {
            break;
        }
        ++str1.begin;
        ++str2.begin;
    }
    return ToLowerCase (*str1.begin) - ToLowerCase (*str2.begin);
}


/**
 * @brief a wrap for CmpFront()
 */
int comp_front (const void* a, const void* b)
{
    return CmpFront (*(String*)a, *(String*)b);
}


/**
 * @brief Compares two Strings
 *
 * Function compares two Strings lexicographically
 * from endings, skipping non-letter and non-digit symbols
 *
 * @param str1 the first string
 * @param str2 the second string
 *
 * @return positive number if str1 > str2,
 *         zero if str1 == str2,
 *         negative number if str1 < str2
 */
int CmpBack (String str1, String str2)
{

    // "__aaa" > "aaa" потому что в теории после aaa в первой строке могут следовать другие символы

    ASSERT (str1.begin != NULL);
    ASSERT (str1.end != NULL);
    ASSERT (str2.begin != NULL);
    ASSERT (str2.end != NULL);

    if (str1.begin == str2.begin)
    {
        return 0;
    }

    while (1)
    {
        while (!IsSortable (*str1.end) && str1.end > str1.begin)
        {
            --str1.end;
        }
        while (!IsSortable (*str2.end) && str2.end > str2.begin)
        {
            --str2.end;
        }

        if (str1.end == str1.begin || str2.end == str2.begin)
        {
            if (IsSortable (*str1.end) && !IsSortable (*str2.end) ||
                IsSortable (*str1.end) &&  IsSortable (*str2.end) && str1.end > str1.begin)
            {
                return 1; // Плохая строка < Любая другая
            }
            if (!IsSortable (*str1.end) && IsSortable (*str2.end) ||
                 IsSortable (*str1.end) && IsSortable (*str2.end) && str2.end > str2.begin)
            {
                return -1;
            }
            break;
        }

        if (ToLowerCase (*str1.end) != ToLowerCase (*str2.end))
        {
            break;
        }

        --str1.end;
        --str2.end;

        ASSERT (str1.end >= str1.begin);
        ASSERT (str2.end >= str2.begin);
    }

    return ToLowerCase (*str1.end) - ToLowerCase (*str2.end);
}


/**
 *  @brief a wrap for CmpBack()
 */
int comp_back (const void* a, const void* b)
{
    ASSERT (a != NULL);
    ASSERT (b != NULL);

    return CmpBack (*(String*)a, *(String*)b);
}


/**
 * @brief Writes lines to file
 *
 * Function writes lines of text to file till text doesn't
 * point to NULL (it means that lines are over)
 *
 * @param text array of Strings
 * @param file_name name of output file
 * @param mode mode of writing ("a" - appending, "w" - rewriting)
 */
void WriteLines (String* text, char* file_name, const char* mode)
{
    ASSERT (text != NULL);
    ASSERT (file_name != NULL);
    ASSERT (mode != NULL);

    FILE* stream = fopen (file_name, mode);

    while (text->begin != NULL)
    {
        ASSERT (text->end != NULL)

        Write (text->begin, stream);
        ++text;
    }

    Write ("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%", stream);

    fclose (stream);
}


/**
 * @brief Compares two strings
 *
 * Function compares two strings lexicographically
 *
 * @param str1 the first string
 * @param str2 the second string
 *
 * @return positive number if str1 > str2,
 *         zero if str1 == str2,
 *         negative number if str1 < str2
 */
int StrCmp (const char* str1, const char* str2)
{
    ASSERT (str1);
    ASSERT (str2);

    if (str1 == str2)
    {
        return 0;
    }

    while (*str1 == *str2)
    {
        if (*str1 == '\0' || *str2 == '\0')
        {
            return 0;
        }
        ++str1;
        ++str2;
    }
    return *str1 - *str2;
}


/**
 * @brief copies bytes of memory plot to another memory plot
 *
 * @param from pointer to begin of domain
 * @param to pointer to begin of target
 * @param num of variables to copy
 * @param size of variable in bytes
 */
void CopyBytes (void* from, void* to, const int num, size_t type_size)
{
    ASSERT (from != NULL);
    ASSERT (to != NULL);
    ASSERT (type_size > 0);

    char* buf = (char*) to;

    for (char* ptr = (char*) from; ptr < (char*) from + num * type_size; ++ptr)
    {
        *buf = *ptr;
        ++buf;
    }

    return;
}


/**
 * @brief compares 2 integers
 *
 * @return difference
 */
int cmp_int (const void* a, const void* b)
{
    ASSERT (a != NULL);
    ASSERT (b != NULL);

    return *(int*)a - *(int*)b;
}


