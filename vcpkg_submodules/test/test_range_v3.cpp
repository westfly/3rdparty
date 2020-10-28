//#include <range/v3/all.hpp>
//using namespace ranges;

#define PrintHelp()        \
    do {                   \
    #if DEBUG              \
        printf("outof\n"); \
    #else                  \
        printf("inxof\n"); \
    #endif                 \
    } while(0)

PrintHelp()