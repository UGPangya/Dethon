#ifndef __PRACTICE_H_
#define __PRACTICE_H_

#include "connect.h"
#include "map.h"
#include "room.h"
#include "library.h"

void PracticeCopyStructInfo(struct GeralConfig *GC, struct ServerConfig *SC, struct GlobalVariables *GV);
DWORD InitPractice(LPVOID);
void TradeMapPractice();


#endif
