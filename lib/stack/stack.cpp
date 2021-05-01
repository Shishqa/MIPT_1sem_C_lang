#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "stack.h"


/**
 * @brief stack initialization
 * allocates memory for data stack
 * @param[in] this_ pointer to a Stack to initialize
 * @param[in] init_name name of Stack
 * @param[in] init_size size of memory to allocate in data (default = START_STACK_SIZE)
 * @return error number (0 if OK)
 */
Err_n Stack_Construct (Stack_t* this_, char* init_name, size_t init_size)
{
    ASSERT (this_ != nullptr)

    this_->data = (Elem_t*) calloc (init_size + $$( 2 ), sizeof (Elem_t));

    memset (this_->data + $$( 1 ), POISON_BYTE, init_size * sizeof (Elem_t));

    CANARY ( this_->data[0]             = CANARY_ELEM_VAL; )
    CANARY ( this_->data[init_size + 1] = CANARY_ELEM_VAL; )

    this_->mem_size = init_size;
    this_->size = 0;
    this_->err = OK_CODE;
    this_->is_empty = true;
    strncpy (this_->name, init_name, sizeof(this_->name) - 1);

    IF_HASHING ( StackHash (this_, GetHash); )

    ASSERT_OK (this_)

    return this_->err;
}


/**
 * @brief stack destruction
 * deallocates memory of data stack
 * @param[in] this_ pointer to a Stack to destruct
 * @return error number (0 if OK)
 */
Err_n StackDestruct (Stack_t* this_)
{
    ASSERT (this_ != nullptr)
    //ASSERT_OK (this_)

    if (this_->data != nullptr)
    {
        free (this_->data);
    }

    this_->err = DESTRUCTED_ERR_CODE;
    this_->size = 0;
    this_->mem_size = 0;
    this_->data = nullptr;
    this_->is_empty = true;

    IF_HASHING ( this_->str_hash = 0; )
    IF_HASHING ( this_->data_hash = 0; )

    return this_->err;
}


/**
 * @brief reallocation
 * allocates memory for data stack
 * @param[in] this_ pointer to a Stack to reallocate data
 * @param[in] new_size new size of data after reallocation
 * @return error number (0 if OK)
 */
Err_n StackRealloc (Stack_t* this_, size_t new_size)
{
    ASSERT (this_ != nullptr)
    ASSERT (new_size > 0)

    ASSERT_OK (this_)

    this_->data = (Elem_t*) realloc (this_->data, (new_size + $$( 2 )) * sizeof (Elem_t));

    if (this_->mem_size <= new_size)
    {
        memset (this_->data + this_->size + $$( 1 ), POISON_BYTE, (new_size - this_->mem_size + 1) * sizeof (Elem_t));
    }

    CANARY ( this_->data[new_size + 1] = CANARY_ELEM_VAL; )

    this_->mem_size = new_size;

    IF_HASHING ( StackHash (this_, GetHash); )

    ASSERT_OK (this_)

    return this_->err;
}


/**
 * @brief stack push
 * pushes an element to the Stack
 * @param[in] this_ pointer to the Stack
 * @param[in] value element to push
 * @return error number (0 if OK)
 */
Err_n StackPush (Stack_t* this_, Elem_t value)
{
    ASSERT (this_ != nullptr)
    ASSERT_OK (this_)

    if (this_->size >= this_->mem_size - 1)
    {
        StackRealloc (this_, this_->mem_size * MEM_RESIZE_RATIO);
    }

    this_->data[this_->size + $$( 1 )] = value;
    ++this_->size;
    this_->is_empty = false;

    IF_HASHING ( StackHash (this_, GetHash); )

    ASSERT_OK (this_)

    return this_->err;
}


/**
 * @brief stack top
 *
 * @param[in] this_ pointer to the Stack
 * @param[in] rec pointer to Elem_t variable to which top should be recorded
 *
 * @return error number (0 if OK)
 */
Err_n StackTop (Stack_t* this_, Elem_t* rec)
{
    ASSERT (this_  != nullptr)
    ASSERT (rec    != nullptr)

    ASSERT_OK (this_);

    if (this_->is_empty)
    {
        if (!STACK_SILENT)
        {
            StackDump (this_, "Trying to top empty stack", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            abort();
        }

        return EMPTY_ERR_CODE;
    }

    *rec = this_->data[this_->size - 1 + $$( 1 )];

    return this_->err;
}


/**
 * @brief stack pop
 * pops an element from the Stack
 * @param[in] this_ pointer to the Stack
 * @param[in] rec pointer to Elem_t variable to which pop should be recorded
 *
 * @return error number (0 if OK)
 */
Err_n StackPop (Stack_t* this_, Elem_t* rec)
{
    ASSERT (this_ != nullptr)
    ASSERT_OK (this_)

    if (this_->is_empty)
    {
        if (!STACK_SILENT)
        {
            StackDump (this_, "Trying to pop empty stack", __FILE__, __PRETTY_FUNCTION__, __LINE__);
            abort();
        }

        return EMPTY_ERR_CODE;
    }

    if (this_->size - 1 < this_->mem_size / RESIZE_GISTERESIS_RATIO)
    {
        StackRealloc (this_, this_->mem_size / MEM_RESIZE_RATIO);
    }

    *rec = this_->data[--this_->size + $$( 1 )];
    this_->data[this_->size + $$( 1 )] = POISON_VAL;

    if (!this_->size)
    {
        this_->is_empty = true;
    }

    IF_HASHING ( StackHash (this_, GetHash); )

    ASSERT_OK (this_)

    return this_->err;
}


/**
 * @brief check for destructed stack
 *
 * @param[in] this_ pointer to the Stack
 *
 * @return true  if stack is destructed
 *         false if stack is not destructed
 */
bool IsStackDestructed (Stack_t* this_)
{
    return ( this_->data == nullptr      &&
             this_->mem_size == 0        &&
             this_->size == 0            &&
             this_->is_empty == true        );
}


/**
 * @brief validation check of Stack
 *
 * @param[in] this_ pointer to the Stack
 *
 * @return error number (0 if OK)
 */
Err_n StackValid (Stack_t* this_)
{
    ASSERT (this_ != nullptr);

    if (IsStackDestructed(this_))
    {
        return DESTRUCTED_ERR_CODE;
    }

    if (!CheckHash (this_, GetHash))
    {
        IF_HASHING ( return HASH_ERR_CODE; )
    }

    if (!CheckCanaries (this_))
    {
        CANARY ( return CANARY_ERR_CODE; )
    }

    if (this_->data == nullptr)
    {
        return NON_INIT_ERR_CODE; // non initialized data
    }

    if (this_->size < 0 || this_->size >= this_->mem_size || this_->size > 0 && this_->is_empty)
    {
        return SIZE_ERR_CODE;
    }

    if (this_->mem_size < 1 || (!this_->is_empty && this_->mem_size / RESIZE_GISTERESIS_RATIO > this_->size))
    {
        return MEM_SIZE_ERR_CODE;
    }

    if (this_->err != OK_CODE)
    {
        return UNDEFINED_ERR_CODE;
    }

    return OK_CODE;
}


/**
 * @brief Dump
 *
 * @param[in] reason reason for which dump is called (default = "Undefined Dump")
 * @param[in] this_ pointer to the Stack
 * @param[in] active_file file from which dump is called (default = "undefined file")
 * @param[in] active_func function from which dump is called (default = "undefined func")
 * @param[in] active_line number of line from which dump is called (default = -1)
 */
void StackDump (Stack_t* this_, const char* reason, const char* active_file, const char* active_func, const int active_line)
{
    printf ("\n##################################################################\n");
    printf ("%s from file %s,\nfunction %s, line %d:\n\n", reason, active_file, active_func, active_line);
    printf ("Stack_t \"%s\" [0x%x]", this_->name, this_);

    if (!strcmp (reason, "Assertion"))
    {
        printf ("\e[0;31m");
        printf (" (not ok)");
        printf ("\e[0m");
    }

    printf ("\n{\n");
    CANARY ( printf ("\tcanary_left = %d\n", this_->canary_left) );
    printf("\tsize = %u\n", this_->size);
    printf ("\tdata[%u] = [0x%x]\n",   this_->mem_size, this_->data);
    printf ("\t\{\n");

    for (int i = 0; i < this_->mem_size + $$( 2 ); ++i)
    {
        if (this_->mem_size < 1 || this_->data == nullptr || $( this_->err == HASH_ERR_CODE ))
        {
            printf ("\e[0;31m");
            printf ("\t\tCan't show data :(\n");
            printf ("\e[0m");
            break;
        }
        if (this_->err == SIZE_ERR_CODE || this_->err == MEM_SIZE_ERR_CODE)
        {
            printf ("\e[0;33m");
            printf ("\t\tWarning: mem_size or size took an unexpected value\n");
            printf ("\e[0m");
            break;
        }
        printf ("\t\t%c ", ((i < this_->size + $$( 1 ) && i >= $$( 1 )) ? '*' : ' '));
        printf ("[%d] \t= %d ", (i >= $$( 1 ) && i <= this_->mem_size - 1 + $$( 1 )) ? (i - $$( 1 )) : (-1), this_->data[i]);
        printf ("%s", ((i == -1 + $$( 1 ) || i == this_->mem_size + 1) ? ("\t(CANARY)") : ("")));
        printf ("%s", ((this_->data[i] == POISON_VAL) ? ("\t\t(poison)\n") : ("\n")));
    }

    printf ("\t}\n");
    printf ("\terr = %d ", this_->err);

    switch (this_->err)
    {
                    case DESTRUCTED_ERR_CODE: printf ("(destructed)\n");
                                              break;
                    case OK_CODE:             printf ("\e[0;32m");
                                              printf ("(ok)\n");
                                              printf ("\e[0m");
                                              break;
                    case NON_INIT_ERR_CODE:   printf ("\e[0;31m");
                                              printf ("(non-initialized data)\n");
                                              printf ("\e[0m");
                                              break;
                    case SIZE_ERR_CODE:       printf ("\e[0;31m");
                                              printf ("(size points to not-allocated memory)\n");
                                              printf ("\e[0m");
                                              break;
                    case MEM_SIZE_ERR_CODE:   printf ("\e[0;31m");
                                              printf ("(mem_size doesn't match size of allocated memory)\n");
                                              printf ("\e[0m");
                                              break;
                    case EMPTY_ERR_CODE:      printf ("\e[0;31m");
                                              printf ("(popping empty stack)\n");
                                              printf ("\e[0m");
                                              break;
       IF_HASHING ( case HASH_ERR_CODE:       printf ("\e[0;31m");                )
       IF_HASHING (                           printf ("(hash has changed)\n");    )
       IF_HASHING (                           printf ("\e[0m");                   )
       IF_HASHING (                           break;                              )
           CANARY ( case CANARY_ERR_CODE:     printf ("\e[0;31m");                )
           CANARY (                           printf ("(canary redefined)\n");    )
           CANARY (                           printf ("\e[0m");                   )
           CANARY (                           break;                              )
                    default:                  printf ("\e[0;33m");
                                              printf ("(undefined code)\n");
                                              printf ("\e[0m");
                                              break;
    }

    IF_HASHING ( printf ("\tstr_hash  = 0x%x \n",  this_->str_hash); )
    IF_HASHING ( printf ("\tdata_hash = 0x%x \n", this_->data_hash); )
    CANARY ( printf ("\tcanary_right = %d\n", this_->canary_right); )

    printf ("}\n");
    printf ("##################################################################\n\n");

}


/**
 * @brief Stack hasher
 *
 * Writes hash of the current stack to this_->hash
 *
 * @param[in] this_ pointer to the Stack
 * @param[in] ByteHasher pointer to hashing function
 */
void StackHash (Stack_t* this_, unsigned int (*ByteHasher)(void*, size_t))
{
    IF_HASHING ( size_t size_of_stack = (char*)&this_->str_hash - (char*)&this_->size; )

    IF_HASHING ( this_->str_hash  = ByteHasher (&this_->size, size_of_stack); )
    IF_HASHING ( this_->data_hash = ByteHasher (this_->data + $$( 1 ), this_->mem_size * sizeof (Elem_t)); )
}


/**
 * @brief Polynomial hasher of memory block
 *
 * @param[in] buffer pointer to the block of memory
 * @param[in] num number of bytes to hash
 *
 * @return hash
 */
unsigned int GetPolyHash (void* buffer, size_t num)
{
    unsigned int res = 0;

    for (char* ptr = (char*)buffer; ptr < (char*)buffer + num; ++ptr)
    {
        IF_HASHING ( res = (res * POLY_HASHING_MULT + *ptr); )
    }

    return res;
}


/**
 * @brief MurMurHash2
 */
uint32_t GetMurMurHash2 (const uint8_t* key, size_t len)
{
    uint32_t h = 0x123c132b;
	if (len > 3) {
		size_t i = len >> 2;
		do {
			uint32_t k;
			memcpy(&k, key, sizeof(uint32_t));
			key += sizeof(uint32_t);
			k *= 0xcc9e2d51;
			k = (k << 15) | (k >> 17);
			k *= 0x1b873593;
			h ^= k;
			h = (h << 13) | (h >> 19);
			h = h * 5 + 0xe6546b64;
		} while (--i);
	}
	if (len & 3) {
		size_t i = len & 3;
		uint32_t k = 0;
		do {
			k <<= 8;
			k |= key[i - 1];
		} while (--i);
		k *= 0xcc9e2d51;
		k = (k << 15) | (k >> 17);
		k *= 0x1b873593;
		h ^= k;
	}
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}


/**
 * @brief A wrap for current hashing function
 */
unsigned int GetHash (void* buffer, size_t num)
{
    return GetMurMurHash2 ((uint8_t*)buffer, num);
}


/**
 * @brief Check if this_->hash is equal to current hash
 *
 * @param[in] this_ pointer to the Stack
 * @param[in] ByteHasher pointer to hashing function
 *
 * @return true  if hashes are equal
 * @return false if hashes are different
 */
bool CheckHash (Stack_t* this_, unsigned int (*ByteHasher)(void*, size_t))
{
    IF_HASHING ( size_t size_of_stack = (char*)&this_->str_hash - (char*)&this_->size; )
    IF_HASHING ( unsigned int expected_stack_hash = ByteHasher (&this_->size, size_of_stack); )

    IF_HASHING ( if (this_->str_hash != expected_stack_hash) return false; )

    IF_HASHING ( expected_stack_hash = ByteHasher (this_->data + $$( 1 ), this_->mem_size * sizeof (Elem_t)); )

    IF_HASHING ( return expected_stack_hash == this_->data_hash; )

    return true;
}


/**
 * @brief Check if this_ canaries are not touched
 *
 * @param[in] this_ pointer to the Stack
 *
 * @return true  if canaries are equal to CANARY_VAL
 *         false if at least one of canaries differs from CANARY_VAL
 */
bool CheckCanaries (Stack_t* this_)
{
    return ( !($$( this_->canary_left  != CANARY_VAL                   ) ||
               $$( this_->canary_right != CANARY_VAL                   ) ||
               $$( this_->data[0]                   != CANARY_ELEM_VAL ) ||
               $$( this_->data[this_->mem_size + 1] != CANARY_ELEM_VAL ) ) );
}

