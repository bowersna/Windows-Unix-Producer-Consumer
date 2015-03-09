// producer-consumer using message passing synchronization - pipe i/o module
//
#ifndef __PRODUCER_PIPE_IO_H__
#define __PRODUCER_PIPE_IO_H__

#include <unistd.h> 

void WriteToPipe(int, int, int, int);
void ReadFromPipe(int, int, int);

#endif