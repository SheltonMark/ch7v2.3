#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <dirent.h>

#include <linux/unistd.h>

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include <termios.h>
 #include <sys/vfs.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/times.h>
#include <sys/mount.h>
#include <sys/time.h>
#include <sys/sysinfo.h>

#include <linux/if_ether.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "hifb.h"

#ifdef UCLINUX
#include "../PPSocket/ezsocket/skel/skel_uclinux.h"
#endif //UCLINUX

#ifdef PSOS
#include "../PPSocket/ezsocket/skel/skel_psos.h"
#endif

#include "sdk_comm.h"

typedef unsigned int			uint32;
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef signed int				LONG;
typedef unsigned int			DWORD;
typedef unsigned int			PARAM;
typedef void *				HANDLE;
typedef int					BOOL;
typedef int					bool;
typedef unsigned long long		UQWORD;
typedef long long				SQWORD;
typedef DWORD				COLORREF;//0x00bbggrr
typedef DWORD				COLORDEV;//0x????????
typedef int					INT;
typedef unsigned char                        U8;
typedef unsigned short                      U16;
typedef unsigned int                           U32;

typedef char			CHAR;
typedef void 			VOID;
typedef char *     PCHAR;
typedef BYTE *     PBYTE;
typedef enum ReadWrite_e
{
 READ = 0,
 WRITE
}ReadWrite_e;



#define LINE_MAX_LENGTH		128
#define NAME_MAX_LENGTH 	         32


#define FOR(i, max)				for (i = 0; i < (max); i++)
#define BITMSK(bit)				(int)(1 << (bit))
#define INTEGER_MIN (int)(0x80000000)
#define INTEGER_MAX (int)(0x7FFFFFFF)
#define UINTEGER_MAX (unsigned int)0xFFFFFFFF
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define MAKEDWORD(a, b, c, d) ((DWORD)(((BYTE)(a)) | ((BYTE)(b) << 8) | ((BYTE)(c) << 16) | ((BYTE)(d) << 24)))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define TRUE					1
#define FALSE				0

#ifndef NULL
#define NULL					0
#endif

#define ON						1
#define OFF						0
#define NC						0	//常关

#ifndef PSOS
#define NO						1	//常开
#endif

#ifndef MAX
#define MAX(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)				(((a) < (b)) ? (a) : (b))
#endif


typedef struct {
   int left;
   int top;
   int right;
   int bottom;
}RECT,*PRECT;

typedef struct {
	int x;
	int y;
}POINT,*PPOINT;

typedef struct {
	int w;
	int h;
}SIZE,*PSIZE;

typedef const RECT* PCRECT;
typedef const POINT* PCPOINT;
typedef const SIZE* PCSIZE;
typedef const char* PCSTR;

//时间结构
typedef struct _DHTIME								
{
	DWORD second		:6;					//	秒	1-60		
	DWORD minute		:6;					//	分	1-60		
	DWORD hour			:5;					//	时	1-24		
	DWORD day			:5;					//	日	1-31		
	DWORD month		:4;					//	月	1-12		
	DWORD year			:6;					//	年	2000-2063	

}DHTIME,*pDHTIME;

#if 0
typedef union {	//IP addr
	BYTE	c[4];
	WORD	s[2];
	DWORD	l;
} IPADDR;

typedef struct tagTIMESECTION
{
	//!使能
	BOOL enable;
	//!开始时间:小时
	int startHour;
	//!开始时间:分钟
	int	startMinute;
	//!开始时间:秒钟
	int	startSecond;
	//!结束时间:小时
	int	endHour;
	//!结束时间:分钟
	int	endMinute;
	//!结束时间:秒钟
	int	endSecond;
}TIMESECTION;	
#endif
//#define RGB(r,g,b)		((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define RGBA(r,g,b,a)	((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)|(((DWORD)(BYTE)(a))<<24)))
#define GetRValue(c)	((BYTE)(c))
#define GetGValue(c)	((BYTE)(((WORD)(c)) >> 8))
#define GetBValue(c)	((BYTE)((c)>>16))
#define GetAValue(c)	((BYTE)((c)>>24))


//#define DEBUG

#ifdef DEBUG
#define	PDBUG	do { PRINT_TRACE("__FILE__ = %s, __FUNCTION__ = %s, __LINE__ = %d\n",__FILE__, __FUNCTION__, 	__LINE__); } while (0)
#else
#define	PDBUG 
#endif

#define PRINT_TIME(s,x) \
	do{PRINT_TRACE("%s[%d-%d-%d %d:%d:%d]\n", \
	(s), (x).year, (x).month, (x).day, (x).hour, (x).minute, (x).second);}while(0)


#define MAX_NAME_LENGTH		32

//例如
//1.PRINT_TIME("",dhtime);
//2.PRINT_TIME("systimeX:",systimeX);

#endif

