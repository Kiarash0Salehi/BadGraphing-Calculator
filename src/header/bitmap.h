#pragma once
#include "global.h"

void InitBuffer(bitmap* buffer, volatile int width, volatile int height, unsigned char type);
void CopyBufferToBuffer(bitmap* buffer, bitmap* src);
void ResizeBuffer(int width, int height, bitmap* buffer);
void GetBitmapInfo(bitmap* buffer, BITMAPINFO* bi);
void clearBuffer();