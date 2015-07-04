#ifndef __LIBRARY_H_
#define __LIBRARY_H_

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <time.h>
#include <openssl/md5.h>

#ifdef _WIN32
#include <winsock2.h>
#include <conio.h>
#include <tlhelp32.h>
#elif LINUX
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <curses.h>
#endif

#include "encrypt.h"
#include "decrypt.h"

#include "color.h"

#define IDD_DIALOG1 101
#define IDC_EDT1 1001
#define IDC_BTN1 1002
#define IDC_LST1 1003
#define IDC_STC1 1004
#define IDC_LST2 1005
#define IDC_EDIT3 1007

// *********************************** //

#define MAX_SERVER_CONFIG 6
#define MAX_SERVER_LOGIN_CONFIG 5

void DEBUG_MODE_TYPE(unsigned int TYPE);
unsigned int RECV_DEBUG_MODE_TYPE();

struct GeralConfig
{
 unsigned int AutoLogin;
 
 char Login[32];
 char Password[64];
 
 char Ip[20];
 unsigned int Port;
 
 unsigned char PASSMD5[MD5_DIGEST_LENGTH];
 unsigned char PASSMD5CHAR[32];
};

typedef struct ServerConfig
{
 char SERVERNAME[MAX_SERVER_CONFIG][32];
 char SERVERIP[MAX_SERVER_CONFIG][20];
 unsigned int SERVERPORT[MAX_SERVER_CONFIG];
 unsigned char OFFSETSERVER[MAX_SERVER_CONFIG];
 unsigned int server_login_channel_change;
 unsigned int server_game_change;
 unsigned char SUBSERVERNAME[MAX_SERVER_CONFIG][MAX_SERVER_LOGIN_CONFIG][32];
 unsigned int OFFSETSUBSERVER[MAX_SERVER_LOGIN_CONFIG][MAX_SERVER_LOGIN_CONFIG];
 
 char OffsetPrimary[8];
 char OffsetSecondary[8];
 
 unsigned char VERSION_PANGYA[7];
 unsigned int VERSION_OFFSET[4];
 unsigned int OFFSET_CHAR[4];
 unsigned GAME_ACCESS_OFFSET;
 unsigned PCT_PANGYA;
 unsigned SPEED_SEND_PACKET;
 
 unsigned int SELECT_SERVER;
 unsigned int SELECT_SUBSERVER;
 unsigned int RELOAD_GAME;
 
 int TYPE_SCRIPT;
 
 // MASTER_GAME_PLAYER
 char PLAYER_COMMON[4][32];
 int QUANTITY_PLAYER_WAIT;
 
 // NORMAL_GAME_PLAYER
 char PLAYER_MASTER[32];
 int COMMON_CONFIRMED[4];
 
 unsigned int P_ROOM_ID;
 char P_PASSWORD_ROOM[32];
 
 
 char ROOM_NAME[32];
 unsigned int NUMBER_PLAYER;
 unsigned int NUMBER_HOLE;
 unsigned int TYPE_MAP;
 unsigned int TIME_MIN;
 char ROOM_PASSWORD[32];
};

// **********************************************************

typedef void (__stdcall *uncompress_packet)(unsigned char* local_uncompress, int size_uncompress, unsigned char* recv_packet);
typedef unsigned char* (__stdcall *gameguard_init)(unsigned char* PacketInfo);
typedef void* (__stdcall *call_encrypt_map)(unsigned char* ByteEncrypt, unsigned char*PacketEncrypt);

// **********************************************************

typedef struct GlobalVariables
{
 int bytes_recv;
 char CInfo[1024];
 char PacketRecv[262140];
 char PacketSend[262140];
 unsigned char packet_decrypt[262140];
 
 uncompress_packet UnCompressPacket;
 gameguard_init    GameGuardCall;
 call_encrypt_map  CallEncryptMapPacket;
 
 char NickName[64];
 int bytes_recv_piece;
 
 char BytesEncryptMap[20];
};

unsigned int PacketStrLen(char *buffer, int size_buffer);

void MD5Convert(unsigned char *, unsigned char *, int);

void CopyString(char *, char *, int);
bool CopyStringPos(unsigned char*, unsigned char*, int, int, int);

bool CmpString(char*, char*, int);
bool CmpStringPos(unsigned char*, unsigned char*, int, int, int);

void FailedServer(char *);
void InfoServer(char *);
void AttackInfoServer(char *);
void ServerCommand(char *);
void ColorText(char *, int);
void InfoColor(char *, int);

void LerConfig(GeralConfig*, ServerConfig*, char*);

void Log();
void BarSpace();
void TextCentralize(char *);

void SpaceText(char *buffer);

void ShowPacketInHex(unsigned char *packet_buffer, unsigned int size);

BOOLEAN PacketAnalyser(int);

void AddStringToListBoxChat(char *buffer);
void AddStringToListBoxDebug(char *buffer);

BOOLEAN LoadingLibrary(GlobalVariables *);
void InitGameGuard(GlobalVariables *);
DWORD GetProcessID(const char*);

void SetStatusDialog(BOOLEAN SetByteStatus);
void SetStatusRecvPacket(BOOLEAN SetByteStatus);
void SetStatusConsole(BOOLEAN SetByteStatus);
void SetStatusSendTimePacket(BOOLEAN SetByteStatus);

BOOLEAN GetStatusDialog();
BOOLEAN GetStatusRecvPacket();
BOOLEAN GetStatusConsole();
BOOLEAN GetStatusSendTimePacket();

DWORD ThreadDialog(LPVOID);
DWORD ThreadRecvPacket(LPVOID);
DWORD ThreadConsole(LPVOID);

DWORD MasterPlayerGame(LPVOID);
DWORD CommomPlayerGame(LPVOID);

void OpenDialog();
void TermineteThreads();

void ShowCommand();

void currentDateTime();

#endif
