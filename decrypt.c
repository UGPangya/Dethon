#include "decrypt.h"

short int SetByteInfoDecrypt = 0;

void SetByteDecrypt(unsigned short int byte)
{
 SetByteInfoDecrypt = byte;
}

void DecryptPacket(unsigned char *packet_buffer, unsigned int size_packet)
{
 int tmp;
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Encrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Encrypt *************");
 
 ShowPacketInHex(packet_buffer, size_packet);
 
 for(int i = size_packet-1; i > 6; i--)
 {
  tmp = packet_buffer[i];
  tmp ^= packet_buffer[i-4];
  packet_buffer[i-4] = tmp;
 }
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Decrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Decrypt *************");
  
 ShowPacketInHex(packet_buffer, size_packet+4);
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("*********************************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("*********************************");
}

void ENCDecryptPacket(unsigned char *packet_decrypt, unsigned char *packet_buffer, unsigned int size_packet)
{
 unsigned short int tmp_byte;
 unsigned short int tmp_bl;
 unsigned short int tmp_xor;
 
 //printf("SetByteInfo [%d]\n", SetByteInfo);
 tmp_byte = SetByteInfoDecrypt << 8;
 tmp_byte += packet_buffer[0];
 //printf("Shif 8 << byte [%x] + packet_buffer[0]\n", tmp_byte);
 tmp_bl = TabletEncrypt[tmp_byte];
 //printf("tmp_bl [%x]\n", tmp_bl);
 
 tmp_xor = packet_buffer[3];
 tmp_xor ^= tmp_bl;
 
 //printf("xor [%x]\n", tmp_xor);
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Encrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Encrypt *************");
  
 ShowPacketInHex(packet_buffer, size_packet);
 
 packet_buffer[7] ^= tmp_xor;
 packet_buffer[8] ^= 0x00;
 packet_buffer[9] ^= 0x00;
 packet_buffer[10] ^= 0x00;
 
 CopyString((char*)packet_decrypt, (char*)packet_buffer, size_packet);
 
 for(int i = 10; i < size_packet; i++)
 {
  //printf("%x %x = %x\n", packet_buffer[i], packet_buffer[i-4], packet_buffer[i] ^packet_buffer[i-4]);
  //packet_buffer[i] ^= packet_buffer[i-4];
  packet_decrypt[i] ^= packet_decrypt[i-4];
 }
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Decrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Decrypt *************");
  
 ShowPacketInHex(packet_decrypt, size_packet+4);
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("*********************************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("*********************************");
}


void EMCClientDecryptPacketSend(/*unsigned char *packet_encrypt,*/ unsigned char *packet_buffer, unsigned int size_packet)
{
 int tmp;
 
 unsigned short int tmp_byte;
 unsigned short int tmp_bl;
 unsigned short int tmp_dl;
 
 //string_copy(packet_encrypt, packet_buffer, 12);
 
 tmp_byte = SetByteInfoDecrypt << 8;
 
 //printf("%x", packet_buffer[0]);
 
 tmp_bl = TabletEncrypt[tmp_byte+packet_buffer[0]];
 tmp_dl = TabletEncrypt[tmp_byte+packet_buffer[0]+0x1000];
 
 tmp_byte += packet_buffer[0];
 
 //printf("BL -> %x DL -> %x\n", tmp_bl, tmp_dl);
 
 packet_buffer[4] = tmp_dl;
 //packet_encrypt[4] = tmp_dl;
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Encrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Encrypt *************");
 
 ShowPacketInHex(packet_buffer, size_packet);
 
 for(int i = 8; i < 12; i++)
 {
  tmp = packet_buffer[i-4];
  tmp ^= packet_buffer[i];
  //printf("%x ^ %x = %x\n", packet_buffer[i-4], packet_buffer[i], tmp);
  packet_buffer[i] = tmp;
 }
 
 for(int i = 8; i < size_packet; i++)
 {
  tmp = packet_buffer[i];
  tmp ^= packet_buffer[i+4];
  //printf("%x ^ %x = %x\n", packet_buffer[i], packet_buffer[i+4], tmp);
  packet_buffer[i+4] = tmp;
 }
 
 packet_buffer[4] ^= tmp_bl;
 packet_buffer[5] ^= 0x00;
 packet_buffer[6] ^= 0x00;
 packet_buffer[7] ^= 0x00;
 
 /*
 packet_encrypt[4] = packet_buffer[4]; 
 packet_encrypt[5] = packet_buffer[5]; 
 packet_encrypt[6] = packet_buffer[6]; 
 packet_encrypt[7] = packet_buffer[7];
 */
 
 /*for(int i = 8; i < size_packet+4; i++)
 {
  packet_encrypt[i] = packet_buffer[i-4];
 }*/
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("************ Decrypt ************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("************ Decrypt *************");
  
 ShowPacketInHex(packet_buffer, size_packet+4);
 
 if(RECV_DEBUG_MODE_TYPE() == 1)
  printf("*********************************\n");
 else if(RECV_DEBUG_MODE_TYPE() == 2)
  AddStringToListBoxDebug("*********************************");
}
