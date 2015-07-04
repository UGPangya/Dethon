#ifndef __GAME_SERVER_H_
#define __GAME_SERVER_H

#include "library.h"
#include "connect.h"
#include "login_server.h"
#include "room.h"

void GameCopyStructInfo(GeralConfig *, ServerConfig *, GlobalVariables *);

void SelectChannelLogin(GeralConfig *, ServerConfig *, GlobalVariables *);

DWORD ThreadSendTimePacket(LPVOID);
DWORD ThreadSendPacketGameGuard(LPVOID);

void GameDecryptFunc(GlobalVariables *);

void SetLimitByteRecvPacket(int);

void SendPacketLoginConfirmation();

void ReloadGame();

#endif
