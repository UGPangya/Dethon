#ifndef __CONNECT_H_
#define __CONNECT_H_

#include "library.h"

bool init_connect(GeralConfig *GC);
void SendPacket(char *buffer_send, unsigned int size);
void EncryptSendPacket(char *buffer_send, unsigned int size, int size_finally);
unsigned int DecryptRecvPacket(unsigned char *, unsigned char *buffer_recv, unsigned int size);
unsigned int RecvPacket(char *buffer_recv, unsigned int size);
void TerminedSocket();

#endif
