#include "windows.h"
#include "help.h"
/*
字符串连接函数
@d 目标字符串
@s 源字符串
@n 目标最大可以保存的字符串数量 包括末尾的 \0
@return 返回连接后的字符串
*/
CHAR *libx_strncat(CHAR *d, CHAR *s, int n)
{
    int i = 0;
    for (i = strlen(d); i < n - 1 && *s; i++, s++)
    {
        d[i] = *s;
    }
    d[i] = 0;
    return d;
}


/*
字符串复制函数
@d 目标内存
@s 源字符串
@n 目标内存中可以存放的最多字符数量 保护末尾的 \0
@return 返回 d
*/
CHAR *libx_strncpy(CHAR *d, const CHAR *s, int n)
{
    int i = 0;
    for (i = 0; i < n - 1 && *s; i++, s++)
    {
        d[i] = *s;
    }
    d[i] = 0;
    return d;
}

/*
获取符号链接对应的文件名称
@return 返回值需要使用 free 释放 失败返回 NULL
*/
char *libx_GetLinkName(const char *linkName)
{
    char *buff = NULL;
    int len = 512;
    int ret = len;
    do
    {
        if (ret < len)
        {
            break;
        }
        if (buff)
            free(buff);
        len ++;
        buff = (char *)malloc(len + 1);
    }while (-1 != (ret = readlink(linkName, buff, len)));
    if(ret == -1)
    {
        if(buff)
            free(buff);
        return NULL;
    }
    buff[ret] = '\0';
    return buff;
}

char *libx_GetFormatString(const char *fmt,...)
{
    int len = 512;
    char *buff = NULL;
    va_list args;
    int n = len;
    va_start(args, fmt);
   
    do
    {
        if(buff)
            free(buff);
        len += 2;
        buff = (char *)malloc(len + 1);
        if(buff)
        {
            n = vsnprintf(buff, len, fmt, args);
        }
        else
            break;
    }while(n >= len);

     va_end(args);

     return buff;
}

/*
via /proc/self/fd/[fd] get file name info
*/
char *libx_GetFileNameFromFd(int fd)
{
    char *ret = NULL;
    char *linkName = libx_GetFormatString("/proc/self/fd/%d",fd);
    if(linkName)
    {
        char *name = libx_GetLinkName(linkName);
        if(name)
        {
            ret = name;
        }
        free(linkName);
    }
    return ret;
}
