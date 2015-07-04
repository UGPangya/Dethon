#include "map.h"

unsigned char packet_loading_map_part1[] = "\x05\x07\x00\x72\xB9\x0E\x00\x90\x00\x00\x00";
unsigned char packet_loading_map_part2[] = "\xB4\x14\x00\x73\xBA\x0C\x00\x07\x33\xCD\x0E\x00\x00\x00\x00\x00\x59\x00\x43\x03\x02\x00\x00\x14";
//unsigned char packet_loading_map_part3[] = "\xED\x04\x00\x00\x00\x1D\x00\x00";
unsigned char packet_loading_map_part4[] = "\xA5\x04\x00\x74\x2D\x48\x00\x02";
unsigned char packet_loading_map_part5[] = "\x4C\x1D\x00\x75\x06\x1A\x00\x01\x00\x00\x00\x00\x70\x0E\x00\x00\x04\xB8\x5E\x6E\xC3\x85\xEB\x38\xC4\x52\x38\x84\x42\x0A\x97\x12\x44";
unsigned char packet_loading_map_part6[] = "\xEC\x03\x00\x7E\x67\x11\x00";
unsigned char packet_loading_map_part7[] = "\x3D\x03\x00\x80\x27\x9C\x00";
unsigned char init_player_part1[] = "\x84\x03\x00\x1E\x95\x34\x00"; 
unsigned char init_player_part2[] = "\x2B\x03\x00\x1F\xC6\x22\x00"; 

unsigned char packet_exit_game[] = "\x29\x16\x00\xE5\x5C\x0F\x00\x01\xEE\xFF\x00\x00\x11\x00\x00\x00\x11\x00\x00\x00\x11\x00\x00\x00\x11\x00";
unsigned char packet_return_room_part1[] = "\xDC\x03\x00\x33\x63\x37\x00";
unsigned char packet_return_room_part2[] = "\x47\x03\x00\x31\xDB\x22\x00";

// 1 = 1 seg
#define COUNTER_TIME_SHOT 12

static bool my_turn = 0x00;
static unsigned int recv_turn;
static unsigned int counter_time_shot = COUNTER_TIME_SHOT;
static bool status_shotcheck = 0x00;

GeralConfig *MpGC;
ServerConfig *MpSC;
GlobalVariables *MpGV;

void MapCopyStructInfo(GeralConfig *GC, ServerConfig *SC, GlobalVariables *GV)
{
 MpGC = GC;
 MpSC = SC;
 MpGV = GV;
}

void InitGameMap()
{
 if((MpSC->TYPE_SCRIPT == 0x04) || (MpSC->TYPE_SCRIPT == 0x06))
 {
  InfoServer("INICIANDO JOGO\n");
  // Init Packet MasterGame
  PacketSoma(packet_loading_map_part1, sizeof(packet_loading_map_part1)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part1, sizeof(packet_loading_map_part1)-1, 0);
  
  
  PacketSoma(packet_loading_map_part2, sizeof(packet_loading_map_part2)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part2, sizeof(packet_loading_map_part2)-1, 0);
  
  /*
  PacketSoma(packet_loading_map_part3, sizeof(packet_loading_map_part3)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part3, sizeof(packet_loading_map_part3)-1, 0);
  */
  
  
  // 0x02
  packet_loading_map_part4[7] = 0x02;
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  PacketSoma(packet_loading_map_part5, sizeof(packet_loading_map_part5)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part5, sizeof(packet_loading_map_part5)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x03
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x04
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x05
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x06
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x07
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x08
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x09
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  packet_loading_map_part4[7]++;       // 0x0A
  PacketSoma(packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part4, sizeof(packet_loading_map_part4)-1, 0);
  
  PacketSoma(packet_loading_map_part6, sizeof(packet_loading_map_part6)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part6, sizeof(packet_loading_map_part6)-1, 0);
  
  PacketSoma(packet_loading_map_part7, sizeof(packet_loading_map_part7)-1, 0);
  EncryptSendPacket((char*)packet_loading_map_part7, sizeof(packet_loading_map_part7)-1, 0);
 }
 
 //time_count = 5;
 while(((MpSC->TYPE_SCRIPT == 0x04) || (MpSC->TYPE_SCRIPT == 0x06) || (MpSC->TYPE_SCRIPT == 0x02)) && ((GetStatusRecvPacket() == TRUE) && ((GetPlayersConfirmed() >= 1) || (MpSC->TYPE_SCRIPT == 0x06))))
 {
  // ... continua até acabar o jogo
  /*sprintf(buffer_tmp, "Player Online [%d]\n", GetPlayersConfirmed()+1);
  InfoServer(buffer_tmp);*/
  if(my_turn)
  {
   my_turn = 0x00; // Zera o turno logo no inicio.
   shot();
  }
  Sleep(100);
 }
 Sleep(1000);
 InfoServer("GAME FINALIZADO\n");
}

void InitPlayerMap()
{
 InfoServer("INICIANDO PLAYER MAP\n");
 PacketSoma(init_player_part1, sizeof(init_player_part1)-1, 0);
 EncryptSendPacket((char*)init_player_part1, sizeof(init_player_part1)-1, 0);
  
 PacketSoma(init_player_part2, sizeof(init_player_part2)-1, 0);
 EncryptSendPacket((char*)init_player_part2, sizeof(init_player_part2)-1, 0);
}

void ExitGame()
{
 PacketSoma(packet_exit_game, sizeof(packet_exit_game)-1, 0);
 EncryptSendPacket((char*)packet_exit_game, sizeof(packet_exit_game)-1, 0);
}

void ReturnRoom()
{
 PacketSoma(packet_return_room_part1, sizeof(packet_return_room_part1)-1, 0);
 EncryptSendPacket((char*)packet_return_room_part1, sizeof(packet_return_room_part1)-1, 0);
 
 PacketSoma(packet_return_room_part2, sizeof(packet_return_room_part2)-1, 0);
 EncryptSendPacket((char*)packet_return_room_part2, sizeof(packet_return_room_part2)-1, 0);
}

void SetMyTurn(bool _my_turn)
{
 my_turn = _my_turn;
}

bool GetTurn()
{
 return my_turn;
}

// Packet SHOT
unsigned char packet_shot_part1[] = "\xA1\x08\x00\x43\x4E\x14\x00\x01\x00\x00\x0C\x43"; // Inicio Shot
// [\xFA\x43]
unsigned char packet_shot_part2[] = "\x64\x08\x00\x47\x97\x14\x00\x02\x00\x00\xFA\x43"; // Inicia Força do Shot
unsigned char packet_shot_part3[] = "\xDF\x08\x00\x48\xC7\x14\x00\x03\x00\x00\x13\x43"; // Completa o Shot

// unknown
// [\xFA\x43]
// [\x00\x00\x13\x43] Pangya local na barrinha (rosa Inicia em 13)
// [\x00\x00\x0B\x43] Mude para < Sempre Pangya ou 0x00

//0E9750D8           13 BC D3 3E 24 12 74 E9 8B 61 AB BA A4     ¼Ó>$té‹a«º¤
//0E9750E8  41 B8 88 D9                                      A¸ˆÙ

/*
unsigned char packet_shot_part4_shot_normal[] = "\xB3\x3B\x00\x49\x11\x12\x00\x00\x00\x00\x00\xFA\x43\x00\x00\x13\x43\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\xD2\x03\x00\x00\x40\x47\xD5\xBD\xEC\x61\x00\x00\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x40\x40\x52\xCD\xAC\x41";
unsigned char packet_shot_part4_shot_pangya[] = "\x66\x44\x00\xFF\x97\x12\x00\x01\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\xFA\x43\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\xED\x01\x00\x00\x60\xCE\xBB\xBD\x64\x34\x00\x00\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x40\x40\x38\xAE\x3F\x41";
*/


// WORKD 15/11/2014
//unsigned char packet_shot_part4_shot_normal[] = "\x6E\x43\x00\xEE\xD1\x12\x00\x00\x00\x00\x00\xF8\x43\x00\x00\x08\x43\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x55\x02\x00\x00\x80\x74\x95\xBC\xA3\x48\x00\x00\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x6B\x4B\x5E\x41";
//unsigned char packet_shot_part4_shot_pangya[] = "\x6E\x43\x00\xEE\xD1\x12\x00\x00\x00\x00\x00\xF8\x43\x00\x00\x08\x43\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\x55\x02\x00\x00\x80\x74\x95\xBC\xA3\x48\x00\x00\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x6B\x4B\x5E\x41";

// Fecha o Game do Player adversário ...
//unsigned char packet_shot_part4_shot_normal[] = "\x9E\x4C\x00\x5E\xA1\x12\x00\x01\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x50\x6C\x65\x61\x73\x65\x20\x63\x6F\x6E\x66\x69\x67\x75\x72\x65\x20\x79\x6F\x75\x72\x20\x43\x68\x61\x74\x20\x4D\x61\x63\x72\x6F\x73\x2E\x00\xFE\xBD\x2D\x6B\xB2\xF4\xF9\x00\x00\x00\xFF\x5E\x9C\xEF\x00\x01\x0C\xFF\xD0\x12\x83\x04\xD0\x12\x83\x04";
//unsigned char packet_shot_part4_shot_pangya[] = "\x9E\x4C\x00\x5E\xA1\x12\x00\x01\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x08\x50\x6C\x65\x61\x73\x65\x20\x63\x6F\x6E\x66\x69\x67\x75\x72\x65\x20\x79\x6F\x75\x72\x20\x43\x68\x61\x74\x20\x4D\x61\x63\x72\x6F\x73\x2E\x00\xFE\xBD\x2D\x6B\xB2\xF4\xF9\x00\x00\x00\xFF\x5E\x9C\xEF\x00\x01\x0C\xFF\xD0\x12\x83\x04\xD0\x12\x83\x04";


unsigned char packet_shot_part4_shot_normal[] = "\x43\x4C\x00\x2C\xBB\x12\x00\x01\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x80\x9F\x43\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x8D\x07\x00\x00\x8B\x97\x2F\x40\x0D\x18\x00\x00\x00\x00\x0C\x43\x0D\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\xA0\xE1\x9A\x41";
unsigned char packet_shot_part4_shot_pangya[] = "\x43\x4C\x00\x2C\xBB\x12\x00\x01\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x80\x9F\x43\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x04\x00\x00\x00\x00\x8D\x07\x00\x00\x8B\x97\x2F\x40\x0D\x18\x00\x00\x00\x00\x0C\x43\x0D\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\xA0\xE1\x9A\x41";

//unsigned char packet_shot_part4_shot_normal[] = "\xF9\x43\x00\x00\x07\x43\x00\x01\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\xBA\x02\x00\x00\xC0\x96\x4E\xBD\x64\x59\x00\x00\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x87\x9D\x7E\x41";
//unsigned char packet_shot_part4_shot_pangya[] = "\xF9\x43\x00\x00\x07\x43\x00\x01\x00\x00\x00\x00\x00\x00\x03\x00\x00\x00\x00\xBA\x02\x00\x00\xC0\x96\x4E\xBD\x64\x59\x00\x00\x00\x00\x0C\x43\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x40\x00\x00\x00\x00\x00\x00\x00\x00\x87\x9D\x7E\x41";


/*
0013F548   99 00 00 00 C2 2E 2C C3 F7 27 43 C1 CA B7 2D C2  ™...Â.,Ã÷'CÁÊ·-Â
0013F558   02 00 00 55 00 00 00 00 00 00 00 00 00 00 00 00  ..U............
0013F568   08 10 00 C7                                      .Ç
*/

//0F92727C                                      4F 38 00 6F              O8.o
//0F92728C  00 1B 00                                         ..

// 2EE0
// PANG HERE \xA0\x0F
//unsigned char packet_shot_geral_part1[] = "\xA9\x38\x00\x03\xD9\x1B\x00";
unsigned char packet_shot_geral_part1[] = "\xB9\x39\x00\x29\xFE\x1B\x00";
//unsigned char packet_shot_geral_part1[] = "\x19\x41\xCC\x07\x00\x12\xCE"
//unsigned char packet_shot_geral_part1[]   = "\x12\x39\x00\x0C\x2C\x1B\x00";
//unsigned char packet_shot_geral_part2[] = "\x1B\x00\x00\x00\xD6\x0E\x66\xC2\xFD\x3B\x46\xC1\xA2\x91\x1D\x44\x02\x00\x00\x08\x00\x00\x00\x1E\x00\x00\x00\x00\x40\x00\x00\x00\x08\x10\x00\x3B";
//unsigned char packet_shot_geral_part2[] = "\xC3\x00\x00\x00\x53\x43\x84\x42\xD5\xBE\x0C\xC1\x33\xA4\x12\x44\x04\x00\x00\x32\x00\x00\x00\x14\x00\x00\x00\x00\x11\x00\x00\x00\x08\x10\x00\x02";
//unsigned char packet_shot_geral_part2[] = "\xC3\x00\x00\x00\x53\x43\x84\x42\xD5\xBE\x0C\xC1\x33\xA4\x12\x44\x04\x00\xA0\x0F\x00\x00\x00\x14\x00\x00\x00\x00\x11\x00\x00\x00\x08\x10\x00\x02";
//unsigned char packet_shot_geral_part2[] = "\xC3\x00\x00\x00\x53\x43\x84\x42\xD5\xBE\x0C\xC1\x33\xA4\x12\x44\x04\x00\x00\xA0\x0F\x00\x00\xFF\x00\x00\x00\x00\x11\x00\x00\x00\x08\x10\x00\x02";
//unsigned char packet_shot_geral_part2[]   = "\xC3\x00\x00\x00\x53\x43\x84\x42\xD5\xBE\x0C\xC1\x33\xA4\x12\x44\x04\x00\x00\xA0\x0F\x00\x00\xFF\x00\x00\x00\x00\x11\x00\x00\x00\x08\x10\x00\x02";
//unsigned char packet_shot_geral_part2[] = "\x30\x00\x00\x00\x1C\x57\x9B\xC3\xE9\xDC\x90\xC1\xB3\x39\xCF\xC2\x02\x00\x00\x0C\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x08\x10\x00\xF6\x02";
//unsigned char packet_shot_geral_part2[] = "\x30\x00\x00\x00\x1C\x57\x9B\xC3\xE9\xDC\x90\xC1\xB3\x39\xCF\xC2\x02\x00\x00\x0C\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x08\x10\x00\xF6\x02";
//unsigned char packet_shot_geral_part2[] = "\x69\x00\x00\x00\xE7\x90\x36\x42\x5A\x1A\x31\xC1\x87\x95\xF1\x43\x02\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x08\x10\x00\x1E\x02";
//unsigned char packet_shot_geral_part2[]   = "\x0C\x00\x00\x00\x22\x91\xF7\x41\x6B\xCD\x4A\xC1\xAE\x5C\xDB\x43\x04\x00\x00\x1D\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x08\x80\x00\x45\x00";//\x00\x12
unsigned char packet_shot_geral_part2[]   = "\x0C\x00\x00\x00\x22\x91\xF7\x41\x6B\xCD\x4A\xC1\xAE\x5C\xDB\x43\x04\x00\x00\xE8\x03\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x08\x80\x00\x45\x00";//\x00\x12

unsigned char packet_shot_geral_part3[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

unsigned char packet_shot_geral_part2_tmp[sizeof(packet_shot_geral_part2)];

unsigned char packet_shot_finity[] = "\xE0\x05\x00\x6B\x72\x1C\x00\x01\x00";

unsigned char packet_shot_practice[] = 
"\x09\x5A\x00\x43\x74\x2F\x01\x00\x00\x80\x3F\x91\x53\x96\x40\xBC\xEF\x23\x42\x8E"
"\x62\x68\x43\x01\x52\x38\x84\x42\xAC\xFC\x00\xC1\x0A\x97\x12\x44\x25\x13\xB1\x3F"
"\x00\x00\x00\x00\x37\xE1\xB8\x3F\x0C\x50\x49\x3A\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\xC0\x3F\x0C\x50\x49\x3A\x00\x00\x00\x00\x6C\x43\x00\x00\x66\x43\xCD\xCC"
"\x0C\x3F\x7B\x14\xCE\x3F\x00\x00\x6C\x43\x74\x01\x00\x00";

void shot()
{
 int pct = (rand() % 100) + 1;
 
 PacketSoma(packet_shot_part1, sizeof(packet_shot_part1)-1, 0);
 EncryptSendPacket((char*)packet_shot_part1, sizeof(packet_shot_part1)-1, 0);
 
 PacketSoma(packet_shot_part2, sizeof(packet_shot_part2)-1, 0);
 EncryptSendPacket((char*)packet_shot_part2, sizeof(packet_shot_part2)-1, 0);
 
 PacketSoma(packet_shot_part3, sizeof(packet_shot_part3)-1, 0);
 EncryptSendPacket((char*)packet_shot_part3, sizeof(packet_shot_part3)-1, 0);
 
 if((MpSC->TYPE_SCRIPT == 0x02))
 {
  PacketSoma(packet_shot_practice, sizeof(packet_shot_practice)-1, 0);
  EncryptSendPacket((char*)packet_shot_practice, sizeof(packet_shot_practice)-1, 0);
 }
 
 
 if(pct <= MpSC->PCT_PANGYA)
 {
  sprintf(MpGV->CInfo, "ACERTOU PANGYA PCT PROGRAMADA [%d] PCT RAND [%d]\n", MpSC->PCT_PANGYA, pct);
  InfoColor(MpGV->CInfo, 2);
  PacketSoma(packet_shot_part4_shot_pangya, sizeof(packet_shot_part4_shot_pangya)-1, 0);
  EncryptSendPacket((char*)packet_shot_part4_shot_pangya, sizeof(packet_shot_part4_shot_pangya)-1, 0);
 }
 else
 {
  sprintf(MpGV->CInfo, "ERROU PANGYA PCT PROGRAMADA [%d] PCT RAND [%d]\n", MpSC->PCT_PANGYA, pct);
  InfoColor(MpGV->CInfo, 1);
  PacketSoma(packet_shot_part4_shot_normal, sizeof(packet_shot_part4_shot_normal)-1, 0);
  EncryptSendPacket((char*)packet_shot_part4_shot_normal, sizeof(packet_shot_part4_shot_normal)-1, 0);
 }
 
        
 Confirmed_Shot();
 InfoServer("SHOT!\n");
 if((MpSC->TYPE_SCRIPT == 0x04) || (MpSC->TYPE_SCRIPT == 0x06))
 SendShotOK();

 if((MpSC->TYPE_SCRIPT == 0x04))
  SendPlayerTurn();
 
 counter_time_shot = COUNTER_TIME_SHOT;
 
 Sleep(100);
 /*
 PacketSoma(packet_shot_finity, sizeof(packet_shot_finity)-1, 0);
 EncryptSendPacket((char*)packet_shot_finity, sizeof(packet_shot_finity)-1, 0);*/
 
 //InfoServer("SHOT TERMINADO\n");
 SetMyTurn(0);
}

void Confirmed_Shot()
{
 char buffer_tmp[1024];
 /*for(int i = 0; i < sizeof(MpGV->BytesEncryptMap); i++)
 {
  printf("%02x ", (unsigned char)MpGV->BytesEncryptMap[i]);
 }
 
 printf("\n");*/

 /*for(int i = 0; i < sizeof(packet_shot_geral_part3)-1; i++)
 {
  packet_shot_geral_part3[i] = rand() % 0xFF;
 }
 
 packet_shot_geral_part2[0] = rand() % 0xFF;
 packet_shot_geral_part1[0] = rand() % 0xFF;
 */
 
 CopyStringPos((unsigned char*)packet_shot_geral_part2_tmp, packet_shot_geral_part2, sizeof(packet_shot_geral_part2), 0, 0);
 
 MpGV->CallEncryptMapPacket((unsigned char*)MpGV->BytesEncryptMap, (unsigned char*)packet_shot_geral_part2_tmp);
 PacketSoma(packet_shot_geral_part1, sizeof(packet_shot_geral_part1)-1, 0);
 
 CopyStringPos((unsigned char*)buffer_tmp, packet_shot_geral_part1, sizeof(packet_shot_geral_part1), 0, 0);
 CopyStringPos((unsigned char*)buffer_tmp, packet_shot_geral_part2_tmp, sizeof(packet_shot_geral_part2_tmp), sizeof(packet_shot_geral_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, packet_shot_geral_part3, sizeof(packet_shot_geral_part3), sizeof(packet_shot_geral_part2_tmp)+sizeof(packet_shot_geral_part1)-2, 0);
 
 EncryptSendPacket((char*)buffer_tmp, sizeof(packet_shot_geral_part1)+sizeof(packet_shot_geral_part2_tmp)+sizeof(packet_shot_geral_part3)-3, 0);
}

//unsigned char recv_yourturn_ok_part1[] = "\x7C\x05\x00\xF3\xFE\x1C\x00\x01\x24";
unsigned char recv_yourturn_ok_part1[] ="\x00\x05\x00\xA3\x00\x1C\x00\x01\x00";
unsigned char recv_yourturn_ok_part2[] = "\x00\x03\x00\xA4\x00\x22";

void SendTurnOK()
{  
 InfoServer("TURN CONFIRMED\n");
 SendPacketLoginConfirmation();
 
 PacketSoma(recv_yourturn_ok_part1, sizeof(recv_yourturn_ok_part1)-1, 0);
 EncryptSendPacket((char*)recv_yourturn_ok_part1, sizeof(recv_yourturn_ok_part1)-1, 0);
 
 //PacketSoma(recv_yourturn_ok_part2, sizeof(recv_yourturn_ok_part2)-1, 0);
 //EncryptSendPacket((char*)recv_yourturn_ok_part2, sizeof(recv_yourturn_ok_part2)-1, 0);
}

void SendShotOK()
{
 InfoServer("YOUR SHOT CONFIRMED\n");
 
 SendPacketLoginConfirmation();
  
 PacketSoma(recv_yourturn_ok_part1, sizeof(recv_yourturn_ok_part1)-1, 0);
 EncryptSendPacket((char*)recv_yourturn_ok_part1, sizeof(recv_yourturn_ok_part1)-1, 0);
}

unsigned char trade_map_packet01[] = "\xD8\x05\x00\xCA\x5E\x1C\x00\x01\x00";
unsigned char trade_map_packet02[] = "\x2E\x03\x00\xCB\xCD\x40\x01";
//unsigned char trade_map_packet03[] = "\xDE\x1D\x00\xCC\xB5\x1A\x00\x02\x00\x00\x00\x00\x70\x0E\x00\x00\x04\xB8\x5E\x6E\xC3\x85\xEB\x38\xC4\xD7\xA3\x6E\x42\x0A\x67\x0B\x44";
unsigned char trade_map_packet03[] = "\x2C\x1D\x00\x60\x46\x1A\x00\x02\x00\x00\x00\x00\x82\x14\x00\x00\x03\x52\xB8\x31\x42\x48\x01\x91\xC3\x0A\x57\xA3\x42\x29\xDC\xDF\x43";
unsigned char trade_map_packet04[] = "\xD6\x03\x00\xCD\xB3\x11\x00";
unsigned char trade_map_packet05[] = "\x97\x03\x00\xCE\x26\x34\x00"; // init Map já existe ...

void MapTrade()
{
 PacketSoma(trade_map_packet01, sizeof(trade_map_packet01)-1, 0);
 EncryptSendPacket((char*)trade_map_packet01, sizeof(trade_map_packet01)-1, 0);
 
 PacketSoma(trade_map_packet02, sizeof(trade_map_packet02)-1, 0);
 EncryptSendPacket((char*)trade_map_packet02, sizeof(trade_map_packet02)-1, 0);
 
 PacketSoma(trade_map_packet03, sizeof(trade_map_packet03)-1, 0);
 EncryptSendPacket((char*)trade_map_packet03, sizeof(trade_map_packet03)-1, 0);
 
 PacketSoma(trade_map_packet04, sizeof(trade_map_packet04)-1, 0);
 EncryptSendPacket((char*)trade_map_packet04, sizeof(trade_map_packet04)-1, 0);
}

unsigned char finity_part01[] = 

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

/*
"\x24\xEE\x00\x30\x4B\x06\x00\x03\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x31\x00\x00\x00\x09\x5A\x7B\x43"
"\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xDC\x01\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\xA6\x5B\xFC\x3F\x00\x00\x00\x00\xFF\xFF\xFF"
"\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
*/

/*"\x24\xEE\x00\x30\x4B\x06\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0A\x00\x00\x00\x09\x5A\x7B\x43"
"\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xDC\x01\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\xA6\x5B\xFC\x3F\x00\x00\x00\x00\xFF\xFF\xFF"
"\xFF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00";*/

unsigned char finity_part02[] = "\xBE\x08\x00\x5C\x9F\x63\x00\x00\xC3\xF5\xA8\x3E";
//4E 05 00 1E 79 1C 00 01 00

void FinityGame()
{ 
 PacketSoma(finity_part01, sizeof(finity_part01)-1, 0);
 EncryptSendPacket((char*)finity_part01, sizeof(finity_part01)-1, 0);
 
 PacketSoma(finity_part02, sizeof(finity_part02)-1, 0);
 EncryptSendPacket((char*)finity_part02, sizeof(finity_part02)-1, 0);
 
 SetPlayersConfirmed(0);
 
 if((MpSC->TYPE_SCRIPT == 0x06))
  MpSC->TYPE_SCRIPT = 0x00;
}

void SendPlayerTurn()
{
 recv_turn = 0x00;
 while(recv_turn != 0x01)
 {
  SendChatMsg(MpGV, "player_turn");  
  Sleep(800);
  if(recv_turn == 0x01)
   break;
  Sleep(800);                    
 }
 counter_time_shot = COUNTER_TIME_SHOT;
}

void RecvPacketTurn(bool __recv_turn)
{
 recv_turn = __recv_turn;
}

DWORD ThreadShotCounterCheck(LPVOID)
{
 while(true)
 {
  Sleep(1000);
  if(status_shotcheck)
  {
   if(counter_time_shot <= 0x00)
    SendPlayerTurn();
   else
    counter_time_shot--;
  }
 }
}

void StatusShotCounterCheck(bool _status_shotcheck)
{
 status_shotcheck = _status_shotcheck;
}
