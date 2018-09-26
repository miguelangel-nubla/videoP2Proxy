#ifndef COMMON_H_
#define COMMON_H_

#if 1
    #define    DPRINTF(f,...)    fprintf(stderr,f,##__VA_ARGS__)
#else
    #define    DPRINTF(f,...)
#endif


int MODE_RTSP, MODE_STDOUT;

#ifdef ENABLE_RTSP
char* MODE_RTSP_FIFO_FILE;
int MODE_RTSP_FIFO;
#endif

#endif