#include "header/global.h"
void (*graph_func)(float);

void setMathFunc(void (*func)(float))
{
	graph_func = func;
}

//thread* 