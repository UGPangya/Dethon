#ifndef __CHAT_H_
#define __CHAT_H_

#include "library.h"
#include "connect.h"
#include "map.h"

void RecvChatMsg(GlobalVariables *GV, ServerConfig *SC);
void SendChatMsg(GlobalVariables *GV, char*);
void SendPMMsg(GlobalVariables *GV, char*, char*);
void GetNick(GlobalVariables *GV);

#endif
