#include "connect.h"

WSADATA data;
SOCKET GSocket;
SOCKADDR_IN addr_sock;

bool init_connect(GeralConfig *GC)
{
 WSAStartup(0x202, &data);
 GSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
 addr_sock.sin_family = AF_INET;
 addr_sock.sin_port = htons(GC->Port);
 addr_sock.sin_addr.s_addr = inet_addr(GC->Ip);
 
 if(connect(GSocket, (SOCKADDR*)&addr_sock, sizeof(struct sockaddr)) == SOCKET_ERROR)
  return 0;
 
 return 1;
}

void SendPacket(char *buffer_send, unsigned int size)
{
 if(RECV_DEBUG_MODE_TYPE() == 1)
  InfoServer("Send Packet\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("Send Packet");
  
 ShowPacketInHex((unsigned char*)buffer_send, size);
   
 send(GSocket, (const char*)buffer_send, size, 0);
}

void EncryptSendPacket(char *buffer_send, unsigned int size, int size_finally)
{
 char msg_debug[1024];
 if(RECV_DEBUG_MODE_TYPE() == 1)
  InfoServer("Send Packet Encrypt\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("Send Packet Encrypt");
  
 sprintf(msg_debug, "SIZE [%d] Send Packet", size_finally+size);
 AddStringToListBoxDebug(msg_debug);
  
 unsigned char *buffer_encrypt = (unsigned char*)malloc(sizeof(unsigned char)*size+4);
 ClientEncryptPacket(buffer_encrypt, (unsigned char*)buffer_send, size);
 send(GSocket, (const char*)buffer_encrypt, size+size_finally, 0);
}

unsigned int DecryptRecvPacket(unsigned char *buffer_decrypt, unsigned char *buffer_recv, unsigned int size)
{
 if(RECV_DEBUG_MODE_TYPE() == 1)
  InfoServer("Recv Packet Decrypt\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("Recv Packet Decrypt");
  
 unsigned int tmp_other = 0x01;
 unsigned int retn_tmp;
 unsigned int tmp_ebp;
 
 retn_tmp = recv(GSocket, (char*)buffer_recv, size, 0);

 if(retn_tmp != 0 && retn_tmp != -1)
 {
  for(;;)
  {
   tmp_ebp = ((unsigned char)buffer_recv[tmp_other+1]*0x100)+(unsigned char)buffer_recv[tmp_other]+3;
   
   ENCDecryptPacket(buffer_decrypt, (unsigned char*)buffer_recv+tmp_other-1, tmp_ebp);
 
   if(tmp_other+((unsigned char)buffer_recv[tmp_other+1]*0x100)+(unsigned char)buffer_recv[tmp_other]+3 <= retn_tmp-1)
   {
    if(RECV_DEBUG_MODE_TYPE() == 1)
     printf("Info Next -> %x Addr %x\n", ((unsigned char)buffer_recv[tmp_other+1]*0x100)+(unsigned char)buffer_recv[tmp_other]+3, tmp_other+((unsigned char)buffer_recv[tmp_other+1]*0x100)+(unsigned char)buffer_recv[tmp_other]+3);
    tmp_other += ((unsigned char)buffer_recv[tmp_other+1]*0x100)+(unsigned char)buffer_recv[tmp_other]+3;
   }
   else
    break;
  }
 }
 
 return retn_tmp;
}

unsigned int RecvPacket(char *buffer_recv, unsigned int size)
{
 int tmp_byte;
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  InfoServer("Recv Packet Encrypt\n");
  
 tmp_byte = recv(GSocket, buffer_recv, size, 0);
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Encrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Encrypt ************");
 
 ShowPacketInHex((unsigned char*)buffer_recv, tmp_byte);
 
 return tmp_byte;
}

void TerminedSocket()
{
 closesocket(GSocket);
}
