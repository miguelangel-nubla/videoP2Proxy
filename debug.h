#ifndef DEBUG_H_
#define DEBUG_H_

#if 1
    #define    DPRINTF(f,...)    fprintf(stderr,f,##__VA_ARGS__)
#else
    #define    DPRINTF(f,...)
#endif

#endif