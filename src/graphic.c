#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "header/global.h"

extern bitmap g_bitmap;

void setPixel(int x, int y, uint32_t color)
{
	if (x < 0 || y < 0 || x >= g_bitmap.width || y >= g_bitmap.height) return;
	uint32_t* pixel = (uint32_t*)g_bitmap.memory + y * g_bitmap.width + x;
	*pixel = color;
}

void setPixelB(int x, int y, uint32_t color, bitmap* buffer)
{
	if (x < 0 || y < 0 || x >= buffer->width || y >= buffer->height) return;
	uint32_t* pixel = (uint32_t*)buffer->memory + y * buffer->width + x;
	*pixel = color;
}

void CopyBufferToWindow(HDC hdc, bitmap* buffer, BITMAPINFO* bi)
{
	StretchDIBits(hdc, 0, 0, buffer->width, buffer->height, 0, 0, buffer->width, buffer->height, g_bitmap.memory, bi, DIB_RGB_COLORS, SRCCOPY);
}


void Fill(int x0, int y0, int x1, int y1, uint32_t color)
{
	int minX = x0;
	int minY = y0;
	int maxX = x1;
	int maxY = y1;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			setPixel(x, y, color);
		}
	}
}

void FillB(int x0, int y0, int x1, int y1, uint32_t color, bitmap* buffer)
{
	int minX = x0;
	int minY = y0;
	int maxX = x1;
	int maxY = y1;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			setPixelB(x, y, color, buffer);
		}
	}
}


void drawLine(int x0, int y0, int x1, int y1, int size, uint32_t color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int steps;
	if (abs(dx) > abs(dy)) steps = abs(dx); else steps = abs(dy);

	float xinc = dx / (float)steps;
	float yinc = dy / (float)steps;

	float x = (float)x0, y = (float)y0;

	for (int i = 0; i <= steps; i++)
	{
		float halfsize = (float)size / 2;
		for (float j = y - halfsize; j < y + halfsize; j++)
		{
			for (float w = x - halfsize; w < x + halfsize; w++)
			{
				setPixel(round(w), round(j), color);
			}
		}
		x += xinc;
		y += yinc;
	}
}

void drawLineB(int x0, int y0, int x1, int y1, int size, uint32_t color, bitmap* buffer)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int steps;
	if (abs(dx) > abs(dy)) steps = abs(dx); else steps = abs(dy);

	float xinc = dx / (float)steps;
	float yinc = dy / (float)steps;

	float x = (float)x0, y = (float)y0;

	for (int i = 0; i <= steps; i++)
	{
		float halfsize = (float)size / 2;
		for (float j = y - halfsize; j < y + halfsize; j++)
		{
			for (float w = x - halfsize; w < x + halfsize; w++)
			{
				setPixelB(round(w), round(j), color, buffer);
			}
		}
		x += xinc;
		y += yinc;
	}
}

void createFont(HFONT* font, const char* font_name, int size)
{
	*font = CreateFont(size,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,font_name);
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	drawLine(x0, y0, x1, y1, 1, color);
	drawLine(x1, y1, x2, y2, 1, color);
	drawLine(x2, y2, x0, y0, 1, color);
}

void DrawTriangleB(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color, bitmap* buffer)
{
	drawLineB(x0, y0, x1, y1, 1, color, buffer);
	drawLineB(x1, y1, x2, y2, 1, color, buffer);
	drawLineB(x2, y2, x0, y0, 1, color, buffer);
}

void CopyBufferToBuffer(bitmap* buffer, bitmap* src)
{
	int32_t bitmap_offset = 0;
	int32_t color = 0;
	int32_t start_bitmap = src->width * src->height - src->width;

	for (int pixel_x = 0; pixel_x < src->width; pixel_x++)
	{		
		int wich_dist = pixel_x;
		if (wich_dist > src->width) wich_dist = src->width;

		if (pixel_x > 0)
		{
			for (int pixel_y = 0; pixel_y < src->height; pixel_y++)
			{

				bitmap_offset = start_bitmap + pixel_x - pixel_y * src->width;

				void* c = memcpy(&color, (uint32_t*)src->memory + bitmap_offset, sizeof(uint32_t));

				if (color != 0x0)	// this color would be cyan, used for transparency
				{
					setPixelB(pixel_x, pixel_y, color, buffer);
				}
			}
		}

	}
}

const uint32_t RGB32(uint8_t r, uint8_t g, uint8_t b) { return  (uint32_t)(r << 16) | (g << 8) | (b << 0); }