/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_STRING_H_
#define _NIAL_STRING_H_

#include <stdarg.h>

#ifndef _NIAL_VA_LIST
#define _NIAL_VA_LIST
#define nial_va_list         va_list
#define nial_va_arg(ap, T)   va_arg(ap, T)
#define nial_va_end(ap)      va_end(ap)
#define nial_va_start(ap, A) va_start(ap, A)
#endif

char *nial_strcpy(char *s1, const char *s2);

char *nial_strncpy(char *s1, const char *s2, int size);

int nial_strlcpy(char *s1, const char *s2, int size);

char *nial_strcat(char *s1, const char *s2);

char *nial_strncat(char *s1, const char *s2, int size);

int nial_strlcat(char *s1, const char *s2, int size);

int nial_strcmp(const char *s1, const char *s2);

int nial_strncmp(const char *s1, const char *s2, int size);

int nial_strnicmp(const char *s1, const char *s2, int size);

int nial_strcasecmp(const char *s1, const char *s2);

int nial_strncasecmp(const char *s1, const char *s2, int n);

char *nial_strchr(const char *s, int n);

char *nial_strnchr(const char *s, int count, int c);

char *nial_strrchr(const char *s, int c);

char *nial_skip_spaces(const char *s);

char *nial_strim(char *s);

char *nial_strstrip(char *str);

char *nial_strstr(const char *s1, const char *s2);

char *nial_strnstr(const char *s1, const char *s2, int n);

int nial_strlen(const char *s);

int nial_strnlen(const char *s, int size);

char *nial_strpbrk(const char *s1, const char *s2);

char *nial_strsep(char **s, const char *ct);

int nial_strspn(const char *s, const char *accept);

int nial_strcspn(const char *s, const char *reject);

void *nial_memset(void *str, int c, int count);

void *nial_memcpy(void *s1, const void *s2, int count);

void *nial_memmove(void *s1, const void *s2, int count);

void *nial_memscan(void *addr, int c, int size);

int nial_memcmp(const void *cs, const void *ct, int count);

void *nial_memchr(const void *s, int c, int n);

void *nial_memchr_inv(const void *s, int c, int n);

unsigned long long nial_strtoull(const char *cp, char **endp, unsigned int base);

unsigned long nial_strtoul(const char *cp, char **endp, unsigned int base);

long nial_strtol(const char *cp, char **endp, unsigned int base);

long long nial_strtoll(const char *cp, char **endp, unsigned int base);

int nial_snprintf(char *buf, int size, const char *fmt, ...);

int nial_scnprintf(char *buf, int size, const char *fmt, ...);

int nial_sprintf(char *buf, const char *fmt, ...);

int nial_sscanf(const char *buf, const char *fmt, ...);

int nial_vsnprintf(char *str, int size, const char *fmt, nial_va_list args);

#endif /* _NIAL_STRING_H_ */
