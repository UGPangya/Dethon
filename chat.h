#ifndef __CHAT_H_
#define __CHAT_H_

#include "library.h"
#include "connect.h"
#include "map.h"

void RecvChatMsg(struct GlobalVariables *GV, struct ServerConfig *SC);
void SendChatMsg(struct GlobalVariables *GV, char*);
void SendPMMsg(struct GlobalVariables *GV, char*, char*);
void GetNick(struct GlobalVariables *GV);

#endif
