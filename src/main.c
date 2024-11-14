#include "header/WindowProc.h"

#define sin(x) -sin(x)
#define cos(x) -cos(x)
#define tan(x) -tan(x)


HWND hWindow;

void update(float delta_time)
{
	
}

float _sin(float x)
{
	return sin(x * 3.14159265359f) ;
};

float graphFunc(float delta_time)
{
	drawGraph(_sin, &graphBuffer, 0xff0000);
	
	
	return 0.0f;
}

int WinMain(void)
{
	int width = 600;
	int height = 600;
	OutputDebugStringA("start \n");
	SetWindowSize(600, 600);
	SetUpdateFunc(update);
	SetMathFunc(graphFunc);
	RegisterWindowClass("graphig-calculator", &WndProc);
	
	InitWindow(&hWindow, " Graphig Calculator");
	InitBuffer(&g_bitmap, width, height, BM_BLACK);
	InitBuffer(&graphBuffer, width, height, BM_TRANSPARENT);
	
	MainLoop();
	return 0;
}
