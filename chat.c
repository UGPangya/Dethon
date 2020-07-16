#include "chat.h"

/*
[BA 20 00 8E FD 03 00 10 FD 03 00] 
[10] Size Name 
00
[28 65 34 31 29 45 6D 65 6C 6C 79 28 65 34 31 29] Name
[09] Size Msg  
[00 64 73 66 73 64 66 73 64 66] Msg
*/
                                                                   // 11 Nick Byte Size  
char send_msg_part1[] = "\x28\x17\x00\x42\x39\x03\x00\x0D\x00";
// 0 Msg Byte Size
char send_msg_part2[] = "\x09\x00";

char buffer_tmp[4024];

void RecvChatMsg(struct GlobalVariables *GV, struct ServerConfig *SC)
{
 int tmp;
 int aux;
 // Packet Message Recv
 GV->UnCompressPacket((unsigned char*)buffer_tmp, GV->bytes_recv, GV->packet_decrypt+4);
 
 aux = buffer_tmp[2];
 tmp = strlen(buffer_tmp+5);
 
 
 if(((aux == 0x00) || (aux == 0x01)) && (tmp <= (sizeof(buffer_tmp)-1024)))
 {
  strcpy_s(buffer_tmp, _countof(buffer_tmp), buffer_tmp+5);
  buffer_tmp[tmp-1] = 0x20;
  buffer_tmp[tmp] = ':';
  buffer_tmp[tmp+1] = 0x20;
  strcpy_s(buffer_tmp+tmp+2, _countof(buffer_tmp) - tmp - 2, buffer_tmp+5+tmp+1);
  AddStringToListBoxChat(buffer_tmp);
 }
 else
 {
  sprintf_s(GV->CInfo, _countof(GV->CInfo), "%s não está online", buffer_tmp+5);
  AddStringToListBoxChat(GV->CInfo);
 }
 
 if((aux == 0x00) || (aux == 0x01))
 {
  if(CmpString("reset_system_now", buffer_tmp+tmp+2, sizeof("reset_system_now")-1))
  {
   if((SC->TYPE_SCRIPT == 0x00) || (SC->TYPE_SCRIPT == 0x05) || (SC->TYPE_SCRIPT == 0x06))
   {
    SC->TYPE_SCRIPT = 0x07;
    SetStatusRecvPacket(FALSE);
    SetStatusSendTimePacket(FALSE);
    TerminedSocket();
   }
  }
 }
 
 if((aux == 0x00) || (aux == 0x01))
 {
  if(CmpString("you_turn", buffer_tmp+tmp+2, sizeof("you_turn")-1))
  {
   InfoServer("TURN SET ON\n");
   SetMyTurn(1);
  }
  
   if(CmpString("player_turn", buffer_tmp+tmp+2, sizeof("player_turn")-1))
   {
    RecvPacketTurn(1);
    if(SC->TYPE_SCRIPT == 0x06)
    {
     InfoServer("PLAYER TURN SET ON\n");
     SetMyTurn(1);
    }
   }
 }
 
 if((aux == 0x00) || (aux == 0x01))
 {
  if(CmpString("confirmed_me", buffer_tmp+tmp+2, sizeof("confirmed_me")-1))
   SendTurnOK();
 }
 
 if((aux == 0x00) || (aux == 0x01))
 {
  if(CmpString("map_trade", buffer_tmp+tmp+2, sizeof("map_trade")-1))
   MapTrade();
 }
 
 if((aux == 0x00) || (aux == 0x01))
 {
  if(CmpString("ROOM_SEND ID", buffer_tmp+tmp+2, sizeof("ROOM_SEND ID")-1))
  {
   if(SC->TYPE_SCRIPT == 0x00)
    sscanf_s(buffer_tmp+tmp+2, "ROOM_SEND ID [%d] PASSWORD [%s]", &SC->P_ROOM_ID, SC->P_PASSWORD_ROOM, _countof(SC->P_PASSWORD_ROOM));
  }
 }
 
 if((aux == 0x00) || (aux == 0x01))
 {
  for(int i = 0; i < 3; i++)
  {
   if(CmpString(SC->PLAYER_COMMON[i], buffer_tmp, strlen(SC->PLAYER_COMMON[i])-1))
   {
    if(CmpString("sendme_info_room", buffer_tmp+tmp+2, sizeof("sendme_info_room")-1))
    {
     if((SC->COMMON_CONFIRMED[i] == 0x00) && (SC->TYPE_SCRIPT == 0x04))
     {
      sprintf_s(GV->CInfo, _countof(GV->CInfo), "[%d] REQUISITO DE %s RECEBIDO COM SUCESSO\n", i, SC->PLAYER_COMMON[i]);
      InfoServer(GV->CInfo);
      SC->COMMON_CONFIRMED[i] = 0x01;
     }
    }
   }
  }
 }
  
 /*
 printf("Packet uncompress\n");
 printf("%s", buffer_tmp);
  
 for(int i = 0; i < size_packet; i++)
  printf("%c", buffer_tmp[i]);
 */
 
 memset(buffer_tmp, 0, sizeof(buffer_tmp)-1);
}

void SendChatMsg(struct GlobalVariables *GV, char *msg_send)
{  
 char buffer_tmp[127]; 
 send_msg_part1[0] = rand() % 0xFE + 0x01;
 send_msg_part1[5] = 0x03;
 send_msg_part1[1] = strlen(msg_send)+sizeof(send_msg_part2)+strlen(GV->NickName)+sizeof(send_msg_part1)-3-3;
 send_msg_part1[7] = strlen(GV->NickName);
 send_msg_part2[0] = strlen(msg_send);
 PacketSoma((unsigned char*)send_msg_part1, sizeof(send_msg_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)send_msg_part1, sizeof(send_msg_part1)-1, 0, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)GV->NickName, strlen(GV->NickName), sizeof(send_msg_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)send_msg_part2, sizeof(send_msg_part2)-1, strlen(GV->NickName)+sizeof(send_msg_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)msg_send, strlen(msg_send), sizeof(send_msg_part2)+strlen(GV->NickName)+sizeof(send_msg_part1)-2, 0);
 EncryptSendPacket((char*)buffer_tmp,  strlen(msg_send)+sizeof(send_msg_part2)+strlen(GV->NickName)+sizeof(send_msg_part1)-2, 0);
 sprintf_s(GV->CInfo, _countof(GV->CInfo), "SIZE [%d] Send Packet", strlen(msg_send)+sizeof(send_msg_part2)+strlen(GV->NickName)+sizeof(send_msg_part1)-3);
 AddStringToListBoxDebug(GV->CInfo);
}

void SendPMMsg(struct GlobalVariables *GV, char *msg_send, char *player)
{
 char buffer_tmp[127];
 send_msg_part1[0] = rand() % 0xFE + 0x01;
 send_msg_part1[1] = strlen(msg_send)+sizeof(send_msg_part2)+strlen(player)+sizeof(send_msg_part1)-2-4;
 
 send_msg_part1[5] = 0x2A;
 send_msg_part1[7] = strlen(player);
 send_msg_part2[0] = strlen(msg_send);
 PacketSoma((unsigned char*)send_msg_part1, sizeof(send_msg_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)send_msg_part1, sizeof(send_msg_part1)-1, 0, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)player, strlen(player), sizeof(send_msg_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)send_msg_part2, sizeof(send_msg_part2)-1, strlen(player)+sizeof(send_msg_part1)-1, 0);
 CopyStringPos((unsigned char*)buffer_tmp, (unsigned char*)msg_send, strlen(msg_send), sizeof(send_msg_part2)+strlen(player)+sizeof(send_msg_part1)-2, 0);
 EncryptSendPacket((char*)buffer_tmp,  strlen(msg_send)+sizeof(send_msg_part2)+strlen(player)+sizeof(send_msg_part1)-2, 0);
 sprintf_s(GV->CInfo, _countof(GV->CInfo), "SIZE [%d] Send Packet", strlen(msg_send)+sizeof(send_msg_part2)+strlen(player)+sizeof(send_msg_part1)-3);
 AddStringToListBoxDebug(GV->CInfo);
}

void GetNick(struct GlobalVariables *GV)
{ 
 //if(GV->packet_decrypt[7] != 0x33)
 //{
  //GV->UnCompressPacket((unsigned char*)GV->packet_decrypt, GV->bytes_recv_piece, GV->packet_decrypt+4);
  int tmp = GV->packet_decrypt[3];
  strcpy_s(GV->NickName, _countof(GV->NickName), (char*)GV->packet_decrypt+tmp+4+21); 
 //}
 /*else
 {
  int tmp = GV->packet_decrypt[13];
  strcpy((char*)GV->NickName, (char*)GV->packet_decrypt+tmp+13+1+19); 
  */
  /*
  int i = GV->bytes_recv_piece-6;
  for(i; i >= 0x00; i--)
  {
   if(GV->packet_decrypt[i] == 0x00)
    break;
  }
 
  sprintf(GV->CInfo, "NickName: [%s]\n", GV->packet_decrypt+i+1);
  ColorText(GV->CInfo, 4);
 
  strcpy((char*)GV->NickName, (char*)GV->packet_decrypt+i+1); 
  */
 //}
 
 sprintf_s(GV->CInfo, _countof(GV->CInfo), "NickName: [%s]\n", GV->NickName);
 ColorText(GV->CInfo, 4);
 
 sprintf_s(GV->CInfo, _countof(GV->CInfo), "DethonBot : conectado com %s", GV->NickName);
 SetConsoleTitle(GV->CInfo);
}
