#pragma once

#include <windows.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "../header/window.h"
#include "../header/graphic.h"
#include "../header/vector.h"
#include "../header/thread.h"
#include "../header/bitmap.h"

float _sin(float);

static float zoom = 10.0f;
vec2d center = {0.5, 0.5};
uchar mDragging = 0;
bitmap graphBuffer;
extern bitmap g_bitmap;

void drawGrids()
{
	int win_width = WinWidth();
	int win_height = WinHeight();
	int s = 12;
	Fill(0, 0,  win_width, win_height, 0xffffff);
	int lineSize = 2;
	uint32_t color = 0x9e9d9d;
	
	
	for(float x = win_width * center.x; x <= win_width; x += zoom * s)
	{
		drawLine(((int)x), 0, ((int)x), win_height, lineSize, color);
	}
	for(float y = win_height * center.y; y <= win_height; y += zoom * s)
	{
		drawLine(0, ((int)y), win_width, ((int)y), lineSize, color);
	}
	for(float x = win_width * center.x; x >= 0; x -= zoom * s)
	{
		drawLine(((int)x), 0, ((int)x), win_height, lineSize, color);
		
	}
	for(float y = win_height * center.y; y >= 0; y -= zoom * s)
	{
		drawLine(0, ((int)y), win_width, ((int)y), lineSize, color);
	}
	drawLine(0, win_height * center.y, win_width, win_height * center.y, lineSize + 1, 0x0);
	drawLine(win_width * center.x, 0, win_width * center.x, win_height, lineSize + 1, 0x0);
	CopyBufferToBuffer(&g_bitmap, &graphBuffer);
}

float drawGraph(float (*func)(float), bitmap* buffer, uint32_t color)
{
	int s = 12;
	int win_width = WinWidth();
	int win_height = WinHeight();
	vec2d currentPoint;
	vec2d lastPoint = {.x = -win_width * center.x, .y = 0};
	for(float i = -win_width * center.x; i < win_width * center.x; i +=  (1 / zoom) * 0.1f)
	{	
		currentPoint.x = i * zoom * s;
		currentPoint.y = func(i) * s * zoom;
		drawLineB(lastPoint.x + (win_width * 0.5f), lastPoint.y + (win_height * 0.5f), currentPoint.x + (win_width * 0.5f), currentPoint.y + (win_height * 0.5f), 2, color, buffer);
		lastPoint = currentPoint;
		currentPoint.x = 0;
		currentPoint.y = 0;
	}
	return 0.0f;
}

#define DrawGraph drawGraph(_sin, &graphBuffer, 0xff0000)

LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_QUIT:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		DrawGraph;
		drawGrids();
		return 0;
	}
	case WM_SIZE:
	{
		RECT size = { 0 }; 
		GetWindowRect(hWnd, &size);
		
		int width = size.right - size.left;
		int height = size.bottom - size.top;
		ResizeBuffer(width, height, &graphBuffer);
		ResizeBuffer(width, height, &g_bitmap);
		drawGrids();
		DrawGraph;
		FillB(0, 0, graphBuffer.width, graphBuffer.height, 0x0, &graphBuffer);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		clearBuffer();
		float zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta < 0.0f)
			zoom += 0.1f;
		if (zDelta > 0.0f && zoom > 0.4f)
			zoom -= 0.1f;
		drawGrids();
		FillB(0, 0, graphBuffer.width, graphBuffer.height, 0x0, &graphBuffer);
		DrawGraph;
		return 0;
	}
	default:
	{
		return DefWindowProcA(hWnd, Msg, wParam, lParam);
	}
	}
	
	return 0;
}