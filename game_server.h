#pragma once

#include <windows.h>
#include "library.h"

void GameCopyStructInfo(struct GeralConfig*, struct ServerConfig*, struct GlobalVariables*);

void SelectChannelLogin(struct GeralConfig*, struct ServerConfig*, struct GlobalVariables*);

DWORD ThreadSendTimePacket(LPVOID);
DWORD ThreadSendPacketGameGuard(LPVOID);

void GameDecryptFunc(struct GlobalVariables*);

void SetLimitByteRecvPacket(int);

void SendPacketLoginConfirmation();

void ReloadGame();
