#include "library.h"

BOOLEAN ThreadDialogOpen = FALSE;
BOOLEAN ThreadRecvPacketOpen = FALSE;
BOOLEAN ThreadConsoleOpen = FALSE;
BOOLEAN ThreadSendTimePacketOpen = FALSE;

unsigned int DEBUG_MODE;

static char buf_time[80];

unsigned int PacketStrLen(char *buffer, int size_buffer)
{
 int tmp;
 for(int i = 0; i < sizeof(size_buffer); i++)
 {
  if(buffer[i] == 0x00)
   break;
  tmp++;
 }
 return tmp;
}

void DEBUG_MODE_TYPE(unsigned int TYPE)
{
 DEBUG_MODE = TYPE;
}

unsigned int RECV_DEBUG_MODE_TYPE()
{
 return DEBUG_MODE;
}

void MD5Convert(unsigned char *BufferMD5, unsigned char *DstMD5, int size)
{
 int tmp_i;
 char tmp_c[2];
 int counter_DstMD5 = 0;
 for(int i = 0; i < size; i++)
 {
  tmp_i = BufferMD5[i];
  //itoa(tmp_i, tmp_c, 16);
  sprintf(tmp_c, "%02X", tmp_i);
  DstMD5[counter_DstMD5] = tmp_c[0];
  counter_DstMD5++;
  DstMD5[counter_DstMD5] = tmp_c[1];
  counter_DstMD5++;
 }
}

void CopyString(char *string1, char*string2, int size)
{
 for(int i = 0; i < size; i++)
 {
  string1[i] = string2[i];
 }
}

bool CopyStringPos(unsigned char *string1, unsigned char *string2, int size, int pos1, int pos2)
{
 for(int i = 0; i < size; i++)
 {
  string1[pos1] = string2[pos2];
  pos1++;
  pos2++;
 }
}

bool CmpString(char*string1, char*string2, int size)
{
 for(int i = 0; i < size; i++)
 {
  if(string1[i] != string2[i])
   return 0;
 }
 return 1;
}

bool CmpStringPos(unsigned char *string1, unsigned char *string2, int size, int pos1, int pos2)
{
 for(int i = 0; i < size; i++)
 {
  if(string1[pos1] != string2[pos2])
   return 0;
   
  pos1++;
  pos2++;
 }
 
 return 1;
}

void FailedServer(char *str_err)
{
  currentDateTime();
  setcolor(0);
  printf("[%s] ", buf_time);
  setcolor(1);
  printf("[ERROR] ");
  restcolor();
  printf("%s", str_err);
}

void InfoServer(char *str_info)
{
 currentDateTime();
 setcolor(0);
 printf("[%s] ", buf_time);
 setcolor(3);
 printf("[INFO] ");
 restcolor();
 printf("%s", str_info);
}

void AttackInfoServer(char *str_info)
{
 currentDateTime();
 setcolor(0);
 printf("[%s] ", buf_time);
 setcolor(3);
 printf("[INFO] ");
 setcolor(2);
 printf("%s", str_info);
 restcolor();
}

void ColorText(char *str_color, int color)
{
 currentDateTime();
 setcolor(0);
 printf("[%s] ", buf_time);
 setcolor(color);
 printf("%s", str_color);
 restcolor();
}

void InfoColor(char *str_info, int color)
{
 currentDateTime();
 setcolor(0);
 printf("[%s] ", buf_time);
 setcolor(3);
 printf("[INFO] ");
 setcolor(color);
 printf("%s", str_info);
 restcolor();
}

void LerConfig(GeralConfig *GC, ServerConfig *SC, char *file)
{
 char PATH_FILE[FILENAME_MAX];
 char PATH_FILE_TMP[FILENAME_MAX];
 char PATH_FILE_TMP_SERVER[FILENAME_MAX];
 char PATH_FILE_TMP_MASTER[FILENAME_MAX];
 
 char text_sprintf[1024];
 
 GetModuleFileName(NULL, PATH_FILE, FILENAME_MAX);
 
 int i = strlen(PATH_FILE);
 
 for(i; i > 0; i--)
 {
  if(PATH_FILE[i] == '\\')
  {
   break;
  }
 }
 
 strncpy(PATH_FILE_TMP, PATH_FILE, i+1);
 PATH_FILE_TMP[i+1] = '\0';
 
 strncpy(PATH_FILE_TMP_SERVER, PATH_FILE, i+1);
 PATH_FILE_TMP_SERVER[i+1] = '\0';
 
 strncpy(PATH_FILE_TMP_MASTER, PATH_FILE, i+1);
 PATH_FILE_TMP_MASTER[i+1] = '\0';
 
 strcat(PATH_FILE_TMP, file);
 strcat(PATH_FILE_TMP_SERVER, "configserver.ini");
 strcat(PATH_FILE_TMP_MASTER, "script.ini");
 
 GetPrivateProfileString("GAME_UPDATED", "VERSION_CLIENT", "000.00", (char*)SC->VERSION_PANGYA, sizeof(SC->VERSION_PANGYA), PATH_FILE_TMP_MASTER);
 SC->VERSION_OFFSET[0] = GetPrivateProfileInt("GAME_UPDATED", "VERSION_OFFSET0", 0, PATH_FILE_TMP_MASTER);
 SC->VERSION_OFFSET[1] = GetPrivateProfileInt("GAME_UPDATED", "VERSION_OFFSET1", 0, PATH_FILE_TMP_MASTER);
 SC->VERSION_OFFSET[2] = GetPrivateProfileInt("GAME_UPDATED", "VERSION_OFFSET2", 0, PATH_FILE_TMP_MASTER);
 SC->VERSION_OFFSET[3] = GetPrivateProfileInt("GAME_UPDATED", "VERSION_OFFSET3", 0, PATH_FILE_TMP_MASTER);
 
 SC->GAME_ACCESS_OFFSET = GetPrivateProfileInt("GAME_UPDATED", "OFFSET_PACKET_ACCESS", 0, PATH_FILE_TMP_MASTER);
 
 SC->PCT_PANGYA = GetPrivateProfileInt("CONFIG_GAME_TYPE", "PCT_PANGYA", 0, PATH_FILE_TMP_MASTER);
 
 SC->SPEED_SEND_PACKET = GetPrivateProfileInt("CONFIG_GAME_TYPE", "SPEED_SEND_PACKET", 0, PATH_FILE_TMP_MASTER);
 
 SC->COMMON_CONFIRMED[0] = 0x00;
 SC->COMMON_CONFIRMED[1] = 0x00;
 SC->COMMON_CONFIRMED[2] = 0x00;
  
 SC->TYPE_SCRIPT = GetPrivateProfileInt("DethonBot", "TYPE_SCRIPT", 3, PATH_FILE_TMP);
 
 SC->QUANTITY_PLAYER_WAIT = GetPrivateProfileInt("MASTER_GAME_NORMAL", "QUANTITY_PLAYER_WAIT", 1, PATH_FILE_TMP_MASTER);
 GetPrivateProfileString("MASTER_GAME_NORMAL", "PLAYER_COMMON00", "NO_EXIST", (char*)SC->PLAYER_COMMON[0], sizeof(SC->PLAYER_COMMON[0]), PATH_FILE_TMP_MASTER);
 GetPrivateProfileString("MASTER_GAME_NORMAL", "PLAYER_COMMON01", "NO_EXIST", (char*)SC->PLAYER_COMMON[1], sizeof(SC->PLAYER_COMMON[1]), PATH_FILE_TMP_MASTER);
 GetPrivateProfileString("MASTER_GAME_NORMAL", "PLAYER_COMMON02", "NO_EXIST", (char*)SC->PLAYER_COMMON[2], sizeof(SC->PLAYER_COMMON[2]), PATH_FILE_TMP_MASTER);
 
 GetPrivateProfileString("MASTER_GAME_NORMAL", "ROOM_NAME", "TESTE", (char*)SC->ROOM_NAME, sizeof(SC->ROOM_NAME), PATH_FILE_TMP_MASTER);
 GetPrivateProfileString("MASTER_GAME_NORMAL", "ROOM_PASSWORD", "123", (char*)SC->ROOM_PASSWORD, sizeof(SC->ROOM_PASSWORD), PATH_FILE_TMP_MASTER);
 
 GetPrivateProfileString("PLAYER_GAME_NORMAL", "PM_PLAYER_MASTER", "MASTER", (char*)SC->PLAYER_MASTER, sizeof(SC->PLAYER_MASTER), PATH_FILE_TMP_MASTER);
  
  
 SC->NUMBER_PLAYER = GetPrivateProfileInt("MASTER_GAME_NORMAL", "NUMBER_PLAYER", 4, PATH_FILE_TMP_MASTER);
 SC->NUMBER_HOLE = GetPrivateProfileInt("MASTER_GAME_NORMAL", "NUMBER_HOLE", 18, PATH_FILE_TMP_MASTER);
 SC->TYPE_MAP = GetPrivateProfileInt("MASTER_GAME_NORMAL", "TYPE_MAP", 0, PATH_FILE_TMP_MASTER);
 SC->TIME_MIN  = GetPrivateProfileInt("MASTER_GAME_NORMAL", "TIME_MIN", 120, PATH_FILE_TMP_MASTER);
 
 SC->OFFSET_CHAR[0] = 0x00;
 SC->OFFSET_CHAR[1] = 0x00;
 SC->OFFSET_CHAR[2] = 0x00;
 SC->OFFSET_CHAR[3] = 0x00;
 
 GetPrivateProfileString("MASTER_GAME_NORMAL", "PLAYER_COMMON02", "NO_EXIST", (char*)SC->PLAYER_COMMON[2], sizeof(SC->PLAYER_COMMON[2]), PATH_FILE_TMP_MASTER);
 
 GC->AutoLogin = GetPrivateProfileInt("DethonBot", "AUTOLOGIN", -1, PATH_FILE_TMP);
 if(GC->AutoLogin == -1)
 {
  sprintf(text_sprintf, "impossivel ler [%s]\n", file);
  InfoServer(text_sprintf);
  GC->AutoLogin = 0;
 }
 
 char tmp[127];
 char subtmp[127];
 
 /*SC->RELOAD_GAME = GetPrivateProfileInt("DethonBot", "RELOAD_GAME", 1, PATH_FILE_TMP);*/
 
 for(int i = 0; i < MAX_SERVER_CONFIG-1; i++)
 {
  sprintf(tmp, "SERVERNAME%02d", i);
  GetPrivateProfileString("ConfigServer", tmp, "NO_SERVER", SC->SERVERNAME[i], sizeof(SC->SERVERNAME[i]), PATH_FILE_TMP_SERVER);
  sprintf(tmp, "SERVERIP%02d", i);
  GetPrivateProfileString("ConfigServer", tmp, "127.0.0.1", SC->SERVERIP[i], sizeof(SC->SERVERIP[i]), PATH_FILE_TMP_SERVER);
  sprintf(tmp, "OFFSETSERVER%02d", i);
  SC->OFFSETSERVER[i] = GetPrivateProfileInt("ConfigServer", tmp, 0, PATH_FILE_TMP_SERVER);
  sprintf(tmp, "SERVERPORT%02d", i);
  SC->SERVERPORT[i] = GetPrivateProfileInt("ConfigServer", tmp, 0, PATH_FILE_TMP_SERVER);
 } 
  
 for(int x = 0; x < MAX_SERVER_CONFIG-1; x++)
 {
  sprintf(tmp, "SERVER%02d", x);
  for(int y = 0; y < MAX_SERVER_LOGIN_CONFIG-1; y++)
  {
   sprintf(subtmp, "SUBSERVERNAME%d", y);
   GetPrivateProfileString(tmp, subtmp, "NO_SERVER", (char*)SC->SUBSERVERNAME[x][y], sizeof(SC->SUBSERVERNAME[x][y]), PATH_FILE_TMP_SERVER);
   sprintf(subtmp, "OFFSETSUBSERVER%d", y);
   SC->OFFSETSUBSERVER[x][y] = GetPrivateProfileInt(tmp, subtmp, 0, PATH_FILE_TMP_SERVER);
  }
 }
 
 SC->SELECT_SERVER = GetPrivateProfileInt("DethonBot", "SELECT_SERVER", 0, PATH_FILE_TMP);
 SC->SELECT_SUBSERVER = GetPrivateProfileInt("DethonBot", "SELECT_SUBSERVER", 0, PATH_FILE_TMP);
 
 GetPrivateProfileString("DethonBot", "LOGIN", "user", GC->Login, sizeof(GC->Login), PATH_FILE_TMP);
 GetPrivateProfileString("DethonBot", "SENHA", "password", GC->Password, sizeof(GC->Password), PATH_FILE_TMP);
 
 GetPrivateProfileString("DethonBot", "IP", "127.0.0.1", GC->Ip, sizeof(GC->Ip), PATH_FILE_TMP);
 GC->Port = GetPrivateProfileInt("DethonBot", "PORT", 10103, PATH_FILE_TMP);
 
 sprintf(text_sprintf, "AUTOLOGIN   = %d\n", GC->AutoLogin);
 AttackInfoServer(text_sprintf);

 
 if(GC->AutoLogin == 1)
 {
  sprintf(text_sprintf, "LOGIN      = %s\n", GC->Login);
  AttackInfoServer(text_sprintf);
  sprintf(text_sprintf, "SENHA      = %s\n", GC->Password);
  AttackInfoServer(text_sprintf);
 }
 sprintf(text_sprintf, "IP          = %s\n", GC->Ip);
 AttackInfoServer(text_sprintf);
 sprintf(text_sprintf, "PORTA       = %d\n", GC->Port);
 AttackInfoServer(text_sprintf);
 
 sprintf(text_sprintf, "VERSION     = %s\n", SC->VERSION_PANGYA);
 AttackInfoServer(text_sprintf);
 
 /*sprintf(text_sprintf, "RELOAD TIME = %d MINUTOS\n", SC->RELOAD_GAME);
 AttackInfoServer(text_sprintf);*/
 
 BarSpace();
}

void Log()
{
 char text[80];
 BarSpace();
 TextCentralize("DethonBot By Firefox");
 BarSpace();
 sprintf(text, "Pangya Dethon");
 TextCentralize(text);
 BarSpace();
}

void BarSpace()
{
 setcolor(1);
 printf("*******************************************************************************\n");
 restcolor();
}

void TextCentralize(char *str)
{
 setcolor(2);
 int pos = (int)(80-strlen(str))/2;
 for(int i = 0; i < pos; i++)
 {
  printf(" ");
 }
 printf("%s\n", str);
 restcolor();
}

void SpaceText(char *buffer)
{
 for(int i = 0; i < strlen(buffer); i++)
 {
  if(buffer[i] == '_')
  {
   buffer[i] = 0x20;
  }
 }
}

void ShowPacketInHex(unsigned char *packet_buffer, unsigned int size)
{
 char DEBUG_MODE_BOX[1024];
 char CONVERT_TO_CHAR[4];
 
 int COLUM_TYPE;
 
 if(DEBUG_MODE == 1)
  COLUM_TYPE = 16;
 else if(DEBUG_MODE == 2)
  COLUM_TYPE = 23;
  
 int TMP_DEBUGMOD = 0;
 int colum = 0;
 
 for(int i = 0; i < size; i++)
 {
  colum++;
   
  if(DEBUG_MODE == 1)
  {
   printf("\\x%02x", (unsigned char)packet_buffer[i]);
  }
  else if(DEBUG_MODE == 2)
  {
   sprintf(CONVERT_TO_CHAR, "%02X", (unsigned char)packet_buffer[i]);
   DEBUG_MODE_BOX[TMP_DEBUGMOD] = CONVERT_TO_CHAR[0];
   TMP_DEBUGMOD++;
   DEBUG_MODE_BOX[TMP_DEBUGMOD] = CONVERT_TO_CHAR[1];
   TMP_DEBUGMOD++;
   DEBUG_MODE_BOX[TMP_DEBUGMOD] = ' ';
   TMP_DEBUGMOD++;
  }
   
  if(colum == COLUM_TYPE || i >= size-1)
  {
   if(DEBUG_MODE == 1)
    printf("\n");
   else if(DEBUG_MODE == 2)
   {
    DEBUG_MODE_BOX[TMP_DEBUGMOD] = 0x00;
    AddStringToListBoxDebug(DEBUG_MODE_BOX);
    TMP_DEBUGMOD = 0;
   }
   colum = 0;
  }
 }
}

BOOLEAN LoadingLibrary(GlobalVariables *GV)
{
 if(HINSTANCE uncompressDLL = LoadLibrary("uncompress.dll"))
 {
  GV->UnCompressPacket = (uncompress_packet)GetProcAddress(uncompressDLL, "call_uncompress");
  GV->CallEncryptMapPacket = (call_encrypt_map)GetProcAddress(uncompressDLL, "call_encrypt");
  InfoServer("DLL uncompress.dll carregado com sucesso!\n");
 }
 else
 {
  FailedServer("DLL uncompress.dll inexistente!\n");
  return 0;
 }
    
 if(HINSTANCE gameguardDLL = LoadLibrary("gameguard.dll"))
 {
  GV->GameGuardCall = (gameguard_init)GetProcAddress(gameguardDLL, "gameguard_init");
  InfoServer("DLL gameguard.dll carregado com sucesso!\n");
 }
 else
 {
  FailedServer("DLL gameguard.dll inexistente!\n");
  return 0;
 }
}
 
/*
char LocateGameDes[] = "C:\\SG Interactive\\Pangya\\GameGuard.des";
char LocateGameMon[] = "C:\\SG Interactive\\Pangya\\GameGuard\\GameMon.des";


unsigned char *CommandLine;
unsigned char *StringCommandLine[249];
char PacketInfo[32];

STARTUPINFO StartUpInfo;
PROCESS_INFORMATION ProcessInformation;

STARTUPINFO StartUpInfoDes;
PROCESS_INFORMATION ProcessInformationDes;

char npgmup[] = "npgmup";
char npsc[] = "npsc";
char npgm[] = "npgm";
char npgg[] = "npgg";
char npgl[] = "npgl";

SECURITY_ATTRIBUTES SA;

void InitGameGuard(GlobalVariables *GV)
{
 int pID;
 pID = GetProcessID("dethon.exe");
 HANDLE EventOne = 0x00;
 HANDLE EventTwo = 0x00;
 HANDLE EventTree = 0x00;
 
 HANDLE ReadPipeLineOne;
 HANDLE WritePipeLineOne;
 
 HANDLE ReadPipeLineTwo;
 HANDLE WritePipeLineTwo;
 
 //0013FDCC  0C 00 00 00 00 00 00 00 01 00 00 00              ...........

 int size;
 int Init = 0x100000;
 
 if(Init > 0)
  size = 40;
 else
 size = 35;

 SA.nLength = sizeof(SECURITY_ATTRIBUTES);
 SA.bInheritHandle = 1;
 
 EventOne = CreateEvent(&SA, FALSE, FALSE, NULL);
 EventTwo = CreateEvent(&SA, FALSE, TRUE, NULL);
 
 EventTree = CreateEvent(&SA, FALSE, FALSE, NULL);
 
 sprintf(PacketInfo, " %d PangyaUS %x %x %x 0 70718 ", Init, EventOne, EventTwo, pID);
 printf("PacketInfo [0x%x]\n%s\n", &PacketInfo, PacketInfo);
 
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)npgl); 
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)npgg); 
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)npgm); 
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)npsc); 
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)npgmup); 
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)PacketInfo); 
 
 int tmp = 0;
 for(int i = 0; i < 300; i++)
 {
  if((CommandLine[i] == 0x00) && (CommandLine[i+1] == 0x00) && (CommandLine[i+2] == 0x00))
   break;
  sprintf((char*)StringCommandLine+tmp, "\\x%02x", CommandLine[i]);
  tmp += 4;
 }
 
    
 printf("GameGuard CommandLine\n%s\n", StringCommandLine);
 
 StartUpInfoDes.cb = 0x44;
 int t = CreateProcessA(LocateGameDes, (char*)StringCommandLine, 0, 0, 1, 0, 0, 0, &StartUpInfoDes, &ProcessInformationDes);
 printf("\nFOI ? [%d]\n", t);
 
 Sleep(0x2000);
 //CloseHandle(ProcessInformationDes.hProcess);
 
 HANDLE hTargetProcessHandleOne = (void*)-1;
 HANDLE CurrentProcess = GetCurrentProcess();
  
 CreatePipe(&ReadPipeLineOne, &WritePipeLineOne, &SA, 0);
 SetStdHandle(STD_OUTPUT_HANDLE, &WritePipeLineOne);
 DuplicateHandle(CurrentProcess, &ReadPipeLineOne, CurrentProcess, &hTargetProcessHandleOne, 0, FALSE, DUPLICATE_SAME_ACCESS);
 CloseHandle(&ReadPipeLineOne);
 
 
 HANDLE hTargetProcessHandleTwo = (void*)-1;
 CreatePipe(&ReadPipeLineTwo, &WritePipeLineTwo, &SA, 0);
 SetStdHandle(STD_OUTPUT_HANDLE, &ReadPipeLineTwo);
 CurrentProcess = GetCurrentProcess();
 DuplicateHandle(CurrentProcess, &WritePipeLineTwo, CurrentProcess, &hTargetProcessHandleTwo, 0, FALSE, DUPLICATE_SAME_ACCESS);
 CloseHandle(&WritePipeLineTwo);
 
 ResetEvent(&EventOne);
 SetThreadPriority(NULL, THREAD_PRIORITY_HIGHEST);
 
 
 sprintf(PacketInfo, " 0 %d 1 %x %x PangyaUS %x %x 2 70718", pID, ReadPipeLineOne, WritePipeLineOne, EventTree, EventOne);
 printf("PacketInfo [0x%x]\n%s\n", &PacketInfo, PacketInfo);
 CommandLine = (unsigned char*)GV->GameGuardCall((unsigned char*)PacketInfo); 
 tmp = 0;
 for(int i = 0; i < 300; i++)
 {
  if((CommandLine[i] == 0x00) && (CommandLine[i+1] == 0x00) && (CommandLine[i+2] == 0x00))
   break;
  sprintf((char*)StringCommandLine+tmp, "\\x%02x", CommandLine[i]);
  tmp += 4;
 }
 
 printf("GameGuard CommandLine\n%s\n", StringCommandLine);
   
 StartUpInfo.cb = 0x44;
 StartUpInfo.lpReserved = 0x00;
 StartUpInfo.lpDesktop = 0x00;
 
 printf("CreateProcessA\n");
 t = CreateProcessA(LocateGameMon, (char*)StringCommandLine, 0, 0, 0, 4, 0, 0, &StartUpInfo, &ProcessInformation);
 printf("\nFOI ? [%d]\n", t);
 printf("GameDes Iniciado\n");
 //WaitForSingleObject(ProcessInformation.hThread, (DWORD)9999);
 ResumeThread(ProcessInformation.hThread);
 SetEvent(&EventTwo);
}

DWORD GetProcessID(const char* szExeName)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(Process32First(hSnapshot, &pe))
		while(Process32Next(hSnapshot, &pe))
			if(!strcmp(pe.szExeFile, szExeName))
				return pe.th32ProcessID;

	return 0;
}*/

void SetStatusDialog(BOOLEAN SetByteStatus) {ThreadDialogOpen = SetByteStatus;}
void SetStatusRecvPacket(BOOLEAN SetByteStatus) {ThreadRecvPacketOpen = SetByteStatus;}
void SetStatusConsole(BOOLEAN SetByteStatus) {ThreadConsoleOpen = SetByteStatus;}
void SetStatusSendTimePacket(BOOLEAN SetByteStatus) {ThreadSendTimePacketOpen = SetByteStatus;}

BOOLEAN GetStatusDialog() {return ThreadDialogOpen;}
BOOLEAN GetStatusRecvPacket() {return ThreadRecvPacketOpen;}
BOOLEAN GetStatusConsole() {return ThreadConsoleOpen;}
BOOLEAN GetStatusSendTimePacket() {return ThreadSendTimePacketOpen;}

void ShowCommand()
{
 if(GetStatusDialog())
 {
  BarSpace();
  ColorText("/visual_mode (modo visual)\n", 2);
  ColorText("/close_socket (desconecta do servidor)\n", 2);
  ColorText("/commands (exibe lista de comando)\n", 2);
  ColorText("/exit (fecha o programa)\n", 2);
  BarSpace();
 }
 else
 {
  AddStringToListBoxDebug("/visual_mode (modo visual)");
  AddStringToListBoxDebug("/close_socket (desconecta do servidor)");
  AddStringToListBoxDebug("/commands (exibe lista de comando)");
  AddStringToListBoxDebug("/exit (fecha o programa)");
 }
}

void currentDateTime()
{
 time_t     now = time(0);
 tm  tstruct;
 tstruct = *localtime(&now);
 strftime(buf_time, sizeof(buf_time), "%Y-%m-%d.%X", &tstruct);
}
