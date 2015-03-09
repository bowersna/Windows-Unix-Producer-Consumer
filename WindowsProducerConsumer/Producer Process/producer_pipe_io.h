// producer-consumer using message passing synchronization - pipe i/o module
// adapted from https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
//
#ifndef __PRODUCER_PIPE_IO_H__
#define __PRODUCER_PIPE_IO_H__

#include <windows.h> 

void WriteToPipe(HANDLE, HANDLE, HANDLE, HANDLE);
void ReadFromPipe(HANDLE, HANDLE, HANDLE);

#endif