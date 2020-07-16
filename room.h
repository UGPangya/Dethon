#ifndef __ROOM_H_
#define __ROOM_H_

#include "library.h"
#include "connect.h"
#include "chat.h"
#include "map.h"

// MAPAS IDS
#define BLUE_LAGOON     0x00
#define BLUE_WATER      0x01
#define SEPIA_WIND      0x02
#define WIND_HILL       0x03
#define WIZWIZ          0x04
#define WESTWIZ         0x05
#define BLUE_MOON       0x06
#define SILVIA_CANNON   0x07
#define ICE_CANNON      0x08
#define WHITEWIZ        0x09
#define SHINING_SAND    0x0A
#define WIND            0x0B
#define UNKNOWN         0x0C
#define DEEP_INFERNO    0x0D
#define ICE_SPA         0x0E
#define LOST_SEAWAY     0x0F
#define EASTERB_VALLEY  0x10
#define ICE_INFERNO     0x12
#define WIZCITY         0x13
#define ABBOT_MINE      0x14
        
#define RANDOM 0x7f

void RoomCopyStructInfo(struct GeralConfig *, struct ServerConfig *, struct GlobalVariables *);
void ResetRoom();
void create_room(char *room_name, int player_num, int hole_num, int type_map, unsigned int time_min, char *password);
void enter_room(unsigned int ID_ROOM, char *password);
void ReadyRoom(bool type);

void SetPassword(const char*);
void SetRoomID(unsigned int);

void SendRoomIDPlayer_and_confirmed(char *);

unsigned int GetPlayersConfirmed();
void SetPlayersConfirmed(unsigned int);
void DecrementPlayersConfirmed();
void DecrementPlayersMap();
void IncrementPlayersConfirmed();

void StatusPause(bool);
void IDRoomConfirmed(int);
void TermineteThreadCheckShot();

#endif
