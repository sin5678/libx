#pragma once
CHAR *libx_strncat(CHAR *d, CHAR *s, int n);
CHAR *libx_strncpy(CHAR *d, const CHAR *s, int n);
char *libx_GetLinkName(const char *linkName);
char *libx_GetFormatString(const char *fmt,...);
char *libx_GetFileNameFromFd(int fd);