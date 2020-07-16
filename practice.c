#include "practice.h"

#include <conio.h>
#include <stdio.h>
#include "connect.h"
#include "encrypt.h"
#include "map.h"
#include "room.h"

#define MAX_PANGS 1800000000
unsigned long int pangs_counter;

struct GeralConfig* PpGC;
struct ServerConfig* PpSC;
struct GlobalVariables* PpGV;

//unsigned char enter_practice_part01[] = "\x99\x41\x00\x0B\x72\x08\x00\x00\x00\x00\x00\x00\x40\x77\x1B\x00\x01\x13\x12\x0A\x04\x01\x07\x00\x00\x00\x1B\x00\x53\x69\x6E\x67\x6C\x65\x20\x50\x6C\x61\x79\x65\x72\x20\x50\x72\x61\x63\x74\x69\x63\x65\x20\x4D\x6F\x64\x65\x08\x00\x4D\x44\x41\x79\x4E\x44\x51\x30\x00\x00\x00\x00";
unsigned char enter_practice_part01[] =
	"\xB4\x45\x00\x06\x6B\x08\x00\x00\x00\x00\x00\x00\x40\x77\x1B\x00"
	"\x01\x13\x12\x14\x04\x01\x07\x00\x00\x00\x00\x00\x00\x00\x1B\x00\x53\x69\x6E\x67"
	"\x6C\x65\x20\x50\x6C\x61\x79\x65\x72\x20\x50\x72\x61\x63\x74\x69\x63\x65\x20\x4D"
	"\x6F\x64\x65\x08\x00\x4D\x44\x41\x77\x4D\x6A\x4D\x30\x00\x00\x00";

// \xB4\x45\x00\x06(\x6B\x08\x00\x00\x6B) Remove ()

unsigned char enter_practice_part02[] = "\x66\x03\x00\x0C\x57\x23\x00";

// 44 45 00 0A DE

//(byte 11 is map)
unsigned char select_map_practice[] =
	//"\x86\x08\x00\x11\x04\x0A\x00\xFF\xFF\x01\x03\x0D";

	//"\x4d\x08\x00\x4a\x12\x0a\x00\xff\xff\x01\x03\x00";
	"\x1a\x08\x00\x0c\xdf\x0a\x00\xff\xff\x01\x03\x00";

//"\x4D\x08\x00\x4A\x12\x0A\x00\xFF\xFF\x01\x03\x00";


unsigned char start_practice_part01[] = "\x7E\x07\x00\x15\x96\x0E\x00\x18\x00\x00\x00";
unsigned char start_practice_part02[] =
	"\x5F\x14\x00\x16\x03\x0C\x00\x07\x33\xCD\x0E\x00\x00\x00\x00\x00\x59\x00\x43\x03\x00\x00\x00\x14";
unsigned char start_practice_part03[] = "\x4E\x04\x00\x00\xC0\x1D\x00\x00";
unsigned char start_practice_part04[] =
	"\x54\x1D\x00\x17\xE5\x1A\x00\x01\x00\x00\x00\x00\x59\x12\x00\x00\x04\x3D\xAA\xBB\xC3\xC3\x05\x05\xC4\x5C\x0F\xC1\x43\x7B\xF4\xBC\x43";
unsigned char start_practice_part05[] = "\x99\x03\x00\x19\x72\x11\x00";
unsigned char start_practice_part06[] = "\xED\x03\x00\x1A\xBE\x34\x00";

static int counter_hole = 0x00;

void PracticeCopyStructInfo(struct GeralConfig* GC, struct ServerConfig* SC, struct GlobalVariables* GV)
{
	PpGC = GC;
	PpSC = SC;
	PpGV = GV;
}

DWORD InitPractice(LPVOID Param)
{
	//InfoServer("INICIANDO - PRACTICE (PATCHED)\n");

	Sleep(1500);

	InfoServer("INICIANDO - PRACTICE\n");
	PacketSoma(enter_practice_part01, sizeof(enter_practice_part01) - 1, 0);
	EncryptSendPacket((char*)enter_practice_part01, sizeof(enter_practice_part01) - 1, 0);


	//PacketSoma(enter_practice_part02, sizeof(enter_practice_part02)-1, 0);
	//EncryptSendPacket((char*)enter_practice_part02, sizeof(enter_practice_part02)-1, 0);

	Sleep(5000000);

	Sleep(PpSC->SPEED_SEND_PACKET);

	InfoServer("SELECIONANDO MAPA - PRACTICE\n");

	select_map_practice[11] = PpSC->TYPE_MAP;

	// Select Map
	PacketSoma(select_map_practice, sizeof(select_map_practice) - 1, 0);
	EncryptSendPacket((char*)select_map_practice, sizeof(select_map_practice) - 1, 0);

	Sleep(50000);

	SetPlayersConfirmed(1);

	while (true)
	{
		PpSC->TYPE_SCRIPT = 0x02;
		counter_hole = 0x00;

		PacketSoma(start_practice_part01, sizeof(start_practice_part01) - 1, 0);
		EncryptSendPacket((char*)start_practice_part01, sizeof(start_practice_part01) - 1, 0);

		Sleep(PpSC->SPEED_SEND_PACKET);

		PacketSoma(start_practice_part02, sizeof(start_practice_part02) - 1, 0);
		EncryptSendPacket((char*)start_practice_part02, sizeof(start_practice_part02) - 1, 0);

		Sleep(PpSC->SPEED_SEND_PACKET);
		//PacketSoma(start_practice_part03, sizeof(start_practice_part03)-1, 0);
		//EncryptSendPacket((char*)start_practice_part03, sizeof(start_practice_part03)-1, 0);

		PacketSoma(start_practice_part04, sizeof(start_practice_part04) - 1, 0);
		EncryptSendPacket((char*)start_practice_part04, sizeof(start_practice_part04) - 1, 0);

		Sleep(PpSC->SPEED_SEND_PACKET);

		PacketSoma(start_practice_part05, sizeof(start_practice_part05) - 1, 0);
		EncryptSendPacket((char*)start_practice_part05, sizeof(start_practice_part05) - 1, 0);

		Sleep(PpSC->SPEED_SEND_PACKET);

		PacketSoma(start_practice_part06, sizeof(start_practice_part06) - 1, 0);
		EncryptSendPacket((char*)start_practice_part06, sizeof(start_practice_part06) - 1, 0);

		SetMyTurn(1);

		Sleep(1000);
		InfoServer("START MAP - PRACTICE\n");
		InitGameMap();
		Sleep(500);
	}
}

unsigned char practice_trade_map_packet01[] = "\xD8\x05\x00\xCA\x5E\x1C\x00\x01\x00";
unsigned char practice_trade_map_packet02[] = "\x2E\x03\x00\xCB\xCD\x40\x01";
unsigned char practice_trade_map_packet03[] =
	"\xDE\x1D\x00\xCC\xB5\x1A\x00\x02\x00\x00\x00\x00\x70\x0E\x00\x00\x04\xB8\x5E\x6E\xC3\x85\xEB\x38\xC4\xD7\xA3\x6E\x42\x0A\x67\x0B\x44";
unsigned char practice_trade_map_packet04[] = "\xD6\x03\x00\xCD\xB3\x11\x00";
unsigned char practice_trade_map_packet05[] = "\x97\x03\x00\xCE\x26\x34\x00"; // init Map já existe ...

unsigned char practice_finity_part01[] =

	"\x57\xEE\x00\x5A\x99\x06\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0A\x00\x00\x00\x27\x41\x84\x3D"
	"\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00"
	"\x00\x00\x01\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x27\x41\x84\x3D\xFF\xFF\xFF"
	"\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00";

void TradeMapPractice()
{
	/*if(counter_hole <= 18)
	{
	 PacketSoma(practice_finity_part01, sizeof(practice_finity_part01)-1, 0);
	 EncryptSendPacket((char*)practice_finity_part01, sizeof(practice_finity_part01)-1, 0);
	 
	 Sleep(100);
	
	 PacketSoma(practice_trade_map_packet01, sizeof(practice_trade_map_packet01)-1, 0);
	 EncryptSendPacket((char*)practice_trade_map_packet01, sizeof(practice_trade_map_packet01)-1, 0);
	
	 PacketSoma(practice_trade_map_packet02, sizeof(practice_trade_map_packet02)-1, 0);
	 EncryptSendPacket((char*)practice_trade_map_packet02, sizeof(practice_trade_map_packet02)-1, 0);
	
	 PacketSoma(practice_trade_map_packet03, sizeof(practice_trade_map_packet03)-1, 0);
	 EncryptSendPacket((char*)practice_trade_map_packet03, sizeof(practice_trade_map_packet03)-1, 0);
	
	 PacketSoma(practice_trade_map_packet04, sizeof(practice_trade_map_packet04)-1, 0);
	 EncryptSendPacket((char*)practice_trade_map_packet04, sizeof(practice_trade_map_packet04)-1, 0);
	
	 PacketSoma(practice_trade_map_packet05, sizeof(practice_trade_map_packet05)-1, 0);
	 EncryptSendPacket((char*)practice_trade_map_packet05, sizeof(practice_trade_map_packet05)-1, 0);
	 InfoColor("TROCA DE HOLE EFETUADO COM SUCESSO\n", 3);
	 counter_hole++;
	 Sleep(500);
	 //SetMyTurn(1);
	}
	else
	{*/
	pangs_counter += 4000;
	sprintf_s(PpGV->CInfo, _countof(PpGV->CInfo), "PARTIDA TERMINADA - PANGS [4000] - TOTAL [%d]\n", pangs_counter);
	InfoColor(PpGV->CInfo, 2);
	PacketSoma(practice_trade_map_packet01, sizeof(practice_trade_map_packet01) - 1, 0);
	EncryptSendPacket((char*)practice_trade_map_packet01, sizeof(practice_trade_map_packet01) - 1, 0);

	PacketSoma(practice_finity_part01, sizeof(practice_finity_part01) - 1, 0);
	EncryptSendPacket((char*)practice_finity_part01, sizeof(practice_finity_part01) - 1, 0);

	if (pangs_counter >= MAX_PANGS)
	{
		InfoColor("ALCANDOU O LIMITE DE PANGS", 4);
		_getch();
		exit(0);
	}

	PpSC->TYPE_SCRIPT = 0x08;
	counter_hole = 0x00;
	//}
}
