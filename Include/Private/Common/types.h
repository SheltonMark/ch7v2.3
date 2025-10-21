#ifndef __TYPES_H__
#define __TYPES_H__




#define FAILED		-1
#define SUCCESS		0

#define FALSE		0
#define TRUE		(!FALSE)

#define VPSS_MAX_GRP_NUM 256

typedef signed char			schar;	
typedef unsigned char			uchar;
typedef unsigned char			__u8;

typedef unsigned int			uint;	
typedef unsigned short			ushort;	
typedef unsigned short			__u16;	

typedef unsigned long			ulong;	
typedef unsigned int			__u32;

typedef long long				int64;
typedef unsigned long long		uint64;

typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef int 					INT;
typedef signed int				LONG;
typedef unsigned int			DWORD;
typedef void *					HANDLE;
typedef unsigned short			BOOL;
typedef unsigned long long		UQWORD;

typedef char *					PCHAR;
typedef char					CHAR;

typedef BYTE *					PBYTE;
typedef BYTE 					U8;
typedef WORD 					U16;
typedef DWORD 					U32;
typedef UQWORD 					U64;

typedef DWORD					COLORREF;//0x00bbggrr
typedef DWORD					COLORDEV;//0x????????

typedef const char *			CPCHAR;
typedef const unsigned int 		CDWORD;
typedef void 					VOID;

typedef unsigned long long	u64_t;
typedef signed   long long	s64_t;
typedef unsigned int		u32_t;
typedef signed   int		s32_t;
typedef unsigned short		u16_t;
typedef signed   short		s16_t;
typedef unsigned char		u8_t;
typedef signed   char		s8_t;

typedef unsigned long		u32;
typedef unsigned int		addr_t;
typedef unsigned char		u8;

typedef unsigned char           HI_U8;
typedef unsigned short          HI_U16;
typedef unsigned int            HI_U32;
typedef double                  HI_DOUBLE;
typedef signed char             HI_S8;
typedef short                   HI_S16;
typedef int                     HI_S32;
typedef void 					HI_VOID;

#define HI_SUCCESS	0

#define CONST	const

typedef struct Rect
{
	int left;
	int top;
	int right;
	int bottom;
} Rect;

typedef struct Point
{
	int x;
	int y;
} Point;
#if 0
typedef struct Size
{
	int w;
	int h;
} Size,SIZE_S;

typedef struct fyRECT_S
{
    int s32X;
    int s32Y;
    unsigned int u32Width;
    unsigned int u32Height;
}RECT_S;
#endif

typedef struct Color
{
	uchar r;
	uchar g;
	uchar b;
	uchar a;
} Color;

#define NAME_MAX_LENGTH 		32
#define LINE_MAX_LENGTH		1024


#ifndef MAX
#define MAX(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)				(((a) < (b)) ? (a) : (b))
#endif

#endif// __DH_TYPES_H__

