#pragma once

void SetByteDecrypt(unsigned short int byte);
void ENCDecryptPacket(unsigned char*, unsigned char* packet_buffer, unsigned int size_packet);
void EMCClientDecryptPacketSend(/*unsigned char *packet_encrypt,*/ unsigned char* packet_buffer,
                                                                   unsigned int size_packet);
