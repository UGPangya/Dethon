#include "login_server.h"

// **********************************************************
// **********************************************************
// ********************* Packet Ref *************************
// **********************************************************
// **********************************************************

// Packet de Login.
// C??¼??MagestedMag ?61BF605EA3AF2C343DD5309CDFEFF601
//                      size                    MagestedMag                        Password MD5
// ad 43 00 00 bc 01 00 [0b] 00 [4d 61 67 65 73 74 65 64 4d 61 67 20] 00 [3631424636303545413341463243333433444435333039434446454646363031]
unsigned char packet_login_full[42+32];
unsigned char packet_login_part1[] = "\xad\x43\x00\x00\xbc\x01\x00\x0b\x00";
unsigned char packet_login_part2[] = "\x20\x00\x36\x31\x42\x46\x36\x30\x35\x45\x41\x33\x41\x46\x32\x43\x33\x34\x33\x44"
"\x44\x35\x33\x30\x39\x43\x44\x46\x45\x46\x46\x36\x30\x31\x00\x00\x00\x00\x00\x00\x00\x00";
                                                               
// Packet para selecionar o servidor Titan Boo ou Delfini.
//                                                                      byte 7 byte offset
//unsigned char server_change[] = "\x47\x07\x00\x00\xFA\x03\x00\x81\x00\x00\x00";
unsigned char server_game_change[] = "\x2C\x07\x00\x00\x46\x03\x00\x7F\x22\x00\x00";
//                                      \x2C\x07\x00\x00\x41\x03\x00\x81\x41\x03\x00\x81\x22\x00\x00
//                                      \x2C\x07\x00\x00\x41\x03\x00\x81\x22\x00\x00


/*
[number] = size 
f0 3b 00 00 f8 02 00 0b f8 02 00 [0b] 00 Part 1
4d 61 67 65 73 74 65 64 4d 61 67 (MagestedMag) User
c4 81 11 00 00 00 00 00 96 66 [06] 00 Part 2
66 63 35 34 31 31 (fc5411) Primary
[06] 00 Part3
37 32 30 2e 30 30 (720.002) Version
32 a7 ad 2a 00 00 00 00 [07] 00 Part4
31 35 38 66 32 38 30 (158f280) Secondary
*/

// Packet para logar no servidor
// ð;??ø?ø??MagestedMagÄ?????–f?fc5411?720.002§­*?????158f280
unsigned char server_login_packet_full[8+32+11+6+2+6+6+7];
//                                                        4 Byte = 0x00  size
unsigned char server_login_packet_part1[] = "\x7D\x3B\x00\x05\x57\x02\x00\x0B\x00";
//                                                                                    size
unsigned char server_login_packet_part2[] = "\xC4\x81\x11\x00\x00\x00\x00\x00\x96\x66\x06\x00";
//                                           size
unsigned char server_login_packet_part3[] = "\x06\x00";
//                                                                        size
//unsigned char server_login_packet_part4[] = "\x32\xa7\xad\x2a\x00\x00\x00\x00\x07\x00";
unsigned char server_login_packet_part4[] = "\xde\x87\xad\x2a\x00\x00\x00\x00\x07\x00";


// Fecha a conexão de quem estiver aberto.
unsigned char desconnect_client[] = "\x7A\x03\x00\x00\x50\x04\x00";

// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************

/*char CInfoLogin[1024];

int bytes_recv_login;
char PacketRecv[1024];
char PacketSend[1024];
unsigned char packet_decrypt[1024];*/

void MakePacketLoginServer(struct GeralConfig *GC, struct GlobalVariables *GV)
{
 if(GC->AutoLogin == 0)
 {
  ColorText("Login: ", 4);
  scanf_s("%s", GC->Login, _countof(GC->Login));
  ColorText("Senha: ", 4);
  scanf_s("%s", GC->Password, _countof(GC->Password));
  BarSpace();
 }
      
 MD5((unsigned char*)GC->Password, strlen(GC->Password), GC->PASSMD5);
 MD5Convert(GC->PASSMD5, GC->PASSMD5CHAR, sizeof(GC->PASSMD5));
      
 sprintf_s(GV->CInfo, _countof(GV->CInfo), "PASSWORD MD5 [%s]\n", GC->PASSMD5CHAR);
 InfoServer(GV->CInfo);
      
 // MONTAR O PACKET DO LOGIN ...
 packet_login_part1[7] = strlen(GC->Login);
 CopyStringPos(packet_login_part2, GC->PASSMD5CHAR, sizeof(GC->PASSMD5CHAR), 2, 0);
     
 CopyStringPos(packet_login_full, packet_login_part1, sizeof(packet_login_part1), 0, 0);
 CopyStringPos(packet_login_full, (unsigned char*)GC->Login, strlen(GC->Login), sizeof(packet_login_part1)-1, 0);
 CopyStringPos(packet_login_full, packet_login_part2, sizeof(packet_login_part2), strlen(GC->Login)+sizeof(packet_login_part1)-1, 0);
      
 packet_login_full[1] = (strlen(GC->Login)+sizeof(packet_login_part1)+sizeof(packet_login_part2)-2)+0x0A;
 
 //printf("************ Debug ************\n");
 //ShowPacketInHex(packet_login_full, strlen(GC.Login)+sizeof(packet_login_part1)+sizeof(packet_login_part2)-2);
      
 sprintf_s(GV->CInfo, _countof(GV->CInfo), "connect server [%s]:[%d]\n", GC->Ip, GC->Port);
 InfoServer(GV->CInfo);
}

void SendMakePacketLogin(struct GeralConfig *GC, struct GlobalVariables *GV)
{
 // SEND PACKET
 EncryptSendPacket((char*)packet_login_full, strlen(GC->Login)+sizeof(packet_login_part1)+sizeof(packet_login_part2)-2, 4);
 Sleep(100);
}

void SendPacketChangeLogin(struct GeralConfig *GC, struct ServerConfig *SC, struct GlobalVariables *GV)
{
 if(!GC->AutoLogin)
 {
  for(int i = 0; i < MAX_SERVER_CONFIG-1; i++)
  {
   if(strcmp(SC->SERVERNAME[i], "NO_SERVER"))
   {
    ColorText("SERVER ", 2);
    printf("[%02d] %s [%s]:[%d] OFFSET [0x%02X]\n", i, SC->SERVERNAME[i], SC->SERVERIP[i], SC->SERVERPORT[i], SC->OFFSETSERVER[i]);
   }
  }
         
  for(;;)
  {
   ColorText("SERVER -> ", 4);
   scanf_s("%d", &SC->server_game_change);
   if(SC->server_game_change < MAX_SERVER_CONFIG)
    break;
   else
    FailedServer("servidor inexistente\n");
  }
 }
 else
 {
  if(SC->SELECT_SERVER < MAX_SERVER_CONFIG)
  {
   SC->server_game_change = SC->SELECT_SERVER;
   sprintf_s(GV->CInfo, _countof(GV->CInfo), "AUTO SELECT SERVER ID [%d] SERVER_NAME [%s]\n", SC->SELECT_SERVER, SC->SERVERNAME[SC->server_game_change]);
   InfoServer(GV->CInfo);
  }
  else
  {
   FailedServer("servidor inexistente - set 0\n");
   SC->server_game_change = 0;
  }
 }
          
 server_game_change[7] = SC->OFFSETSERVER[SC->server_game_change];
          
 // Seleciona Servidor primário.
 EncryptSendPacket((char*)server_game_change, sizeof(server_game_change)-1, 0);
          
 /*
 PacketSoma(server_change, sizeof(server_change)-1, 0);
 SendPacket((char*)server_change, sizeof(server_change)-1);
 */
}

BOOLEAN SendServerLogin(struct GeralConfig *GC, struct ServerConfig *SC, struct GlobalVariables *GV)
{
 int tmp_byte = 0;
 
 PacketSoma(server_login_packet_part1, sizeof(server_login_packet_part1)-1, 1);
 
 server_login_packet_part1[7] = strlen(GC->Login);
 server_login_packet_part2[10] = sizeof(SC->OffsetPrimary)-1;
 server_login_packet_part3[0] = sizeof(SC->VERSION_PANGYA)-1;
 server_login_packet_part4[8] = sizeof(SC->OffsetSecondary)-1;
 
 server_login_packet_part4[0] = SC->VERSION_OFFSET[0];
 server_login_packet_part4[1] = SC->VERSION_OFFSET[1];
 server_login_packet_part4[2] = SC->VERSION_OFFSET[2];
 server_login_packet_part4[3] = SC->VERSION_OFFSET[3];
 
 server_login_packet_part2[0] = SC->OFFSET_CHAR[0];
 server_login_packet_part2[1] = SC->OFFSET_CHAR[1];
 server_login_packet_part2[2] = SC->OFFSET_CHAR[2];
 server_login_packet_part2[3] = SC->OFFSET_CHAR[3];
 
 // SizePacket
 server_login_packet_part1[1] = (strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)+sizeof(server_login_packet_part3)+sizeof(server_login_packet_part4)+sizeof(SC->OffsetSecondary)+sizeof(SC->VERSION_PANGYA)-6)-0x05;

 // PART1
 CopyStringPos(server_login_packet_full, server_login_packet_part1, sizeof(server_login_packet_part1), 0, 0);
 CopyStringPos(server_login_packet_full, (unsigned char*)GC->Login, strlen(GC->Login), sizeof(server_login_packet_part1)-1, 0);
 
 // PART2
 CopyStringPos(server_login_packet_full, server_login_packet_part2, sizeof(server_login_packet_part2), strlen(GC->Login)+sizeof(server_login_packet_part1)-1, 0);
 CopyStringPos(server_login_packet_full, (unsigned char*)SC->OffsetPrimary, sizeof(SC->OffsetPrimary), strlen(GC->Login)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)-2, 0);
 
 // PART3
 CopyStringPos(server_login_packet_full, server_login_packet_part3, sizeof(server_login_packet_part3), strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)-3, 0);
 CopyStringPos(server_login_packet_full, (unsigned char*)SC->VERSION_PANGYA, sizeof(SC->VERSION_PANGYA), strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)+sizeof(server_login_packet_part3)-4, 0);
 
 // PART4
 CopyStringPos(server_login_packet_full, server_login_packet_part4, sizeof(server_login_packet_part4), strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)+sizeof(server_login_packet_part3)+sizeof(SC->VERSION_PANGYA)-5, 0);
 CopyStringPos(server_login_packet_full, (unsigned char*)SC->OffsetSecondary, sizeof(SC->OffsetSecondary), strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)+sizeof(server_login_packet_part3)+sizeof(server_login_packet_part4)+sizeof(SC->VERSION_PANGYA)-6, 0);
 
 //printf("************ Debug ************\n");
 //ShowPacketInHex(server_login_packet_full, strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)+sizeof(server_login_packet_part3)+sizeof(server_login_packet_part4)+sizeof(SC->OffsetSecondary)+sizeof(SC->VERSION_PANGYA)-7);
 
 // SEND PACKET
 //DEBUG_MODE_TYPE(1);
 EncryptSendPacket((char*)server_login_packet_full, strlen(GC->Login)+sizeof(SC->OffsetPrimary)+sizeof(server_login_packet_part1)+sizeof(server_login_packet_part2)+sizeof(server_login_packet_part3)+sizeof(server_login_packet_part4)+sizeof(SC->OffsetSecondary)+sizeof(SC->VERSION_PANGYA)-7, 0);
 //DEBUG_MODE_TYPE(0);
 Sleep(100);
 
 // RECV PACKET
 GV->bytes_recv = DecryptRecvPacket(GV->packet_decrypt, (unsigned char*)GV->PacketRecv, sizeof(GV->PacketRecv)-1);
  
 /*if(tmp_byte < 35)
 {
  InfoServer("Packet Error!\n");
  return 0;
 }*/
 
 return 1;
}

void DesconectLogin(struct GlobalVariables *GV)
{
 EncryptSendPacket((char*)desconnect_client, sizeof(desconnect_client)-1, 0);
 Sleep(100);
 InfoServer("client desconectado!\n");
          
 // RECV PACKET
 DecryptRecvPacket(GV->packet_decrypt, (unsigned char*)GV->packet_decrypt, sizeof(GV->PacketRecv)-1);
 Sleep(100);
}
