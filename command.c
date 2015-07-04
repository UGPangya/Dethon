#include "command.h"

void CommandAccess(char *command)
{
 AddStringToListBoxDebug("EXECUTANDO COMMAND");
 
 if(!strcmp(command, "/visual_mode"))
 {
  if(GetStatusDialog() == FALSE)
  {
   OpenDialog();
   SetStatusDialog(TRUE);
   SetStatusConsole(FALSE);
   InfoServer("dialogbox aberto\n");
  }
  ExitThread(0);
 }
  
 if(!strcmp(command, "/reset"))
 {
  ReloadGame();
 }
  
 if(!strcmp(command, "/ready"))
 {
  ReadyRoom(0);
 }
  
 if(!strcmp(command, "/room"))
 {
  //create_room("Teste", 4, 18, BLUE_LAGOON, 120, "123654");
  //create_room("Teste", 2, 9, BLUE_WATER, 120, "");
  create_room("Teste", 2, 9, DEEP_INFERNO, 120, "12355");
 }
  
 if(!strcmp(command, "/enter_room_pass"))
 {
  int tmp;
  char password[32];
  ColorText("ID_ROOM -> ", 4);
  scanf("%d", &tmp);
  ColorText("PASSWORD -> ", 4);
  scanf("%s", password);
  enter_room(tmp, password);
 }
  
 if(!strcmp(command, "/enter_room"))
 {
  int tmp;
  char password[32];
  ColorText("ID_ROOM -> ", 4);
  scanf("%d", &tmp);
  enter_room(tmp, "");
 }
  
 if(!strcmp(command, "/close_socket"))
 {
  /*if(!GetStatusDialog())
  {*/
   InfoServer("socket fechado\n");
   InfoServer("thread dialogbox fechado\n");
   InfoServer("thread recvpacket fechado\n");
   
   SetStatusConsole(FALSE);
   SetStatusRecvPacket(FALSE);
   SetStatusDialog(FALSE);
   SetStatusSendTimePacket(FALSE);
    
   TerminedSocket();
   TermineteThreads();
  /*}
  else
  {
   AddStringToListBoxDebug("Feche o DialogBox para usar essa função");
  }*/
 }
  
 if(!strcmp(command, "/commands"))
  ShowCommand();
  
  /*
  if(!strcmp(command, "/dialog_close"))
  {
   TerminateThread(CThreadDialog, 0);
   InfoServer("dialogbox fechado\n");
  }
  */
  
 if(!strcmp(command, "/exit"))
 {  
  InfoServer("socket fechado\n");
  InfoServer("thread dialogbox fechado\n");
  InfoServer("thread recvpacket fechado\n");
  
  Sleep(1000);
  
  TerminedSocket();
  SetStatusConsole(FALSE);
  
  #ifdef _WIN32
   ExitProcess(0);
  #elif LINUX
   exit(0);
  #endif
 }
}
