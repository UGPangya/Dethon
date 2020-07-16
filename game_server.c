#include "game_server.h"

#include <stdio.h>
#include "connect.h"
#include "decrypt.h"
#include "encrypt.h"
#include "room.h"

// **********************************************************
// **********************************************************
// ********************* Packet Ref *************************
// **********************************************************
// **********************************************************

unsigned char server_login_channel_change[] = "\x8E\x04\x00\x1A\xB9\x04\x00\x00";

// \xE3\x03\x00\x3C\xB3\x6C\x01
// \xE3\x03\x00\x3C\xB3\x6D\x01
unsigned char login_access_ok[] = "\xE3\x03\x00\x3C\xB3\x6C\x01";

unsigned char unknown01[] = "\x00\x03\x00\x0B\x00\x8B\x00";

unsigned char sendtimepacket[] = "\x8B\x07\x00\x32\x28\xF4\x00\x00\x08\xF4\x00";

unsigned char start_game[] = "\x48\x03\x00\xA3\x90\x81\x00";

unsigned char packet_reload[] = "\x41\x03\x00\x69\xF0\x43\x00";

unsigned char packet_gameguard[] =
	"\x90\x53\x00\x22\xA3\x88\x00\x00\x00\x00\x00\xD1\x4E\x9A\x01\x2A\x2E\x9A\x01\x01\x00\x00\x00\xD1\x4E\x9A\x01\xD1\x4E\x9A\x01\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x9F\x77\x7A\xDA\x27\x12\x4F\x2E\xAD\x56\xA3\xE6\x09\x34\xBD\x2B\x6A\xB1\xF3\x75\x03\x4D\x94\xBD\x43\x40\x9D\xDC\x18\x5A\xF7\x0C\x6A\xB1\xF3\x75\x03\x4D\x94\xBD\x43\x40\x9D\xDC";

// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************

static int PACKET_LIMITED = 0;
/*static int TimeReload = 0;*/

struct GeralConfig* GpGC;
struct ServerConfig* GpSC;
struct GlobalVariables* GpGV;

void GameCopyStructInfo(struct GeralConfig* GC, struct ServerConfig* SC, struct GlobalVariables* GV)
{
	GpGC = GC;
	GpSC = SC;
	GpGV = GV;
}

void SelectChannelLogin(struct GeralConfig* GC, struct ServerConfig* SC, struct GlobalVariables* GV)
{
	if (!GC->AutoLogin)
	{
		for (;;)
		{
			for (int i = 0; i < MAX_SERVER_LOGIN_CONFIG - 1; i++)
			{
				ColorText("SUB_SERVER ", 2);
				printf("NAME [%s] - OFFSET [%d]\n", SC->SUBSERVERNAME[SC->server_game_change][i],
				       SC->OFFSETSUBSERVER[SC->server_game_change][i]);
			}

			ColorText("SUB_SERVER -> ", 4);
			scanf_s("%d", &SC->server_login_channel_change);
			if (SC->server_login_channel_change < MAX_SERVER_LOGIN_CONFIG)
				break;
			FailedServer("sub servidor inexistente\n");
		}
	}
	else
	{
		if (SC->SELECT_SUBSERVER < MAX_SERVER_LOGIN_CONFIG)
		{
			SC->server_login_channel_change = SC->SELECT_SUBSERVER;
			sprintf_s(GV->CInfo, _countof(GV->CInfo), "AUTO SELECT SUBSERVER ID [%d] SUBSERVER_NAME [%s]\n",
			          SC->SELECT_SUBSERVER, SC->SUBSERVERNAME[SC->server_game_change][SC->server_login_channel_change]);
			InfoServer(GV->CInfo);
		}
		else
		{
			FailedServer("sub servidor inexistente - set 0\n");
			SC->server_login_channel_change = 0;
		}
	}

	server_login_channel_change[7] = SC->OFFSETSUBSERVER[SC->server_game_change][SC->server_login_channel_change];

	// SEND PACKET
	InfoServer("Send Packet - UNKNOWN\n");
	PacketSoma(unknown01, sizeof(unknown01) - 1, 0);
	SendPacket((char*)unknown01, sizeof(unknown01) - 1);

	Sleep(200);

	// SEND PACKET
	InfoServer("Send Packet - LOGIN SERVER\n");
	PacketSoma(server_login_channel_change, sizeof(server_login_channel_change) - 1, 0);
	SendPacket((char*)server_login_channel_change, sizeof(server_login_channel_change) - 1);

	Sleep(200);

	login_access_ok[5] = SC->GAME_ACCESS_OFFSET;
	// SEND PACKET
	InfoServer("Send Packet - ACCESS GAME\n");
	PacketSoma(login_access_ok, sizeof(login_access_ok) - 1, 0);
	SendPacket((char*)login_access_ok, sizeof(login_access_ok) - 1);

	Sleep(200);

	// SEND PACKET
	PacketSoma(start_game, sizeof(start_game) - 1, 0);
	InfoServer("Send Packet - START GAME\n");
	SendPacket((char*)start_game, sizeof(start_game) - 1);
}

DWORD ThreadSendTimePacket(LPVOID param)
{
	UNREFERENCED_PARAMETER(param);
	while (GetStatusSendTimePacket())
	{
		Sleep(60000);
		SendPacketLoginConfirmation();
	}
	return 0;
}

/*DWORD ThreadSendPacketGameGuard(LPVOID)
{
 while(true)
 {
  Sleep(300000);
  PacketSoma(packet_gameguard, sizeof(packet_gameguard)-1, 0);
  EncryptSendPacket((char*)packet_gameguard, sizeof(packet_gameguard)-1, 0);
  InfoServer("SEND PACKET GAMEGUARD\n");
 }
}*/

char msg_debug[127];

void GameDecryptFunc(struct GlobalVariables* GV)
{
	unsigned int tmp_other = 0x01;
	unsigned int retn_tmp;
	unsigned int tmp_ebp = 0;

	retn_tmp = RecvPacket((char*)GV->PacketRecv, sizeof(GV->PacketRecv) - 1);
	GV->bytes_recv = retn_tmp;

	sprintf_s(msg_debug, _countof(msg_debug), "SIZE [%d] Recv Packet", retn_tmp);
	AddStringToListBoxDebug(msg_debug);

	if (retn_tmp == 0 || retn_tmp == -1)
		SetStatusRecvPacket(FALSE);

	if (retn_tmp != 0 && retn_tmp != -1)
	{
		for (;;)
		{
			if ((int)retn_tmp >= PACKET_LIMITED && PACKET_LIMITED != -1)
				break;

			tmp_ebp = ((unsigned char)GV->PacketRecv[tmp_other + 1] * 0x100) + (unsigned char)GV->PacketRecv[tmp_other]
				+ 3;

			GV->bytes_recv_piece = tmp_ebp;

			ENCDecryptPacket(GV->packet_decrypt, (unsigned char*)GV->PacketRecv + tmp_other - 1, tmp_ebp);

			if (PacketAnalyser(tmp_ebp))
				SetStatusRecvPacket(FALSE);

			if (tmp_other + ((unsigned char)GV->PacketRecv[tmp_other + 1] * 0x100) + (unsigned char)GV->PacketRecv[
				tmp_other] + 3 <= retn_tmp - 1)
			{
				if (RECV_DEBUG_MODE_TYPE() == 1)
					printf("Info Next -> %x Addr %x\n",
					       ((unsigned char)GV->PacketRecv[tmp_other + 1] * 0x100) + (unsigned char)GV->PacketRecv[
						       tmp_other] + 3,
					       tmp_other + ((unsigned char)GV->PacketRecv[tmp_other + 1] * 0x100) + (unsigned char)GV->
					       PacketRecv[tmp_other] + 3);
				tmp_other += ((unsigned char)GV->PacketRecv[tmp_other + 1] * 0x100) + (unsigned char)GV->PacketRecv[
					tmp_other] + 3;
			}
			else
				break;
		}
	}
	else
	{
		if (PacketAnalyser(tmp_ebp))
			SetStatusRecvPacket(FALSE);
	}
}

void SetLimitByteRecvPacket(int LimitByteRecv)
{
	PACKET_LIMITED = LimitByteRecv;
}

void SendPacketLoginConfirmation()
{
	PacketSoma(sendtimepacket, sizeof(sendtimepacket) - 1, 0);
	SendPacket((char*)sendtimepacket, sizeof(sendtimepacket) - 1);
	/*sprintf(msg_debug, "SIZE [%d] Send Packet", sizeof(sendtimepacket)-1);
	AddStringToListBoxDebug(msg_debug);*/
	/*TimeReload++;
	if(TimeReload >= pSC->RELOAD_GAME)
	 TerminedSocket();*/
}

// Reloga no jogo ...
void ReloadGame()
{
	if ((GpSC->TYPE_SCRIPT == 0x04) || (GpSC->TYPE_SCRIPT == 0x01))
	{
		GpSC->TYPE_SCRIPT = 0x01; // volta como Master
		Sleep(500);
		create_room(GpSC->ROOM_NAME, GpSC->NUMBER_PLAYER, GpSC->NUMBER_HOLE, GpSC->TYPE_MAP, GpSC->TIME_MIN,
		            GpSC->ROOM_PASSWORD);
		GpSC->TYPE_SCRIPT = 0x04; // volta como Master
	}

	if ((GpSC->TYPE_SCRIPT == 0x06) || (GpSC->TYPE_SCRIPT == 0x05))
		GpSC->TYPE_SCRIPT = 0x00; // volta como Player

	StatusPause(1);
	Sleep(100);
	PacketSoma(packet_reload, sizeof(packet_reload) - 1, 0);
	EncryptSendPacket((char*)packet_reload, sizeof(packet_reload) - 1, 0);
	Sleep(800);
	SelectChannelLogin(GpGC, GpSC, GpGV);
	Sleep(2000);
	StatusPause(0);

	/*TimeReload = 0;*/
}
