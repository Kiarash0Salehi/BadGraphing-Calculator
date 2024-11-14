
#pragma once
#include <windows.h>
#include "global.h"

void SetWindowSize(int width, int height);
void GetWindowSize(int *width, int *height);
void SetUpdateFunc(void (*func)(float));
int WinWidth(), WinHeight();
int RegisterWindowClass(const char* windowClassName, LRESULT (*WndProc)(HWND, UINT, WPARAM, LPARAM));
int InitWindow(HWND* window_ptr, const char* title);
void SetMathFunc(float (*func)(float));
void MainLoop();
