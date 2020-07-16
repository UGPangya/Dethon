#ifndef __LOGIN_SERVER_H_
#define __LOGIN_SERVER_H_

#include "library.h"
#include "connect.h"

void MakePacketLoginServer(struct GeralConfig *, struct GlobalVariables *);
void SendMakePacketLogin(struct GeralConfig *, struct GlobalVariables *);
void SendPacketChangeLogin(struct GeralConfig *, struct ServerConfig *, struct GlobalVariables *);
BOOLEAN SendServerLogin(struct GeralConfig *, struct ServerConfig *, struct GlobalVariables *);
void DesconectLogin(struct GlobalVariables *);

#endif
