#ifndef HEAD_HPP_
#define HEAD_HPP_

#define RESET  "\033[0m"
#define RED    "\033[1;31m"
#define YELLOW "\033[3;33m"

#define SETCOLOR(color)             \
        printf ("%s", color)        \

#define DEBUG meow

#ifdef DEBUG
#define PRINT( str )                    \
        {                               \
            SETCOLOR (YELLOW);          \
            printf ( str );             \
            SETCOLOR (RESET);           \
        }

#else
#define PRINT( str )          \
        if (0) printf ( str )
#endif // DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <ctype.h>
#include "assert_ext.h"
#include "streamlib.hpp"
#include "ms_stream.h"

#include "tree.h"

#include "OK_Token.hpp"
#include "OK_Math_operators.hpp"

#endif // !HEAD_HPP_
