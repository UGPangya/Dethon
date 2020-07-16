#pragma once
#include "library.h"

void RecvChatMsg(struct GlobalVariables* GV, struct ServerConfig* SC);
void SendChatMsg(struct GlobalVariables* GV, char*);
void SendPMMsg(struct GlobalVariables* GV, char*, char*);
void GetNick(struct GlobalVariables* GV);
