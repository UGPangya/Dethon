#ifndef __GAME_SERVER_H_
#define __GAME_SERVER_H

#include "library.h"
#include "connect.h"
#include "login_server.h"
#include "room.h"

void GameCopyStructInfo(struct GeralConfig *, struct ServerConfig *, struct GlobalVariables *);

void SelectChannelLogin(struct GeralConfig *, struct ServerConfig *, struct GlobalVariables *);

DWORD ThreadSendTimePacket(LPVOID);
DWORD ThreadSendPacketGameGuard(LPVOID);

void GameDecryptFunc(struct GlobalVariables *);

void SetLimitByteRecvPacket(int);

void SendPacketLoginConfirmation();

void ReloadGame();

#endif
