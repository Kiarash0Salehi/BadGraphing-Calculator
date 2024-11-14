#pragma once
#include <stdint.h>
#include <windows.h>



const uint32_t RGB32(uint8_t r, uint8_t g, uint8_t b);
void setPixel(int x, int y, uint32_t color);
void setPixelB(int x, int y, uint32_t color, bitmap* buffer);
void Fillscreen(uint32_t color);
void Fill(int x0, int y0, int x1, int y1, uint32_t color);
void FillB(int x0, int y0, int x1, int y1, uint32_t color, bitmap* buffer);
void drawLine(int x0, int y0, int x1, int y1, int size, uint32_t color);
void drawLineB(int x0, int y0, int x1, int y1, int size, uint32_t color, bitmap* buffer);
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawTriangleB(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color, bitmap* buffer);
void createFont(HFONT* font, const char* font_name, int size);
