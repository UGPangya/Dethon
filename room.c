#include "room.h"

#include <stdio.h>

#include "chat.h"
#include "encrypt.h"
#include "map.h"

/*
[03] N�mero de players
[07] Size of do nome da sala 
[12] N�meros de hole 0x12 = 18
[40] Time 40 minutos
[00] ID Mapa
81 20 00 79 00 08 00 00 [40] 9C 00 00 00 00 00 00 
[03] 
00 
[12] 
[00] 00 
[07] 
00
50 61 6E 67 79 61 21 (Name) Pangya!
00 00 00 00 00 00
*/

unsigned char packet_create_room_part1[] =
	/*"\x9E\x2C\x00\x79\xB9\x08\x00\x00\x40\x9C\x00\x00\x00"
	"\x00\x00\x00\x03\x00\x12\x00\x00\x07\x00";*/

	"\x0d\x2f\x00\xff\xf1\x08\x00\x00\x90\x9C\x00\x00\x00"
	"\x00\x00\x00\x02\x00\x03\x0B\x00\x00\x00\x00\x00\x0f\x00";

// Byte 0 = sizeof password (se n�o tiver password fica em zero e junta com a parte 3)
unsigned char packet_create_room_part2[] = "\x03\x00";
unsigned char packet_create_room_part3[] =
	"\x00\x00\x00\x00\x73\x2E\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

// Byte 3 (ID_ROOM)
// Byte 5 (SIZE PASSWORD - 0x00 NO_PASSWORD)
unsigned char packet_enter_room_part1[] = "\xCE\x0A\x00\xB4\x05\x09\x00\x1A\x00\x06\x00";

// 01 = Desable
// 00 = Enable
unsigned char ready_packet[] = "\x3D\x04\x00\x3F\x00\x0D\x00\x00";

#define TIME_WAIT_MASTER 8

static bool pause_status = 0x00;
static int id_room_confirmed = 0x00;
static int time_wait_master = 0x00;

HWND CThreadCheckShot = NULL;
DWORD* PThreadR;

unsigned char _buffer_tmp[1024];
unsigned int tmp;

/*
0x40 - 40 minutos (1,6*40 = 64 = 0x40)
0x60 - 60 minutos (1,6*60 = 96 = 0x60)
0x90 - 90 minutos (1,6*90 = 144 = 0x90)
0xC0 - 120 minutos (1,6*120 = 192 = 0xC0)
*/
#define TIME_TRASF_HEX 1.6f
#define OFFSET_60_MINUTOS 0xEA


unsigned int ROOM_ID;
unsigned char PASSWORD[32];

struct GeralConfig* RpGC;
struct ServerConfig* RpSC;
struct GlobalVariables* RpGV;

static int quantity_player_confirmed = 0;

void RoomCopyStructInfo(struct GeralConfig* GC, struct ServerConfig* SC, struct GlobalVariables* GV)
{
	RpGC = GC;
	RpSC = SC;
	RpGV = GV;
}

void ResetRoom()
{
	// Reset System
	quantity_player_confirmed = 0;
	for (int i = 0; i < 3; i++)
	{
		RpSC->COMMON_CONFIRMED[i] = 0x00;
	}
}

// DB 2A 00 18 C8 08 00 00 40 9C 00 00 00 00 00 00 02 00 03 0D 00 0D 00 43 68 61 6C 6C 65 6E 67 65 20 4D 65 21 04 00 31 32 33 35 00 00 00 00
void create_room(char* room_name, int player_num, int hole_num, int type_map, unsigned int time_min, char* password)
{
	float time_hex = time_min * TIME_TRASF_HEX;

	PacketSoma(packet_create_room_part1, sizeof(packet_create_room_part1) - 1, 0);

	tmp = time_min * OFFSET_60_MINUTOS / 60;

	packet_create_room_part1[8] = (unsigned char)time_hex;

	packet_create_room_part1[9] = (unsigned short)tmp & 0x0FF;
	packet_create_room_part1[10] = (unsigned short)tmp >> 8 & 0x0F;

	packet_create_room_part1[16] = player_num;
	packet_create_room_part1[18] = hole_num;
	packet_create_room_part1[19] = type_map;
	packet_create_room_part1[25] = (unsigned char)strlen(room_name);
	packet_create_room_part1[1] = (unsigned char)(sizeof(packet_create_room_part3) + sizeof(packet_create_room_part2) +
		strlen(room_name) + strlen(password) + sizeof(packet_create_room_part1) - 3 - 4);

	CopyStringPos(_buffer_tmp, packet_create_room_part1, sizeof(packet_create_room_part1) - 1, 0, 0);
	CopyStringPos(_buffer_tmp, (unsigned char*)room_name, strlen(room_name), sizeof(packet_create_room_part1) - 1, 0);

	packet_create_room_part2[0] = (unsigned char)strlen(password);

	CopyStringPos(_buffer_tmp, packet_create_room_part2, sizeof(packet_create_room_part2) - 1,
	              strlen(room_name) + sizeof(packet_create_room_part1) - 1, 0);

	if (password[0] == 0x00)
	{
		CopyStringPos(_buffer_tmp, packet_create_room_part3, sizeof(packet_create_room_part3) - 1,
		              sizeof(packet_create_room_part2) + strlen(room_name) + sizeof(packet_create_room_part1) - 2, 0);
	}
	else
	{
		CopyStringPos(_buffer_tmp, (unsigned char*)password, strlen(password),
		              sizeof(packet_create_room_part2) + strlen(room_name) + sizeof(packet_create_room_part1) - 2, 0);
		CopyStringPos(_buffer_tmp, packet_create_room_part3, sizeof(packet_create_room_part3) - 1,
		              strlen(password) + sizeof(packet_create_room_part2) + strlen(room_name) + sizeof(
			              packet_create_room_part1) - 2, 0);
	}


	EncryptSendPacket((char*)_buffer_tmp,
	                  strlen(password) + sizeof(packet_create_room_part3) + sizeof(packet_create_room_part2) +
	                  strlen(room_name) + sizeof(packet_create_room_part1) - 3, 0);

	/*DEBUG_MODE_TYPE(1);
	ShowPacketInHex(buffer_tmp, strlen(password)+sizeof(packet_create_room_part2)+strlen(room_name)+sizeof(packet_create_room_part1)-2);
	DEBUG_MODE_TYPE(0);*/
}

void enter_room(unsigned int ID_ROOM, char* password)
{
	PacketSoma(packet_enter_room_part1, sizeof(packet_enter_room_part1) - 1, 0);

	packet_enter_room_part1[7] = ID_ROOM;
	packet_enter_room_part1[9] = (unsigned char)strlen(password);
	packet_enter_room_part1[1] = (unsigned char)(strlen(password) + sizeof(packet_enter_room_part1) - 1 - 4);

	CopyStringPos(_buffer_tmp, packet_enter_room_part1, sizeof(packet_enter_room_part1) - 1, 0, 0);

	if (password[0] != 0x00)
	{
		CopyStringPos(_buffer_tmp, (unsigned char*)password, strlen(password), sizeof(packet_enter_room_part1) - 1, 0);
	}

	EncryptSendPacket((char*)_buffer_tmp, strlen(password) + sizeof(packet_enter_room_part1) - 1, 0);

	/*DEBUG_MODE_TYPE(1);
	ShowPacketInHex(buffer_tmp, strlen(password)+sizeof(packet_enter_room_part1)-1);
	DEBUG_MODE_TYPE(0);*/
}

void ReadyRoom(bool type)
{
	PacketSoma(ready_packet, sizeof(ready_packet) - 1, 0);
	ready_packet[7] = type;
	EncryptSendPacket((char*)ready_packet, sizeof(ready_packet) - 1, 0);
}

void SetPassword(const char* r_password)
{
	strcpy_s((char *)PASSWORD, _countof(PASSWORD), r_password);
}

void SetRoomID(unsigned int id_room)
{
	ROOM_ID = id_room;
}

void SendRoomIDPlayer_and_confirmed(char* player)
{
	sprintf_s((char*)_buffer_tmp, _countof(_buffer_tmp), "ROOM_SEND ID [%d] PASSWORD [%s", ROOM_ID, PASSWORD);
	SendPMMsg(RpGV, (char*)_buffer_tmp, player);
}

DWORD MasterPlayerGame(LPVOID Param)
{
	for (int i = 0; i < 3; i++)
	{
		if (i >= RpSC->QUANTITY_PLAYER_WAIT)
			break;
		sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "ENVIANDO RESET PARA O PLAYER [%s]\n", RpSC->PLAYER_COMMON[i]);
		InfoServer(RpGV->CInfo);
		SendPMMsg(RpGV, "reset_system_now", RpSC->PLAYER_COMMON[i]);
	}

	Sleep(1000);

	strcpy_s((char*)PASSWORD, _countof(PASSWORD), RpSC->ROOM_PASSWORD);

	InfoServer("CRIANDO SALA\n");

	sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "NAME [%s] PASSWORD [%s]\n", RpSC->ROOM_NAME, RpSC->ROOM_PASSWORD);
	InfoServer(RpGV->CInfo);

	create_room(RpSC->ROOM_NAME, RpSC->NUMBER_PLAYER, RpSC->NUMBER_HOLE, RpSC->TYPE_MAP, RpSC->TIME_MIN,
	            RpSC->ROOM_PASSWORD);

	Sleep(2000);

	sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "SALA_ID -> [%d]\n", ROOM_ID);
	InfoServer(RpGV->CInfo);

	while (GetStatusRecvPacket() == TRUE)
	{
		sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "AGUARDANDO PLAYERS [%d/%d]\n", quantity_player_confirmed,
		          RpSC->QUANTITY_PLAYER_WAIT);
		InfoServer(RpGV->CInfo);

		RpSC->TYPE_SCRIPT = 0x04; // Status OK, MasterGame

		while (GetStatusRecvPacket() == TRUE)
		{
			for (int i = 0; i < 3; i++)
			{
				if (RpSC->COMMON_CONFIRMED[i] == 0x01)
				{
					sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "SEND ID_ROOM E PASSWORD PARA [%s]\n",
					          RpSC->PLAYER_COMMON[i]);
					InfoServer(RpGV->CInfo);
					SendRoomIDPlayer_and_confirmed(RpSC->PLAYER_COMMON[i]);

					/*quantity_player_confirmed++;
					RpSC->COMMON_CONFIRMED[i] = 0x03;  // EST�GIO FINAL
					*/

					RpSC->COMMON_CONFIRMED[i] = 0x00;

					/*
					sprintf(RpGV->CInfo, "AGUARDANDO PLAYERS [%d/%d]\n", quantity_player_confirmed, RpSC->QUANTITY_PLAYER_WAIT);
					InfoServer(RpGV->CInfo);
					*/
				}
			}
			if (quantity_player_confirmed >= RpSC->QUANTITY_PLAYER_WAIT)
				break;
			Sleep(10);
		}

		/*InfoServer("PLAYER MASTER SUCESSFULL\n");
	   
		for(int i = 0; i < quantity_player_confirmed; i++)
		{
		 if(RpSC->COMMON_CONFIRMED[i] == 0x03)
		 {
		  sprintf(RpGV->CInfo, "SEND ID_ROOM E PASSWORD PARA [%s]\n", RpSC->PLAYER_COMMON[i]);
		  InfoServer(RpGV->CInfo);
		  SendRoomIDPlayer_and_confirmed(RpSC->PLAYER_COMMON[i]);
		 }
		}
		Sleep(5000);*/

		Sleep(500);

		if (GetStatusRecvPacket() == TRUE)
		{
			/*SendChatMsg(RpGV, "Bem Vindo!");
			Sleep(1000);
			SetMyTurn(1);*/
			InitGameMap();

			quantity_player_confirmed = 0;
			for (int i = 0; i < 3; i++)
			{
				RpSC->COMMON_CONFIRMED[i] = 0x00;
			}
			RpSC->TYPE_SCRIPT = 0x01; // Status OK
		}
	}
	return 0;
}

DWORD CommomPlayerGame(LPVOID Param)
{
	TerminateThread(CThreadCheckShot, 0);
	CloseHandle(PThreadR);
	CThreadCheckShot = (HWND)CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadShotCounterCheck, (LPVOID)0, 0, PThreadR);

	while (GetStatusRecvPacket() == TRUE && (RpSC->TYPE_SCRIPT != 0x07))
	{
		while (GetStatusRecvPacket() == TRUE && (RpSC->TYPE_SCRIPT != 0x07))
		{
			if (RpSC->P_ROOM_ID != 0x00)
				break;
			if (!pause_status)
			{
				sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "ENVIANDO REQUISITO PARA O MASTER [%s]\n",
				          RpSC->PLAYER_MASTER);
				InfoServer(RpGV->CInfo);

				SendPMMsg(RpGV, "sendme_info_room", RpSC->PLAYER_MASTER);
			}
			Sleep(2000);
		}

		if (GetStatusRecvPacket() == TRUE)
		{
			if (id_room_confirmed != RpSC->P_ROOM_ID)
			{
				sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "REQUISITO RECEBIDO SALA [%d] PASSWORD [%s]\n",
				          RpSC->P_ROOM_ID, RpSC->P_PASSWORD_ROOM);
				InfoServer(RpGV->CInfo);

				enter_room(RpSC->P_ROOM_ID, RpSC->P_PASSWORD_ROOM);
				id_room_confirmed = RpSC->P_ROOM_ID;
			}

			Sleep(100);

			InfoServer("READY GAME\n");
			ReadyRoom(0);
			RpSC->TYPE_SCRIPT = 0x05; // Player Game
			InfoServer("AGUARDANDO MASTER\n");
			time_wait_master = TIME_WAIT_MASTER;
			while (RpSC->TYPE_SCRIPT == 0x05)
				// Enquanto n�o receber o packet de confirma��o de inicio do jogo, continua no while
			{
				if (time_wait_master <= 0x00)
				{
					ReadyRoom(1);
					Sleep(100);
					ReadyRoom(0);
					time_wait_master = TIME_WAIT_MASTER;
				}
				Sleep(1000);
				time_wait_master--;
			}
			StatusShotCounterCheck(1);
			if (RpSC->TYPE_SCRIPT == 0x06)
			{
				SetPlayersConfirmed(RpSC->QUANTITY_PLAYER_WAIT);
				SetMyTurn(0);
				// Nesse momento o RpSC->TYPE_SCRIPT � jogado pra 0x06
				InitGameMap();
				StatusShotCounterCheck(0);
				RpSC->TYPE_SCRIPT = 0x00;
				/*RpSC->P_ROOM_ID = 0x00;*/
			}
		}
	}
	RpSC->TYPE_SCRIPT = 0x00;
	RpSC->P_ROOM_ID = 0x00;
	id_room_confirmed = -1;
	StatusShotCounterCheck(0);
	return 0;
}

unsigned int GetPlayersConfirmed()
{
	return quantity_player_confirmed;
}

void SetPlayersConfirmed(unsigned int players)
{
	quantity_player_confirmed = players;
}

void DecrementPlayersConfirmed()
{
	if ((RpSC->TYPE_SCRIPT == 0x04) || (RpSC->TYPE_SCRIPT == 0x01))
	{
		quantity_player_confirmed--;
		sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "AGUARDANDO PLAYERS [%d/%d]\n", quantity_player_confirmed,
		          RpSC->QUANTITY_PLAYER_WAIT);
		InfoServer(RpGV->CInfo);
	}
}

void DecrementPlayersMap()
{
	quantity_player_confirmed--;
	sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "PLAYERS [%d/%d]\n", quantity_player_confirmed,
	          RpSC->QUANTITY_PLAYER_WAIT);
	InfoServer(RpGV->CInfo);
	//if((quantity_player_confirmed <= 0x00))
	//ReturnRoom();
}

void IncrementPlayersConfirmed()
{
	if ((RpSC->TYPE_SCRIPT == 0x04) || (RpSC->TYPE_SCRIPT == 0x01))
	{
		quantity_player_confirmed++;
		sprintf_s(RpGV->CInfo, _countof(RpGV->CInfo), "AGUARDANDO PLAYERS [%d/%d]\n", quantity_player_confirmed,
		          RpSC->QUANTITY_PLAYER_WAIT);
		InfoServer(RpGV->CInfo);
	}
}

void StatusPause(bool StatusPause)
{
	pause_status = StatusPause;
}

void IDRoomConfirmed(int _id_room_confirmed)
{
	id_room_confirmed = _id_room_confirmed;
}

void TermineteThreadCheckShot()
{
	TerminateThread(CThreadCheckShot, 0);
}
