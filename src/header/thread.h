#pragma once
#include "../header/global.h"

void startThread(thread* Thread, void* data);
void removeThread(thread* Thread);
BOOL stopThread(const thread* Thread);
BOOL resumeThread(const thread* Thread);