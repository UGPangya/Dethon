#ifndef __LOGIN_SERVER_H_
#define __LOGIN_SERVER_H_

#include "library.h"
#include "connect.h"

void MakePacketLoginServer(GeralConfig *, GlobalVariables *);
void SendMakePacketLogin(GeralConfig *, GlobalVariables *);
void SendPacketChangeLogin(GeralConfig *, ServerConfig *, GlobalVariables *);
BOOLEAN SendServerLogin(GeralConfig *, ServerConfig *, GlobalVariables *);
void DesconectLogin(GlobalVariables *);

#endif
