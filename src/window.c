#include "header/global.h"
#include "header/thread.h"
#include "header/mathbase.h"
#include <stdint.h>
#include <assert.h>
#include <math.h>

void CopyBufferToWindow(HDC hdc, bitmap* buffer, BITMAPINFO* bi);
void Fill(int x0, int y0, int x1, int y1, uint32_t color);
void drawLine(int x0, int y0, int x1, int y1, int size, uint32_t color);

HWND m_hWnd;
bitmap g_bitmap;

static int Win_Width;
static int Win_Height;

char winClassName[256] = { 0 };

#define zoom 2.0f
vec2d _center = {.x = 0.5f, .y = 0.5f};
#define s 12

void (*update_func)(float);
float (*math_func)(float);



thread updateThread, calculateThread;

void SetWindowSize(int width, int height)
{
	Win_Width = width;
	Win_Height = height;
}

void GetBitmapInfo(const bitmap* buffer, BITMAPINFO* bi)
{
	//assert(sizeof(bi));
	bi->bmiHeader.biSize = sizeof(bi->bmiHeader);
	bi->bmiHeader.biWidth = buffer->width;
	bi->bmiHeader.biHeight = -buffer->height;
	bi->bmiHeader.biBitCount = 32;
	bi->bmiHeader.biClrUsed = DIB_RGB_COLORS;
	bi->bmiHeader.biPlanes = 1;
}

void GetWindowSize(int *width, int *height)
{
	*width = Win_Width;
	*height = Win_Height;
}

void SetUpdateFunc(void (*func)(float))
{
	update_func = func;
}

int WinWidth()
{
	return g_bitmap.width;
}

int WinHeight()
{
	return g_bitmap.height;
}


void SetMathFunc(float (*func)(float))
{
	math_func = func;
}

int RegisterWindowClass(const char* windowClassName, LRESULT(*WndProc)(HWND, UINT, WPARAM, LPARAM))
{
	memcpy(&winClassName, windowClassName, strlen(windowClassName));
	WNDCLASSA wndClass;
	memset(&wndClass, 0, sizeof(WNDCLASS));
	wndClass.hbrBackground = CreateSolidBrush(0xffffff);
	wndClass.hInstance = GetModuleHandleA(0);
	wndClass.lpszClassName = windowClassName;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.hCursor = LoadCursorA(0, MAKEINTRESOURCE(32515));
	if(RegisterClassA(&wndClass) == 0) return 0;
	return 1;
}

int InitWindow(HWND* window_ptr, const char* title)
{
	RECT rect = {0, 0, Win_Width, Win_Height};
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	int width, height;
	int xPos, yPos;
	HINSTANCE hInstance = GetModuleHandleA(0);

	size_t alloc_size = Win_Width * Win_Height * sizeof(uint32_t);
	g_bitmap.memory = malloc(alloc_size);

	AdjustWindowRect(&rect, windowStyle, 0);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	xPos = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
	yPos = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);

	m_hWnd = CreateWindowExA
	(
		0,
		winClassName,
		title,
		windowStyle,
		xPos,
		yPos,
		width,
		height,
		0,
		0,
		hInstance,
		0
	);

	ShowWindow(m_hWnd, 1);

	memcpy(window_ptr, &m_hWnd, sizeof(HWND));
}

void __cdecl updateThreadFunc(void* data)
{
	LARGE_INTEGER cpu_frequency;
	QueryPerformanceFrequency(&cpu_frequency);

	LARGE_INTEGER last_counter;
	QueryPerformanceCounter(&last_counter);
	
	BITMAPINFO bi = {0};
	
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = g_bitmap.width;
	bi.bmiHeader.biHeight = -g_bitmap.height;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biClrUsed = DIB_RGB_COLORS;
	bi.bmiHeader.biPlanes = 1;
	
	while (1)
	{
		LARGE_INTEGER current_counter;
		QueryPerformanceCounter(&current_counter);

		int64_t counter_elapsed = current_counter.QuadPart - last_counter.QuadPart;

		float delta_time = (float)counter_elapsed / (float)cpu_frequency.QuadPart;

		last_counter = current_counter;

		bi.bmiHeader.biWidth = g_bitmap.width;
		bi.bmiHeader.biHeight = -g_bitmap.height;
		update_func(delta_time);
		HDC hdc = GetDC(m_hWnd);
		CopyBufferToWindow(hdc, &g_bitmap, &bi);
		ReleaseDC(m_hWnd, hdc);
	}
}

void __cdecl calculateThreadFunc(void* data)
{
	LARGE_INTEGER cpu_frequency;
	QueryPerformanceFrequency(&cpu_frequency);

	LARGE_INTEGER last_counter;
	QueryPerformanceCounter(&last_counter);
	while(1)
	{
		LARGE_INTEGER current_counter;
		QueryPerformanceCounter(&current_counter);

		int64_t counter_elapsed = current_counter.QuadPart - last_counter.QuadPart;

		float delta_time = (float)counter_elapsed / (float)cpu_frequency.QuadPart;

		last_counter = current_counter;
		//math_func(delta_time);
	}
}

void MainLoop()
{
	MSG message;
	memset(&message, 0, sizeof(MSG));
	uint8_t running = 1;
	
	updateThread.func = updateThreadFunc;
	startThread(&updateThread, 0);
	
	calculateThread.func = calculateThreadFunc;
	startThread(&calculateThread, 0);
	
	while(running)
	{
	if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) { running = 0; }
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		resumeThread(&updateThread);
		resumeThread(&calculateThread);
	}
}

void ResizeBuffer(int width, int height, bitmap* buffer)
{
	stopThread(&updateThread);
	stopThread(&calculateThread);
	buffer->width = width;
	buffer->height = height;

	Win_Width = width;
	Win_Height = height;
	
	free(buffer->memory);
	buffer->memory = malloc((width + 1) * (height + 1) * sizeof(uint32_t));
}

void clearBuffer()
{
	Fill(0, 0, g_bitmap.width, g_bitmap.height, 0x0);
}

void InitBuffer(bitmap* buffer, volatile int width, volatile int height, u8 type)
{
if((int)width == 0) OutputDebugStringA("value lossed");
	OutputDebugStringA("init Buffer/n");
	buffer->memory = malloc(width * height * sizeof(uint32_t));
	buffer->size = width * height * sizeof(uint32_t);
	buffer->width = width;
	buffer->height = height;
	buffer->type = type;
	
}
	

	