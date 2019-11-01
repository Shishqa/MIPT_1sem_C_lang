#ifdef DEBUG
    #define ASSERT( cond );                                                                                                       \
    {                                                                                                                             \
        if (!(cond))                                                                                                              \
        {                                                                                                                         \
            printf ("Assertion failed: %s. File %s, line %d, function %s()\n\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            abort();                                                                                                              \
        }                                                                                                                         \
    }
    #define PRINTF printf
#else
    #define ASSERT( cond ); ;
    #define PRINTF if(0)printf
#endif // DEBUG
