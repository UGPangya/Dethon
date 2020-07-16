#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>

#include "library.h"
#include "connect.h"
#include "login_server.h"
#include "game_server.h"
#include "chat.h"
#include "command.h"
#include "decrypt.h"
#include "encrypt.h"
#include "room.h"
#include "map.h"
#include "practice.h"

// Edit Version .ini
//Packet Send Select Server

/*
03B6FD1C  00 02 00 08 00 54 72 65 65 54 68 6F 72 B7 FC 10  ...TreeThor��
03B6FD2C  00 00 00 00 00 96 66 07 00 31 30 30 34 33 39 64  .....�f.100439d
03B6FD3C  06 00 37 32 31 2E 30 30 [DE 87 AD 2A] 00 00 00 00  .721.00އ�*....
03B6FD4C  07 00 31 35 64 64 35 66 65 00 00 00              .15dd5fe...
[DE 87 AD 2A] -> Version Game
*/


// **********************************************************
// **********************************************************
// ********************* Packet Ref *************************
// **********************************************************
// **********************************************************

// Verificador de Login ou Senha incorreto.
//                                                                          Incorrect login credentials          
// [5c 2d 00 d0 00 00 00 24 35 01 00 e2 61 d2 4d 00 1b 00] (496e636f7272656374206c6f67696e2063726564656e7469616c7311) 0000
unsigned char login_failed[] = "Incorrect login credentials";

// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************
// **********************************************************

struct GeralConfig GC;
struct ServerConfig SC;
struct GlobalVariables GV;

DWORD* PThread;

#ifdef _WIN32
HWND CThreadDialog = NULL;
HWND CThreadRecvPacket = NULL;
HWND CThreadConsole = NULL;
HWND CThreadSendTimePacket = NULL;

HWND CThreadCommonPlayer = NULL;
HWND CThreadMasterPlayer = NULL;
HWND CThreadPractice = NULL;

#elif LINUX
pthread_t CThreadDialog;
pthread_t CThreadRecvPacket;
pthread_t CThreadConsole;
pthread_t CThreadSendTimePacket;
pthread_t CThreadPractice;
#endif

HWND hWndDialog = NULL;
HWND ListBoxDebug = NULL;
HWND ListBoxChat = NULL;
HWND EditBoxMsg = NULL;
HWND EditBoxPlayer = NULL;

int counter_listChat = 0;
int counter_listDebug = 0;

int _tmp = 0;

BOOL CALLBACK GoToProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

void MessageArg()
{
	printf("Execute o dethon com o seguinte parametro\ndethon.exe + arquivo.ini\n exemplo dethon.exe config.ini");
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	SetConsoleTitle("DethonBot");
	Log();
	InfoServer("aperte enter para continuar!\n");
	_getch();

	LoadingLibrary(&GV);
	/*InitGameGuard(&GV);*/

	if (argc < 2)
		MessageArg();
	else
	{
		BarSpace();
		sprintf_s(GV.CInfo, _countof(GV.CInfo), "carregando a ini [%s]\n", argv[1]);
		InfoServer(GV.CInfo);

		while (true)
		{
			GameCopyStructInfo(&GC, &SC, &GV);
			RoomCopyStructInfo(&GC, &SC, &GV);
			MapCopyStructInfo(&GC, &SC, &GV);
			PracticeCopyStructInfo(&GC, &SC, &GV);

			SetLimitByteRecvPacket(512);

			reset_keys();
			ResetRoom();

			counter_listDebug = 0;
			counter_listChat = 0;

			//DEBUG_MODE_TYPE(1);
			BarSpace();
			LerConfig(&GC, &SC, argv[1]);

			MakePacketLoginServer(&GC, &GV);

			if (init_connect(&GC) == 1)
			{
				InfoServer("server online - conectado com sucesso!\n");

				// RECV PACKE
				GV.bytes_recv = RecvPacket(GV.PacketRecv, 14);

				if (GV.bytes_recv != -1 && GV.bytes_recv != 0)
				{
					sprintf_s(GV.CInfo, _countof(GV.CInfo), "SetByte | Encrypt / Decrypt [%d]\n", GV.PacketRecv[6]);
					InfoServer(GV.CInfo);

					// SET BYTE KEY (ENCRYPT/DECRYPT) LOGIN_SERVER
					SetByteDecrypt(GV.PacketRecv[6]);
					SetByteEncrypt(GV.PacketRecv[6]);

					SendMakePacketLogin(&GC, &GV);

					// RECV PACKET
					//DEBUG_MODE_TYPE(1);
					GV.bytes_recv = DecryptRecvPacket(GV.packet_decrypt, (unsigned char*)GV.PacketRecv,
					                                  sizeof(GV.PacketRecv) - 1);
					//DEBUG_MODE_TYPE(0);

					if (GV.bytes_recv == -1 || GV.bytes_recv == 0)
					{
						FailedServer("servidor desconectado!\n");
						continue;
					}

					/*if(GV.PacketRecv[14] == 0x4D)
					{
					 FailedServer("conta banida!\n");
					 continue;
					}*/

					if (GV.packet_decrypt[1] == 0x2E && GV.packet_decrypt[8] == 0x36 && GV.packet_decrypt[9] == 0x01)
						InfoServer("ocorreu um erro tente novamente!\n");
					else
					{
						if (GV.packet_decrypt[1] == 0x12 && GV.packet_decrypt[8] == 0x1a && GV.packet_decrypt[9] == 0x01
						)
						{
							InfoServer("conta ja esta logada!\n");
							DesconectLogin(&GV); // DESCONECTA O LOGIN CASO ELE ESTEJA CONECTADO
						}

						CopyStringPos((unsigned char*)SC.OffsetPrimary, (unsigned char*)GV.packet_decrypt,
						              sizeof(SC.OffsetPrimary) - 1, 0, 13);

						if (!CmpStringPos((unsigned char*)GV.packet_decrypt, (unsigned char*)login_failed,
						                  sizeof(login_failed) - 1, 18, 0))
						{
							sprintf_s(GV.CInfo, _countof(GV.CInfo), "set_offset_primary [%s]\n", SC.OffsetPrimary);
							InfoServer(GV.CInfo);

							GameDecryptFunc(&GV); // Multi-Packet and Decrypt Mult Packet
							SendPacketChangeLogin(&GC, &SC, &GV); // CONSTRUI PACKET DE LOGIN

							// RECV PACKET
							GV.bytes_recv = DecryptRecvPacket(GV.packet_decrypt, (unsigned char*)GV.PacketRecv,
							                                  sizeof(GV.PacketRecv) - 1);
							if (GV.bytes_recv == -1 || GV.bytes_recv == 0)
							{
								FailedServer("servidor desconectado!\n");
								continue;
							}

							CopyStringPos((unsigned char*)SC.OffsetSecondary, (unsigned char*)GV.packet_decrypt,
							              sizeof(SC.OffsetSecondary) - 1, 0, 17);
							sprintf_s(GV.CInfo, _countof(GV.CInfo), "set_offset_secondary [%s]\n", SC.OffsetSecondary);
							InfoServer(GV.CInfo);

							// NEW CONNECT
							CopyString(GC.Ip, SC.SERVERIP[SC.server_game_change], 20);
							GC.Port = SC.SERVERPORT[SC.server_game_change];

							TerminedSocket();

							sprintf_s(GV.CInfo, _countof(GV.CInfo), "[%s] connect [%s]:[%d]\n",
							          SC.SERVERNAME[SC.server_game_change], GC.Ip, GC.Port);
							InfoServer(GV.CInfo);

							if (init_connect(&GC) == 1)
							{
								InfoServer("server conectado com sucesso [2]\n");

								// RECV PACKET
								GV.bytes_recv = DecryptRecvPacket(GV.packet_decrypt, (unsigned char*)GV.PacketRecv,
								                                  sizeof(GV.PacketRecv) - 1);
								if (GV.bytes_recv == -1 || GV.bytes_recv == 0)
								{
									FailedServer("servidor desconectado!\n");
									continue;
								}

								// SET BYTE KEY (ENCRYPT/DECRYPT) GAME SERVER
								SetByteDecrypt(GV.packet_decrypt[8]);
								SetByteEncrypt(GV.packet_decrypt[8]);

								sprintf_s(GV.CInfo, _countof(GV.CInfo), "SetByte | Encrypt / Decrypt [%d]\n",
								          GV.packet_decrypt[8]);
								InfoServer(GV.CInfo);

								if (SendServerLogin(&GC, &SC, &GV))
								{
									//DEBUG_MODE_TYPE(2);

									SetStatusRecvPacket(TRUE);
									SetStatusSendTimePacket(TRUE);
									SetStatusDialog(TRUE);

#ifdef _WIN32
									CThreadRecvPacket = (HWND)CreateThread(
										0, 0, (LPTHREAD_START_ROUTINE)ThreadRecvPacket, (LPVOID)0, 0, 0);
									CThreadSendTimePacket = (HWND)CreateThread(
										0, 0, (LPTHREAD_START_ROUTINE)ThreadSendTimePacket, (LPVOID)0, 0, PThread);
#elif LINUX
            pthread_create(&CThreadRecvPacket, NULL, &ThreadRecvPacket, NULL);
            pthread_create(&CThreadSendTimePacket, NULL, &ThreadSendTimePacket, NULL);
#endif

									if (SC.TYPE_SCRIPT == 0x03)
									{
#ifdef _WIN32
										CThreadDialog = (HWND)CreateThread(
											0, 0, (LPTHREAD_START_ROUTINE)ThreadDialog, (LPVOID)0, 0, PThread);
#elif LINUX
             pthread_create(&CThreadDialog, NULL, &ThreadDialog, NULL);
#endif
									}

									/*
									CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadSendPacketGameGuard, (LPVOID)0, 0, PThread);
									*/

									SetLimitByteRecvPacket(-1);

									Sleep(2000);

									if (GetStatusRecvPacket() == TRUE)
										SelectChannelLogin(&GC, &SC, &GV); // SELE��O DE CANAL DO SERVIDOR

									BarSpace();
									TextCentralize("DETHONBOT CARREGADO COM SUCESSO");
									BarSpace();

									if (SC.TYPE_SCRIPT == 0x01)
										CThreadMasterPlayer = (HWND)CreateThread(
											0, 0, (LPTHREAD_START_ROUTINE)MasterPlayerGame, (LPVOID)0, 0, PThread);
									else if (SC.TYPE_SCRIPT == 0x00)
										CThreadCommonPlayer = (HWND)CreateThread(
											0, 0, (LPTHREAD_START_ROUTINE)CommomPlayerGame, (LPVOID)0, 0, PThread);
									else if (SC.TYPE_SCRIPT == 0x02)
										CThreadPractice = (HWND)CreateThread(
											0, 0, (LPTHREAD_START_ROUTINE)InitPractice, (LPVOID)0, 0, PThread);
								}
							}
							/*else
							 FailedServer("server offline [2]\n");*/

							for (;;)
							{
								if ((GetStatusRecvPacket() == FALSE) && (GetStatusSendTimePacket() == FALSE))
									break;

								if (SC.TYPE_SCRIPT == 3)
								{
									if ((GetStatusDialog() == FALSE) && (GetStatusConsole() == FALSE) && (
										GetStatusRecvPacket() == TRUE))
									{
										SetStatusConsole(TRUE);
#ifdef _WIN32
										CThreadConsole = (HWND)CreateThread(
											0, 0, (LPTHREAD_START_ROUTINE)ThreadConsole, (LPVOID)0, 0, PThread);
#elif LINUX
              pthread_create(&CThreadConsole, NULL, &ThreadConsole, NULL);
#endif
									}
								}
								Sleep(100);
							}

							if (GetStatusConsole() == TRUE)
								printf("\n");

							TerminedSocket();

#ifdef _WIN32
							TerminateThread(CThreadDialog, 0);
							TerminateThread(CThreadRecvPacket, 0);
							TerminateThread(CThreadSendTimePacket, 0);
							TerminateThread(CThreadCommonPlayer, 0);
							TerminateThread(CThreadMasterPlayer, 0);
							TerminateThread(CThreadPractice, 0);
							TermineteThreadCheckShot();
#elif LINUX
           pthread_exit(CThreadDialog);
           pthread_exit(CThreadRecvPacket);
           pthread_exit(CThreadSendTimePacket);
           pthread_exit(CThreadCommonPlayer);
           pthread_exit(CThreadMasterPlayer);
           pthread_exit(CThreadPractice);
           TermineteThreadCheckShot();
#endif

							if ((SC.TYPE_SCRIPT == 0x00) || (SC.TYPE_SCRIPT == 0x05) || (SC.TYPE_SCRIPT == 0x06) || (SC.
								TYPE_SCRIPT == 0x07))
							{
								IDRoomConfirmed(0);
								SC.TYPE_SCRIPT = 0x00;
								SC.P_ROOM_ID = 0x00;
								StatusShotCounterCheck(0);
								InfoServer("PLAYER RESETADO\n");
							}

							if ((SC.TYPE_SCRIPT == 0x01) || (SC.TYPE_SCRIPT == 0x04))
							{
								SetPlayersConfirmed(0);
								for (_tmp = 0; _tmp < 3; _tmp++)
								{
									SC.COMMON_CONFIRMED[_tmp] = 0x00;
								}
								SC.TYPE_SCRIPT = 0x01; // Status OK
								InfoServer("MASTER RESETADO\n");
							}

							if ((SC.TYPE_SCRIPT = 0x02) || (SC.TYPE_SCRIPT = 0x08))
							{
								SC.TYPE_SCRIPT = 0x02;
							}

							SetConsoleTitle("DethonBot");
							FailedServer("server offline [3] - desconectado\n");
							Sleep(500);
						}
						else
						{
							FailedServer("login ou senha invalido\n");
							TerminedSocket();
						}
					}
				}
				else
					FailedServer("server offline [1]\n");
			}
			else
				FailedServer("server offline [0]\n");
		}
	}

	_getch();
	return EXIT_SUCCESS;
}

char text_tmp[1024];

#ifdef _WIN32
BOOL CALLBACK GoToProc(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		ListBoxChat = GetDlgItem(hWndDlg, IDC_LST1);
		ListBoxDebug = GetDlgItem(hWndDlg, IDC_LST2);
		EditBoxMsg = GetDlgItem(hWndDlg, IDC_EDT1);
		EditBoxPlayer = GetDlgItem(hWndDlg, IDC_EDIT3);
		return TRUE;

	case WM_COMMAND:
		if (wParam == IDC_BTN1)
		{
			char* szText;
			char* PlayerMsg;
			szText = (char*)GlobalAlloc(GPTR, 255);
			PlayerMsg = (char*)GlobalAlloc(GPTR, 255);
			memset(PlayerMsg, 0, 255);
			SendMessage(EditBoxMsg, WM_GETTEXT, 255, (LPARAM)szText);
			SendMessage(EditBoxPlayer, WM_GETTEXT, 255, (LPARAM)PlayerMsg);
			if (szText[0] != '/')
			{
				if (szText[0] != 0x00)
				{
					if (PlayerMsg[0] == 0x00)
						SendChatMsg(&GV, szText);
					else
						SendPMMsg(&GV, szText, PlayerMsg);
				}
			}
			else
				CommandAccess((char*)szText);

			GlobalFree((HGLOBAL)szText);
			GlobalFree((HGLOBAL)PlayerMsg);
			SendMessage(EditBoxMsg, WM_SETTEXT, 0, 0);

			/*sprintf(text_tmp, "text-----------\n-------"); 
			AddStringToListBoxDebug(text_tmp); 
			AddStringToListBoxChat(text_tmp);*/
		}

		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SetStatusDialog(FALSE);
			DestroyWindow(hWndDlg);
			return TRUE;
		}
	}
	return FALSE;
}
#endif

DWORD ThreadDialog(LPVOID param)
{
#ifdef _WIN32
	hWndDialog = (HWND)DialogBoxParam(GetModuleHandle(0), (LPCTSTR)MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)GoToProc,
	                                  0);
	ExitThread(0);
#elif LINUX
	// GTK LINUX
#endif
}


DWORD ThreadRecvPacket(LPVOID param)
{
	UNREFERENCED_PARAMETER(param);
	while (GetStatusRecvPacket())
	{
		GameDecryptFunc(&GV);
	}

	SetStatusRecvPacket(FALSE);
	SetStatusDialog(FALSE);
	SetStatusSendTimePacket(FALSE);
	return 0;
}

BOOLEAN PacketAnalyser(int size_packet)
{
	char buffer_tmp[10240] = {0};
	int tmp;

	if (((GV.packet_decrypt[8] == 0x14) && (GV.packet_decrypt[9] == 0x44)))
	{
		AddStringToListBoxDebug("client invalid version!");
		InfoColor("client invalid version!\n", 4);
		TerminedSocket();
		return 1;
	}

	/*if(((GV.packet_decrypt[9] == 0x5D) && (GV.packet_decrypt[11] == 0xBA)))
	 SetMyTurn(0); // Erro, n�o � meu turno
	*/

	if ((size_packet >= 10) && (size_packet <= 200) && (GV.packet_decrypt[GV.bytes_recv_piece - 3] == 0x11))
		GV.UnCompressPacket((unsigned char*)buffer_tmp, GV.bytes_recv_piece, GV.packet_decrypt + 4);

	/*
	if(size_packet <= 200)
	{
	 DEBUG_MODE_TYPE(1);
	 ShowPacketInHex((unsigned char*)buffer_tmp, size_packet);
	 DEBUG_MODE_TYPE(0);
	 printf("\n");
	}
	*/

	/*
	if(size_packet <= 200)
	{
	 DEBUG_MODE_TYPE(1);
	 ShowPacketInHex((unsigned char*)GV.packet_decrypt, size_packet);
	 DEBUG_MODE_TYPE(0);
	 printf("\n");
	}*/

	if ((buffer_tmp[0] == 0x01) && (buffer_tmp[1] == 0x00) && (buffer_tmp[2] == 0x00) && ((unsigned char)SC.OFFSET_CHAR[
		0] == (unsigned char)0x00))
	{
		if (buffer_tmp[4] == 0x00)
		{
			CopyString((char*)GV.packet_decrypt, (char*)buffer_tmp, GV.bytes_recv_piece);
			tmp = GV.packet_decrypt[3];
			SC.OFFSET_CHAR[0] = GV.packet_decrypt[tmp + 3 + 1 + 1];
			SC.OFFSET_CHAR[1] = GV.packet_decrypt[tmp + 3 + 1 + 2];
			SC.OFFSET_CHAR[2] = GV.packet_decrypt[tmp + 3 + 1 + 3];
			SC.OFFSET_CHAR[3] = GV.packet_decrypt[tmp + 3 + 1 + 4];
			printf("OFFSET CHAR [%02x] [%02x] [%02x] [%02x]\n", (unsigned char)SC.OFFSET_CHAR[0],
			       (unsigned char)SC.OFFSET_CHAR[1], (unsigned char)SC.OFFSET_CHAR[2],
			       (unsigned char)SC.OFFSET_CHAR[3]);
			GetNick(&GV);
		}
	}

	// CHAT NORMAL
	if ((buffer_tmp[0] == 0x40) && (buffer_tmp[1] == 0x00))
		RecvChatMsg(&GV, &SC);

	// PM
	if (((unsigned char)buffer_tmp[0] == 0x84) && (buffer_tmp[1] == 0x00))
		RecvChatMsg(&GV, &SC);

	if ((buffer_tmp[0] == 0x49) && (buffer_tmp[2] == 0x08))
	{
		AddStringToListBoxDebug("[ENTER ROOM] INEXISTENTE");
		InfoColor("[ENTER ROOM] INEXISTENTE\n", 4);
		/*SendPacketLoginConfirmation();*/
		if ((SC.TYPE_SCRIPT == 0x06) || (SC.TYPE_SCRIPT == 0x05))
		{
			IDRoomConfirmed(-1);
			SC.TYPE_SCRIPT = 0x00;
			SC.P_ROOM_ID = 0x00;
			CommandAccess("/close_socket");
		}
	}

	if ((buffer_tmp[0] == 0x49) && (buffer_tmp[2] == 0x04))
	{
		AddStringToListBoxDebug("[ENTER ROOM] PASSWORD INVALIDO");
		InfoColor("[ENTER ROOM] PASSWORD INVALIDO\n", 4);
		if ((SC.TYPE_SCRIPT == 0x06) || (SC.TYPE_SCRIPT == 0x05))
		{
			IDRoomConfirmed(-1);
			SC.TYPE_SCRIPT = 0x00;
			SC.P_ROOM_ID = 0x00;
			CommandAccess("/close_socket");
		}
	}

	if (((buffer_tmp[0] == 0x49) && (buffer_tmp[2] == 0x00)))
	{
		/*GV.UnCompressPacket((unsigned char*)buffer_tmp, GV.bytes_recv_piece, GV.packet_decrypt+4);*/

		for (tmp = 0; tmp < GV.bytes_recv_piece; tmp++)
		{
			if ((buffer_tmp[tmp] == 0x1e) && (buffer_tmp[tmp - 1] == 0x00))
				break;
		}


		if (buffer_tmp[tmp + 1] == 0x1e)
		{
			tmp += 1;
		}
		else
		{
			if (buffer_tmp[tmp] != 0x1e)
				CommandAccess("/close_socket");
		}

		printf("%x %x", (unsigned char)buffer_tmp[tmp], (unsigned char)buffer_tmp[tmp + 1]);

		if ((unsigned char)buffer_tmp[tmp + 0x03] != 0)
		{
			SetRoomID((unsigned char)buffer_tmp[tmp + 0x03]);
			CopyString(GV.BytesEncryptMap, buffer_tmp + tmp - 17, 19);
			InfoServer("ROOM KEY RECEBIDO - IN GAME\n");
		}
		else
		{
			CommandAccess("/close_socket");
		}
	}

	if ((buffer_tmp[0] == 0x40) && (buffer_tmp[2] == 0x02))
	{
		if ((SC.TYPE_SCRIPT == 0x04) || (SC.TYPE_SCRIPT == 0x06))
		{
			AddStringToListBoxDebug("UM PLAYER DEIXOU O JOGO");
			InfoColor("UM PLAYER DEIXOU O JOGO\n", 4);
			DecrementPlayersMap();

			if ((GetPlayersConfirmed() <= 0x00) && (SC.TYPE_SCRIPT == 0x06))
			{
				IDRoomConfirmed(-1);
				SC.TYPE_SCRIPT = 0x00;
				SC.P_ROOM_ID = 0x00;
				CommandAccess("/close_socket");
			}
		}
	}

	/*if((buffer_tmp[0] == 0x48) && (buffer_tmp[2] == 0x02))
	{
	 //if((GetPlayersConfirmed() >= 0x01) && (SC.TYPE_SCRIPT != 0x04))
	 if((GetPlayersConfirmed() >= 0x01) && (SC.TYPE_SCRIPT == 0x04))
	  DecrementPlayersConfirmed();
	}*/

	if (buffer_tmp[0] == 0x78)
	{
		if (SC.TYPE_SCRIPT == 0x04)
		{
			if (buffer_tmp[6] == 0x00) // Ready
				IncrementPlayersConfirmed();
			else if (buffer_tmp[6] == 0x01) // UnReady
			{
				if (GetPlayersConfirmed() >= 0x01)
					DecrementPlayersConfirmed();
			}
		}
	}

	//\x7e\x00\x01\x00\xc4\x81\x11\x00\x62\x00\x00\x00\xff\xff\x28\x65
	if (buffer_tmp[0] == 0x7e)
	{
		if (SC.TYPE_SCRIPT == 0x04)
		{
			AddStringToListBoxDebug("VOCE FOI KICADO DO JOGO");
			InfoColor("VOCE FOI KICADO DO JOGO\n", 1);
			//SC.TYPE_SCRIPT = 0x01; // volta como Master
			printf("KICADO DO JOGO\n");
			CommandAccess("/close_socket");
		}

		if (SC.TYPE_SCRIPT == 0x06)
		{
			AddStringToListBoxDebug("VOCE FOI KICADO DO JOGO");
			InfoColor("VOCE FOI KICADO DO JOGO\n", 1);
			//SC.TYPE_SCRIPT = 0x00; // volta como Player
			IDRoomConfirmed(-1);
			SC.TYPE_SCRIPT = 0x00;
			SC.P_ROOM_ID = 0x00;
			//CommandAccess("/reset");
		}
	}

	if (buffer_tmp[0] == 0x7f)
	{
		AddStringToListBoxDebug("IMPOSSIVEL INICIAR PARTIDA");
		InfoColor("IMPOSSIVEL INICIAR PARTIDA\n", 1);
		ResetRoom();
		CommandAccess("/close_socket");
	}

	if ((((unsigned char)buffer_tmp[0] == 0x55)))
	{
		if (!GetTurn())
		{
			SetMyTurn(0);
			Confirmed_Shot();
			//Confirmed_Shot();
			SendTurnOK();
		}
	}

	if ((buffer_tmp[0] == 0x15) && (buffer_tmp[2] == 0x0f))
	{
		InitPlayerMap(); // INICIA TB QUANDO FOR O TURNO
	}

	if (((unsigned char)buffer_tmp[0] == 0x90) || ((unsigned char)buffer_tmp[0] == 0x67))
	{
		if (SC.TYPE_SCRIPT == 0x04)
			SetMyTurn(1); // INICIA PARTIDA
	}

	if (((unsigned char)buffer_tmp[0] == 0x65))
	{
		if ((SC.TYPE_SCRIPT == 0x04) || (SC.TYPE_SCRIPT == 0x06))
		{
			MapTrade();
			InfoColor("TROCA DE HOLE EFETUADO COM SUCESSO\n", 3);
		}
	}

	if (((unsigned char)buffer_tmp[0] == 0x6E))
		//if(((unsigned char)buffer_tmp[0] == 0x43))
	{
		if (SC.TYPE_SCRIPT == 0x02)
		{
			TradeMapPractice();
		}
	}

	// GAME FINALIZADO
	if (((unsigned char)buffer_tmp[0] == 0x66) && ((unsigned char)buffer_tmp[2] == 0x02))
	{
		if ((SC.TYPE_SCRIPT == 0x04) || (SC.TYPE_SCRIPT == 0x06))
		{
			//MapTrade();
			InfoColor("JOGO INTEIRO FINALIZADO\n", 3);
			sprintf_s(GV.CInfo, _countof(GV.CInfo), "RECEBEU PANG & EXP\n");
			InfoColor(GV.CInfo, 2);
			FinityGame();
		}
	}

	// PLAYER KICK GAME AND MASTER WIN
	if (((unsigned char)buffer_tmp[0] == 0x66) && ((unsigned char)buffer_tmp[2] == 0x01))
	{
		if ((SC.TYPE_SCRIPT == 0x04) || (SC.TYPE_SCRIPT == 0x06))
		{
			//MapTrade();
			InfoColor("JOGO PARCIALMENTE FINALIZADO\n", 3);
			sprintf_s(GV.CInfo, _countof(GV.CInfo), "RECEBEU PANG & EXP\n");
			InfoColor(GV.CInfo, 2);
			FinityGame();
		}
	}

	// \x2f\x02\x02\x00\x00\x00\x00\x20\x4c\x6f\x76\x65\x20\x50
	// \x31\x02\x37\x00\x00\x00\x00\x00\x00\x28\x65\x35\x35\x29
	if (((unsigned char)buffer_tmp[0] == 0x31) && ((unsigned char)buffer_tmp[1] == 0x02))
	{
		if (SC.TYPE_SCRIPT == 0x05)
			SC.TYPE_SCRIPT = 0x06;
	}

	// ROOM KEY PRACTICE
	if (((GV.packet_decrypt[9] == 0x0E) && (GV.packet_decrypt[10] == 0x49)))
	{
		if (SC.TYPE_SCRIPT == 0x02)
		{
			CopyString(GV.BytesEncryptMap, (char*)GV.packet_decrypt + 62, 19);
			InfoServer("ROOM KEY PRACTICE RECEBIDO\n");
		}
	}

	/*if(((GV.packet_decrypt[9] == 0x53) && (GV.packet_decrypt[10] == 0x00)))
	{
	 if((SC.TYPE_SCRIPT == 0x02))
	 {
	  SetMyTurn(1);     // INICIA PARTIDA
	 }
	}*/

	return 0;
}

DWORD ThreadConsole(LPVOID param)
{
	char command[127];
	ShowCommand();

	while (GetStatusConsole())
	{
		ColorText("command -> ", 4);
		scanf_s("%s", command, _countof(command));
		CommandAccess((char*)command);
		Sleep(1);
	}
	SetStatusRecvPacket(FALSE);
#ifdef _WIN32
	ExitProcess(0);
#elif LINUX
  exit(0);
#endif
}

#define LINE_CHAT 100

void AddStringToListBoxChat(char* buffer)
{
	if (counter_listChat == LINE_CHAT)
	{
#ifdef _WIN32
		SendMessage(ListBoxChat, LB_DELETESTRING, 0, 0);
#elif LINUX
		// GTK ListBox
#endif
	}
	else
		counter_listChat++;

#ifdef _WIN32
	SendMessage(ListBoxChat, LB_ADDSTRING, 0, (LPARAM)buffer);
	SendMessage(ListBoxChat, WM_VSCROLL, SB_LINEDOWN, 0);
	//SendMessage(ListBoxChat, LB_SETCURSEL, counter_listChat-1, 0);
#elif LINUX
	// GTK ListBox
#endif
}

#define LINE_DEBUG 500

void AddStringToListBoxDebug(char* buffer)
{
	if (counter_listDebug == LINE_DEBUG)
	{
#ifdef _WIN32
		SendMessage(ListBoxDebug, LB_DELETESTRING, 0, 0);
#elif LINUX
		// GTK ListBox
#endif
	}
	else
		counter_listDebug++;

#ifdef _WIN32
	SendMessage(ListBoxDebug, LB_ADDSTRING, 0, (LPARAM)buffer);
	SendMessage(ListBoxDebug, WM_VSCROLL, SB_LINEDOWN, 0);
	//SendMessage(ListBoxDebug, LB_SETCURSEL, counter_listDebug-1, 0);
#elif LINUX
	// GTK ListBox
#endif
}

void OpenDialog()
{
#ifdef _WIN32
	CThreadDialog = (HWND)CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadDialog, (LPVOID)0, 0, PThread);
#elif LINUX
  pthread_exit(&CThreadDialog, NULL, &ThreadDialog, NULL);
#endif
}

void TermineteThreads()
{
#ifdef _WIN32
	TerminateThread(CThreadDialog, 0);
	TerminateThread(CThreadRecvPacket, 0);
	TerminateThread(CThreadSendTimePacket, 0);
#elif LINUX
  pthread_exit(CThreadDialog);
  pthread_exit(CThreadRecvPacket);
  pthread_exit(CThreadSendTimePacket);
#endif
}
