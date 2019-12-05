#ifndef STREAM_HPP_
#define STREAM_HPP_

/**
 * @brief String
 *
 * String structure that contains pointers
 * to the begin of line and to the end of line ('\0')
 *
 * @param begin pointer to the begin of line
 * @param end pointer to the end of line
 */
struct String
{
    char* begin;
    char* end;
};

int CharReplace (char* buffer, const int len, char from_sample, char to_sample);
int ArrangePointers (char* buffer, const int num_of_symbols, String** pointer);

#endif // !STREAM_HPP_