#include "error_codes.h"

// associative arrays that map error codes for fork, exec, and wait to descriptive strings
//
std::map<int, std::string> forkErrorCodes {
    { EAGAIN, "(1) can't allocate sufficient memory for child, or (2) child process imit reached" },
    { ENOMEM, "insufficient kernel memory" }
};
std::map<int, std::string> execErrorCodes {
    { E2BIG,        "The total number of bytes in the environment (envp) and argument list (argv) is too large" },
    { EACCES,       "Search permission is denied, file isn't a regular file, or execute permission denied" },
    { EFAULT,       "The named file's filename points outside user's accessible address space" },
    { EINVAL,       "An ELF executable tried to name more than one interpreter" },
    { EIO,          "An I/O error occurred" },
    { EISDIR,       "An ELF interpreter was a directory" },
    { ELIBBAD,      "An ELF interpreter was not in a recognized format" },
    { ELOOP,        "Too many symbolic links were encountered in resolving filename or the name of a script or ELF interpreter" },
    { EMFILE,       "The process has the maximum number of files open" },
    { ENAMETOOLONG, "The named file's filename is too long", },
    { ENFILE,       "The system limit on the total number of open files has been reached" },
    { ENOENT,       "The named file or a script or ELF interpreter does not exist, or a supporting shared library can't be found" },
    { ENOEXEC,      "An  executable is in an unrecognized format, is for the wrong architecture, or has some other format error" },
    { ENOMEM,       "Insufficient kernel memory" },
    { ENOTDIR,      "A component of the path prefix of the named file or a script or ELF interpreter is not a directory" },
    { EPERM,        "The file system is mounted nosuid, the user isn't superuser, and the file has the set-user-ID or set-group-ID bit set" },
    { ETXTBSY,      "The executable was open for writing by one or more processes" }
};
std::map<int, std::string> pipeErrorCodes {
    { EMFILE, "The process has the maximum number of files open" },
    { ENFILE, "The system limit on the total number of open files has been reached" },
};
std::map<int, std::string> dup2ErrorCodes {
    { EBADF,    "oldfd isn't an open file descriptor, or newfd is out of the allowed range for file descriptors" },
    { EBUSY,    "race condition with open(2) and dup()" },
    { EINTR,    "dup2() was interrupted by a signal" },
    { EINVAL,   "flags contained an invalid value, or oldfd equals newfd" },
    { EMFILE,   "The process has the maximum number of files open" }
};
std::map<int, std::string> openErrorCodes {
    { EACCES,       "The requested access is not allowed, search permission is denied for a directory along the path to the file, or permission is lacking to create the file" },
    { EEXIST,       "pathname already exists and O_CREAT and O_EXCL were used." },
    { EFAULT,       "pathname references memory outside your accessible address space" },
    { EFBIG,        "An attempt was made to exceed the implementation-defined maximum file size or the process's file size limit, or to write at a position past the maximum allowed offset." },
    { EINTR,        "The call was interrupted by a signal while waiting on a slow device" },
    { EISDIR,       "pathname refers to a directory and the access requested involved writing" },
    { ELOOP,        "Too many symbolic links were encountered in resolving filename" },
    { EMFILE,       "The process has the maximum number of files open" },
    { EISDIR,       "fd refers to a directory" },
    { ENAMETOOLONG, "pathname was too long" },
    { ENFILE,       "The system limit on the total number of open files has been reached" },
    { ENODEV,       "pathname refers to a device special file and no corresponding device exists" },
    { ENOENT,       "O_CREAT is not set and the named file does not exist, or a directory component in pathname does not exist or is a dangling symbolic link." },
    { ENOMEM,       "Insufficient kernel memory" },
    { ENOSPC,       "The device containing the file referred to by fd has no room for the file." },
    { ENOTDIR,      "A component of the path prefix of the named file or or O_DIRECTORY was specified and pathname was not a directory." },
    { ENXIO,        "O_NONBLOCK | O_WRONLY is set, the file is a FIFO and no process has the file open for reading.  Or, the file is a device special file and no corresponding device exists." },
    { EOVERFLOW,    "pathname refers to a regular file that is too large to be opened." },
    { EPERM,        "The O_NOATIME flag was specified, but the caller's effective user ID didn't match the file's owner and the caller was not privileged" },
    { EROFS,        "pathname refers to a file on a read-only file system and write access was requested." },
    { ETXTBSY,      "pathname refers to an executable image which is currently being executed and write access was requested" },
    { EWOULDBLOCK,  "The O_NONBLOCK flag was specified, and an incompatible lease was held on the file" }
};
std::map<int, std::string> readErrorCodes {
    { EAGAIN,       "The file descriptor fd refers to a file other than a socket and the read would block" },
    { EWOULDBLOCK,  "The file descriptor fd refers to a file other than a socket and the read would block" },
    { EBADF,        "fd is not a valid file descriptor or is not open for reading" },
    { EFAULT,       "buf is outside your accessible address space" },
    { EINTR,        "The call was interrupted by a signal before any data was read" },
    { EBUSY,        "race condition with open(2) and dup()" },
    { EINVAL,       "fd is attached to an unreadable object; or the file was opened with the O_DIRECT flag, and a read parameter isn't suitably aligned" },
    { EIO,          "I/O error" },
    { EISDIR,       "fd refers to a directory" }
};
std::map<int, std::string> writeErrorCodes {
    { EAGAIN,       "The file descriptor fd refers to a file other than a socket and the write would block" },
    { EWOULDBLOCK,  "The file descriptor fd refers to a file other than a socket and the write would block" },
    { EBADF,        "fd is not a valid file descriptor or is not open for writing" },
    { EFBIG,        "An attempt was made to exceed the implementation-defined maximum file size or the process's file size limit, or to write at a position past the maximum allowed offset." },
    { EINTR,        "The call was interrupted by a signal before any data was read" },
    { EBUSY,        "race condition with open(2) and dup()" },
    { EINVAL,       "fd is attached to an unwritable object; or the file was opened with the O_DIRECT flag, and a write parameter isn't suitably aligned" },
    { EIO,          "I/O error" },
    { ENOSPC,       "The device containing the file referred to by fd has no room for the data." },
    { EPIPE,        "fd is connected to a pipe or socket whose reading end is closed. " },
    { EISDIR,       "fd refers to a directory" }
};
std::map<int, std::string> fstatErrorCodes {
    { EACCES,       "Search permission is denied for one of the directories in the path  prefix  of  path." },
    { EBADF,        "fd is not a valid file descriptor" },
    { EFAULT,       "pathname references memory outside your accessible address space" },
    { ELOOP,        "Too many symbolic links were encountered in resolving filename" },
    { ENAMETOOLONG, "pathname was too long" },
    { ENOENT,       "A component of path does not exist, or path is an empty string." },
    { ENOMEM,       "Insufficient kernel memory" },
    { ENOTDIR,      " A component of the path prefix of path is not a directory" },
    { EOVERFLOW,    "pathname refers to a regular file that is too large to be opened." }
};

