//libx  by sincoder
//提供和windows api 相同功能相同参数的 api 来开发 linux 程序
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h> //for va_list
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>    //hostend
#include <arpa/inet.h>
#include <netinet/tcp.h>    //Provides declarations for tcp header
#include <netinet/ip.h>    //Provides declarations for ip header
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/rtnetlink.h>
#include <signal.h>

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif
#ifndef VOID
#define VOID void
typedef VOID *PVOID;
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#endif

#define near
#define far
#define CONST               const
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef void *HANDLE;

typedef CONST CHAR *LPCTSTR;

typedef long LONG_PTR, *PLONG_PTR;
typedef unsigned long ULONG_PTR, *PULONG_PTR;

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

#define SECTION_QUERY                0x0001
#define SECTION_MAP_WRITE            0x0002
#define SECTION_MAP_READ             0x0004
#define SECTION_MAP_EXECUTE          0x0008
#define SECTION_EXTEND_SIZE          0x0010
#define SECTION_MAP_EXECUTE_EXPLICIT 0x0020 // not included in SECTION_ALL_ACCESS

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
                            SECTION_MAP_WRITE |      \
                            SECTION_MAP_READ |       \
                            SECTION_MAP_EXECUTE |    \
                            SECTION_EXTEND_SIZE)

#define SESSION_QUERY_ACCESS  0x0001
#define SESSION_MODIFY_ACCESS 0x0002

#define SESSION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED |  \
                            SESSION_QUERY_ACCESS |             \
                            SESSION_MODIFY_ACCESS)

#define PAGE_NOACCESS          0x01
#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_WRITECOPY         0x08
#define PAGE_EXECUTE           0x10
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40
#define PAGE_EXECUTE_WRITECOPY 0x80
#define PAGE_GUARD            0x100
#define PAGE_NOCACHE          0x200
#define PAGE_WRITECOMBINE     0x400
#define PAGE_REVERT_TO_FILE_MAP     0x80000000
#define MEM_COMMIT                  0x1000
#define MEM_RESERVE                 0x2000
#define MEM_DECOMMIT                0x4000
#define MEM_RELEASE                 0x8000
#define MEM_FREE                    0x10000
#define MEM_PRIVATE                 0x20000
#define MEM_MAPPED                  0x40000
#define MEM_RESET                   0x80000
#define MEM_TOP_DOWN                0x100000
#define MEM_WRITE_WATCH             0x200000
#define MEM_PHYSICAL                0x400000
#define MEM_ROTATE                  0x800000
#define MEM_DIFFERENT_IMAGE_BASE_OK 0x800000
#define MEM_RESET_UNDO              0x1000000
#define MEM_LARGE_PAGES             0x20000000
#define MEM_4MB_PAGES               0x80000000
#define SEC_FILE           0x800000
#define SEC_IMAGE         0x1000000
#define SEC_PROTECTED_IMAGE  0x2000000
#define SEC_RESERVE       0x4000000
#define SEC_COMMIT        0x8000000
#define SEC_NOCACHE      0x10000000
#define SEC_WRITECOMBINE 0x40000000
#define SEC_LARGE_PAGES  0x80000000
#define SEC_IMAGE_NO_EXECUTE (SEC_IMAGE | SEC_NOCACHE)
#define MEM_IMAGE         SEC_IMAGE
#define WRITE_WATCH_FLAG_RESET  0x01
#define MEM_UNMAP_WITH_TRANSIENT_BOOST  0x01

//
// Define access rights to files and directories
//

//
// The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
// devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
// constants *MUST* always be in sync.
// The values are redefined in devioctl.h because they must be available to
// both DOS and NT.
//

#define FILE_READ_DATA            ( 0x0001 )    // file & pipe
#define FILE_LIST_DIRECTORY       ( 0x0001 )    // directory

#define FILE_WRITE_DATA           ( 0x0002 )    // file & pipe
#define FILE_ADD_FILE             ( 0x0002 )    // directory

#define FILE_APPEND_DATA          ( 0x0004 )    // file
#define FILE_ADD_SUBDIRECTORY     ( 0x0004 )    // directory
#define FILE_CREATE_PIPE_INSTANCE ( 0x0004 )    // named pipe


#define FILE_READ_EA              ( 0x0008 )    // file & directory

#define FILE_WRITE_EA             ( 0x0010 )    // file & directory

#define FILE_EXECUTE              ( 0x0020 )    // file
#define FILE_TRAVERSE             ( 0x0020 )    // directory

#define FILE_DELETE_CHILD         ( 0x0040 )    // directory

#define FILE_READ_ATTRIBUTES      ( 0x0080 )    // all

#define FILE_WRITE_ATTRIBUTES     ( 0x0100 )    // all

#define FILE_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x1FF)

#define FILE_GENERIC_READ         (STANDARD_RIGHTS_READ     |\
                                   FILE_READ_DATA           |\
                                   FILE_READ_ATTRIBUTES     |\
                                   FILE_READ_EA             |\
                                   SYNCHRONIZE)


#define FILE_GENERIC_WRITE        (STANDARD_RIGHTS_WRITE    |\
                                   FILE_WRITE_DATA          |\
                                   FILE_WRITE_ATTRIBUTES    |\
                                   FILE_WRITE_EA            |\
                                   FILE_APPEND_DATA         |\
                                   SYNCHRONIZE)


#define FILE_GENERIC_EXECUTE      (STANDARD_RIGHTS_EXECUTE  |\
                                   FILE_READ_ATTRIBUTES     |\
                                   FILE_EXECUTE             |\
                                   SYNCHRONIZE)

#define FILE_SHARE_READ                 0x00000001
#define FILE_SHARE_WRITE                0x00000002
#define FILE_SHARE_DELETE               0x00000004
#define FILE_ATTRIBUTE_READONLY             0x00000001
#define FILE_ATTRIBUTE_HIDDEN               0x00000002
#define FILE_ATTRIBUTE_SYSTEM               0x00000004
#define FILE_ATTRIBUTE_DIRECTORY            0x00000010
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020
#define FILE_ATTRIBUTE_DEVICE               0x00000040
#define FILE_ATTRIBUTE_NORMAL               0x00000080
#define FILE_ATTRIBUTE_TEMPORARY            0x00000100
#define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200
#define FILE_ATTRIBUTE_REPARSE_POINT        0x00000400
#define FILE_ATTRIBUTE_COMPRESSED           0x00000800
#define FILE_ATTRIBUTE_OFFLINE              0x00001000
#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000
#define FILE_ATTRIBUTE_ENCRYPTED            0x00004000
#define FILE_ATTRIBUTE_INTEGRITY_STREAM     0x00008000
#define FILE_ATTRIBUTE_VIRTUAL              0x00010000
#define FILE_ATTRIBUTE_NO_SCRUB_DATA        0x00020000
#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002
#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004
#define FILE_NOTIFY_CHANGE_SIZE         0x00000008
#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010
#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020
#define FILE_NOTIFY_CHANGE_CREATION     0x00000040
#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100
#define FILE_ACTION_ADDED                   0x00000001
#define FILE_ACTION_REMOVED                 0x00000002
#define FILE_ACTION_MODIFIED                0x00000003
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005
#define MAILSLOT_NO_MESSAGE             ((DWORD)-1)
#define MAILSLOT_WAIT_FOREVER           ((DWORD)-1)
#define FILE_CASE_SENSITIVE_SEARCH          0x00000001
#define FILE_CASE_PRESERVED_NAMES           0x00000002
#define FILE_UNICODE_ON_DISK                0x00000004
#define FILE_PERSISTENT_ACLS                0x00000008
#define FILE_FILE_COMPRESSION               0x00000010
#define FILE_VOLUME_QUOTAS                  0x00000020
#define FILE_SUPPORTS_SPARSE_FILES          0x00000040
#define FILE_SUPPORTS_REPARSE_POINTS        0x00000080
#define FILE_SUPPORTS_REMOTE_STORAGE        0x00000100
#define FILE_VOLUME_IS_COMPRESSED           0x00008000
#define FILE_SUPPORTS_OBJECT_IDS            0x00010000
#define FILE_SUPPORTS_ENCRYPTION            0x00020000
#define FILE_NAMED_STREAMS                  0x00040000
#define FILE_READ_ONLY_VOLUME               0x00080000
#define FILE_SEQUENTIAL_WRITE_ONCE          0x00100000
#define FILE_SUPPORTS_TRANSACTIONS          0x00200000
#define FILE_SUPPORTS_HARD_LINKS            0x00400000
#define FILE_SUPPORTS_EXTENDED_ATTRIBUTES   0x00800000
#define FILE_SUPPORTS_OPEN_BY_FILE_ID       0x01000000
#define FILE_SUPPORTS_USN_JOURNAL           0x02000000
#define FILE_SUPPORTS_INTEGRITY_STREAMS     0x04000000
#define FILE_INVALID_FILE_ID               ((LONGLONG)-1LL)

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)

#define _In_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_
#define _Out_

#ifdef _DEBUG
#define dbg_msg(fmt,...) do{\
        fprintf(stderr, "[libx]%s::%d::%s" fmt "\n", __FILE__, __LINE__,__func__,##__VA_ARGS__);\
    }while(0)
#else
#define dbg_msg(fmt,...)
#endif

typedef struct _OVERLAPPED
{
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union
    {
        struct
        {
            DWORD Offset;
            DWORD OffsetHigh;
        };
        PVOID  Pointer;
    };
    HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef struct _SECURITY_ATTRIBUTES
{
    DWORD  nLength;
    LPVOID lpSecurityDescriptor;
    BOOL   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

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
