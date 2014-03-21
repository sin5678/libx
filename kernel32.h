#pragma once


VOID Sleep(
    _In_  DWORD dwMilliseconds
);

/*
不支持的参数:
    lpSecurityAttributes
    hTemplateFile
    dwShareMode
*/
HANDLE CreateFile(
    _In_      LPCTSTR lpFileName,
    _In_      DWORD dwDesiredAccess,
    _In_      DWORD dwShareMode,
    _In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_      DWORD dwCreationDisposition,
    _In_      DWORD dwFlagsAndAttributes,
    _In_opt_  HANDLE hTemplateFile
);

/*
unsupport lparam:
    lpOverlapped
*/
BOOL WriteFile(
    _In_         HANDLE hFile,
    _In_         LPCVOID lpBuffer,
    _In_         DWORD nNumberOfBytesToWrite,
    _Out_opt_    LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_  LPOVERLAPPED lpOverlapped
);

/*

*/
BOOL ReadFile(
    _In_         HANDLE hFile,
    _Out_        LPVOID lpBuffer,
    _In_         DWORD nNumberOfBytesToRead,
    _Out_opt_    LPDWORD lpNumberOfBytesRead,
    _Inout_opt_  LPOVERLAPPED lpOverlapped
);

BOOL DeleteFile(
  _In_  LPCTSTR lpFileName
);

HANDLE FindFirstFile(
  _In_   LPCTSTR lpFileName,
  _Out_  LPWIN32_FIND_DATA lpFindFileData
);

BOOL FindNextFile(
  _In_   HANDLE hFindFile,
  _Out_  LPWIN32_FIND_DATA lpFindFileData
);

BOOL FindClose(
  _Inout_  HANDLE hFindFile
);

DWORD GetFileAttributes(
  _In_  LPCTSTR lpFileName
);

BOOL CopyFile(
  _In_  LPCTSTR lpExistingFileName,
  _In_  LPCTSTR lpNewFileName,
  _In_  BOOL bFailIfExists
);

DWORD GetFileSize(
  _In_       HANDLE hFile,
  _Out_opt_  LPDWORD lpFileSizeHigh
);
