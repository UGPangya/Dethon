#pragma once
#include "library.h"
#include <windows.h>

void MakePacketLoginServer(struct GeralConfig*, struct GlobalVariables*);
void SendMakePacketLogin(struct GeralConfig*, struct GlobalVariables*);
void SendPacketChangeLogin(struct GeralConfig*, struct ServerConfig*, struct GlobalVariables*);
BOOLEAN SendServerLogin(struct GeralConfig*, struct ServerConfig*, struct GlobalVariables*);
void DesconectLogin(struct GlobalVariables*);
