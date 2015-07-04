#ifndef __MAP_H_
#define __MAP_H_

#include "library.h"
#include "connect.h"
#include "room.h"
#include "game_server.h"

// (1 = 1%)
//#define PCT_PANGYA 100

void MapCopyStructInfo(GeralConfig *, ServerConfig *, GlobalVariables *);
void InitGameMap();
void InitPlayerMap();
void ExitGame();
void ReturnRoom();

void SetMyTurn(bool);
bool GetTurn();

void shot();

void Confirmed_Shot();             // Usado tanto pelo Master e pelo Player
void SendTurnOK();
void SendShotOK();

void MapTrade();
void FinityGame();

void SendPlayerTurn();
void RecvPacketTurn(bool __recv_turn);

DWORD ThreadShotCounterCheck(LPVOID);
void StatusShotCounterCheck(bool);

#endif
