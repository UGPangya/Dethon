#pragma once
#include <stdbool.h>
#include <windows.h>
#include "library.h"

void MapCopyStructInfo(struct GeralConfig*, struct ServerConfig*, struct GlobalVariables*);
void InitGameMap();
void InitPlayerMap();
void ExitGame();
void ReturnRoom();

void SetMyTurn(bool);
bool GetTurn();

void shot();

void Confirmed_Shot(); // Usado tanto pelo Master e pelo Player
void SendTurnOK();
void SendShotOK();

void MapTrade();
void FinityGame();

void SendPlayerTurn();
void RecvPacketTurn(bool __recv_turn);

DWORD ThreadShotCounterCheck(LPVOID);
void StatusShotCounterCheck(bool);
