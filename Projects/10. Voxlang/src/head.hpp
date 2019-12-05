#ifndef HEAD_HPP_
#define HEAD_HPP_

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
#include "assert_ext.h"
#include "streamlib.hpp"
#include "ms_stream.h"

#include "tree.h"

#include "operators.hpp"
#include "Token.hpp"

#endif // !HEAD_HPP_
