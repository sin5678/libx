#define _DEBUG
#include "../windows.h"

int main()
{
    WIN32_FIND_DATA wfd;
    HANDLE hfind = FindFirstFile("/", &wfd);
    if (hfind != INVALID_HANDLE_VALUE)
    {
        do
        {
            printf("--%s--\n", wfd.cFileName);
        }
        while (FindNextFile(hfind, &wfd));
        FindClose(hfind);
    }
    return 0;
}