#ifndef STREAMLIB_H
#define STREAMLIB_H

typedef struct
{
    char* begin;
    char* end;
} String;

int Read (char** buffer, FILE* stream);
void WriteLines (String* text, char* file_name, const char* mode);
int Write (char* buffer, FILE* stream);
int CharReplace (char* buffer, const int len, char from_sample, char to_sample);
char ToLowerCase (char c);
int IsSortable (int c);
int CmpFront (String str1, String str2);
int comp_front (const void* a, const void* b);
int CmpBack (String str1, String str2);
int comp_back (const void* a, const void* b);
int StrCmp (const char* str1, const char* str2);
void Swap (void* buf1, void* buf2, size_t type_size);
void CopyBytes (void* from, void* to, const int num, size_t type_size);
int cmp_int (const void* a, const void* b);

#endif // STREAMLIB_H
