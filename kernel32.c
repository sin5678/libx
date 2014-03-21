#include "windows.h"
#include "help.h"
#include "kernel32.h"


typedef struct _FIND_FILE_HANDLE
{
    int fp; // 文件夹的 fd
    unsigned int idx; //文件的编号
}FIND_FILE_HANDLE;


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

HANDLE FindFirstFile(
    _In_   LPCTSTR lpFileName,
    _Out_  LPWIN32_FIND_DATA lpFindFileData
)
{
    int DirFp = -1;
    if (!lpFileName)
        return INVALID_HANDLE_VALUE;
    DirFp = openat(AT_FDCWD, lpFileName, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (DirFp)
    {
        if (lpFindFileData)
        {
            FIND_FILE_HANDLE *pHandle = (FIND_FILE_HANDLE *)malloc(sizeof(FIND_FILE_HANDLE));
            if (pHandle)
            {
                memset(lpFindFileData, 0, sizeof(WIN32_FIND_DATA));
                strcpy(lpFindFileData->cFileName, "."); // fake first file name
                lpFindFileData->dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;

                pHandle->fp = DirFp;
                pHandle->idx = 0;
                return (HANDLE)pHandle;
            }
        }
        close(DirFp);
    }
    return INVALID_HANDLE_VALUE;
}

struct linux_dirent
{
    long           d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[];
};

BOOL FindNextFile(
    _In_   HANDLE hFindFile,
    _Out_  LPWIN32_FIND_DATA lpFindFileData
)
{
    FIND_FILE_HANDLE *pHandle = (FIND_FILE_HANDLE *)hFindFile;
    if (pHandle && lpFindFileData)
    {
        struct linux_dirent *d;
        char buff[1024];
        //int i = 0;
        int ok = 0;
        while ( 1 != ok )
        {
            int bpos, nread;
            char d_type;
            int inputLen = 1;
            // get one file
            while (1)
            {
                nread = syscall(SYS_getdents, pHandle->fp, buff, inputLen);
                if (nread == -1 )
                {
                    if (errno == EINVAL)
                    {
                        inputLen ++;
                        continue;
                    }
                }
                break;
            }

            if (nread == -1)
            {
                dbg_msg("getdent error,error no : %d ", errno);
                break;
            }
            if (nread == 0)
                break;

            for (bpos = 0; bpos < nread;)
            {
                d = (struct linux_dirent *) (buff + bpos);
                if (strcmp(d->d_name, ".") == 0 )
                {
                    pHandle->idx ++;// skip .
                }
                else
                {
                    d_type = *(buff + bpos + d->d_reclen - 1);
                    lpFindFileData->dwFileAttributes =
                        (d_type == DT_DIR ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL);
                    libx_strncpy(lpFindFileData->cFileName,
                                 d->d_name,
                                 sizeof(lpFindFileData->cFileName));
                    ok = 1;
                    break;
                }
                bpos += d->d_reclen;
            }
        }

        if (ok)
        {
            pHandle->idx ++;
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FindClose(
    _Inout_  HANDLE hFindFile
)
{
    if (hFindFile)
    {
        FIND_FILE_HANDLE *p = (FIND_FILE_HANDLE *)hFindFile;
        close(p->fp);
        free(p);
        return TRUE;
    }
    return FALSE;
}

DWORD GetFileAttributes(
    _In_  LPCTSTR lpFileName
)
{
    DWORD ret = INVALID_FILE_ATTRIBUTES;
    struct stat sb;
    if (stat(lpFileName, &sb) == -1)
    {
        switch (sb.st_mode & S_IFMT)
        {
        case S_IFBLK:
            break;
        case S_IFCHR:
            break;
        case S_IFDIR:
            ret = FILE_ATTRIBUTE_DIRECTORY;
            break;
        case S_IFIFO:
            break;
        case S_IFLNK:
            break;
        case S_IFREG:
            ret = FILE_ATTRIBUTE_NORMAL ;
            break;
        case S_IFSOCK:
            break;
        default:
            break;
        }
    }
    return ret;
}

BOOL CopyFile(
    _In_  LPCTSTR lpExistingFileName,
    _In_  LPCTSTR lpNewFileName,
    _In_  BOOL bFailIfExists
)
{
    return FALSE;
}

DWORD GetFileSize(
    _In_       HANDLE hFile,
    _Out_opt_  LPDWORD lpFileSizeHigh
)
{
    DWORD lowPart = 0;
    int fd = (int)hFile;
    char *fileName = libx_GetFileNameFromFd(fd);
    if(fileName)
    {
        struct stat sb;
        if (stat(fileName, &sb) != -1) 
        {
            FILETIME ft;
            memcpy(&ft, &sb.st_size, sizeof(ft));
            if(lpFileSizeHigh)
            {
                *lpFileSizeHigh = ft.dwHighDateTime;
                lowPart = ft.dwLowDateTime;
            }
        }
        free(fileName);
    }
    return lowPart;
}