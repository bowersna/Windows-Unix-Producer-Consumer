// Format a system error message code as a string
// Parameters:
//      a map that maps error codes to strings, to use as a basis for error message generation 
//      an int that corresponds to a POSIX standard error code
// Return value:  a <string> object containing a message for the specified string, or "unknown error" if the code is missing from the map

#include <string>
#include <sstream>

using namespace std;

#include "format_error_message.h"

string format_error_message(map<int, string> errmap, int errcode) {
   const bool isUnknownError{ errmap.find(errcode) == errmap.end() };    // test for error in error table
   stringstream errmsg_builder;
   errmsg_builder << (isUnknownError ? "unknown error: " : errmap[errcode].c_str());
   if (isUnknownError) errmsg_builder << errcode;     // include code with "unknown error" messages
   return string{ errmsg_builder.str() };             // extract formatted string from stringstream, return
}

