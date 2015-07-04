#ifndef __COLOR_H
#define __COLOR_H

#ifdef _WIN32 
#include <windows.h>
#elif LINUX
#endif

void setcolor(int color);
void restcolor();

#endif
