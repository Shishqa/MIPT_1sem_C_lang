#include "head.hpp"

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
    ASSERT (buffer != nullptr);

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
    ASSERT (buffer != nullptr);
    ASSERT (pointer != nullptr);

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

    curr->begin = nullptr;
    curr->end = nullptr;

    ASSERT (curr - *pointer == num_of_lines);

    return num_of_lines;
}