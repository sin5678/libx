#include "windows.h"

VOID Sleep(
    _In_  DWORD dwMilliseconds
)
{
    struct timespec slptm;
    slptm.tv_sec = dwMilliseconds / 1000;
    slptm.tv_nsec = 1000 * 1000 * (dwMilliseconds - (dwMilliseconds / 1000) * 1000);      //1000 ns = 1 us
    if (nanosleep(&slptm, NULL) != -1)
    {

    }
    else
    {
        dbg_msg("%s : %u", "nanosleep failed !!\n", dwMilliseconds);
    }
}

HANDLE CreateFile(
    _In_      LPCTSTR lpFileName,
    _In_      DWORD dwDesiredAccess,
    _In_      DWORD dwShareMode,
    _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_      DWORD dwCreationDisposition,
    _In_      DWORD dwFlagsAndAttributes,
    _In_opt_  HANDLE hTemplateFile
)
{
    int ret = -1;
    int oflag = 0;
    if (!lpFileName)
        return INVALID_HANDLE_VALUE;
    // test file access
    if (dwDesiredAccess & GENERIC_READ)
        oflag |= O_RDONLY;
    if (dwDesiredAccess & GENERIC_WRITE)
        oflag |= O_RDWR;
    if (dwDesiredAccess & GENERIC_EXECUTE)
        oflag |= O_RDWR;
    if (dwDesiredAccess & GENERIC_ALL)
        oflag |= O_RDWR;

    switch (dwCreationDisposition)
    {
    case CREATE_NEW:
        oflag |= O_CREAT;
        break;
    case OPEN_EXISTING:
        break;
    case CREATE_ALWAYS:
        oflag |= O_TRUNC;
        oflag |= O_CREAT;
        break;
    case OPEN_ALWAYS:
        oflag |= O_TRUNC;
        oflag |= O_CREAT;
        break;
    case TRUNCATE_EXISTING:
        oflag |= O_TRUNC;
        break;
    default:
        break;
    }

    ret = open(lpFileName, oflag);
    if (ret == -1)
    {
        return INVALID_HANDLE_VALUE;
    }
    return (HANDLE)ret;
}

BOOL WriteFile(
    _In_         HANDLE hFile,
    _In_         LPCVOID lpBuffer,
    _In_         DWORD nNumberOfBytesToWrite,
    _Out_opt_    LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_  LPOVERLAPPED lpOverlapped
)
{
    int BytesWritten = write((int)hFile, lpBuffer, nNumberOfBytesToWrite);
    if (BytesWritten >= 0)
    {
        if (lpNumberOfBytesWritten)
        {
            *lpNumberOfBytesWritten = BytesWritten;
        }

        return TRUE;
    }
    return FALSE;
}

BOOL ReadFile(
    _In_         HANDLE hFile,
    _Out_        LPVOID lpBuffer,
    _In_         DWORD nNumberOfBytesToRead,
    _Out_opt_    LPDWORD lpNumberOfBytesRead,
    _Inout_opt_  LPOVERLAPPED lpOverlapped
)
{
    int BytesRead = read((int)hFile, lpBuffer, nNumberOfBytesToRead);
    if (BytesRead >= 0)
    {
        if (lpNumberOfBytesRead)
        {
            *lpNumberOfBytesRead = BytesRead;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL DeleteFile(
    _In_  LPCTSTR lpFileName
)
{
    return 0 == unlinkat(AT_FDCWD, lpFileName, 0);
}
