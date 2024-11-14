#pragma once
#include <windows.h>

#define BM_TRANSPARENT 	0x1
#define BM_BLACK		0x2
#define BM_WHITE		0x3

struct _bitmap
{
	void* memory;
	int width;
	int height;
	size_t size;
	unsigned char type;
	#ifdef WIN32
	BITMAPINFO bi;
	#endif
};

struct _vec2d
{
	float x;
	float y;
};

struct _thread
{
	HANDLE thread_id;
	void  (__cdecl *func)(void*);
};

typedef struct _fpoint
{
	float x;
	float y;
} fpoint;

typedef struct vector {

    void **data;
    int capacity;
    int total;
} vector;

typedef struct _bitmap 	bitmap; 	
typedef struct _vec2d 	vec2d ; 	
typedef struct _thread 	thread;
typedef unsigned char uchar, u8; 