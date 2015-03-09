// functions for reading set number of bytes from, writing set number of bytes to a handle 
// with non-overlapped I/O.
// includes error message generation and exception throwing when expected number of bytes not processes
//
#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <unistd.h>

void writeToFile(int, const void*, int, const char * const);
void readFromFile(int, void*, int, const char * const);

#endif
