#include "encrypt.h"

#include <stdio.h>


#include "chat.h"
#include "decrypt.h"

unsigned int packet_key0_add = 0;
unsigned int packet_key1_add = 0;

short int SetByteInfoEncrypt = 0;

void SetByteEncrypt(unsigned short int byte)
{
	SetByteInfoEncrypt = byte;
}

void reset_keys()
{
	packet_key0_add = 0;
	packet_key1_add = 0;
}

void EncryptPacket(unsigned char* packet_buffer, unsigned int size_packet)
{
	unsigned short int tmp_byte;
	unsigned short int tmp_bl;
	unsigned short int tmp_xor;

	//packet_buffer[0] = rand() % 0xFE + 0x01;

	//printf("SetByteInfo [%d]\n", SetByteInfo);
	tmp_byte = SetByteInfoEncrypt << 8;
	tmp_byte += packet_buffer[0];
	//printf("Shif 8 << byte [%x] + packet_buffer[0]\n", tmp_byte);
	tmp_bl = TabletEncrypt[tmp_byte];
	//printf("tmp_bl [%x]\n", tmp_bl);

	tmp_xor = packet_buffer[3];
	tmp_xor ^= tmp_bl;

	//printf("xor [%x]\n", tmp_xor);

	if (RECV_DEBUG_MODE_TYPE() == 1)
		printf("************ Decrypt ************\n");
	else if (RECV_DEBUG_MODE_TYPE() == 2)
		AddStringToListBoxDebug("************ Decrypt *************");

	ShowPacketInHex(packet_buffer, size_packet);

	for (int i = size_packet; i >= 10; i--)
	{
		//printf("%x %x = %x\n", packet_buffer[i], packet_buffer[i-4], packet_buffer[i] ^packet_buffer[i-4]);
		packet_buffer[i] ^= packet_buffer[i - 4];
	}

	packet_buffer[7] ^= tmp_xor;
	packet_buffer[8] ^= 0x00;
	packet_buffer[9] ^= 0x00;
	packet_buffer[10] ^= 0x00;

	if (RECV_DEBUG_MODE_TYPE() == 1)
		printf("************ Encrypt ************\n");
	else if (RECV_DEBUG_MODE_TYPE() == 2)
		AddStringToListBoxDebug("************ Encrypt *************");

	ShowPacketInHex(packet_buffer, size_packet + 4);

	if (RECV_DEBUG_MODE_TYPE() == 1)
		printf("*********************************\n");
	else if (RECV_DEBUG_MODE_TYPE() == 2)
		AddStringToListBoxDebug("*********************************");
}

void ClientEncryptPacket(unsigned char* packet_encrypt, unsigned char* packet_buffer, unsigned int size_packet)
{
	int tmp;

	unsigned short int tmp_byte;
	unsigned short int tmp_bl;
	unsigned short int tmp_dl;

	//packet_buffer[0] = 0xAD; // 0x8A

	//packet_buffer[0] = rand() % 0xFE + 0x01;

	string_copy(packet_encrypt, packet_buffer, 12);

	tmp_byte = SetByteInfoEncrypt << 8;

	tmp_bl = TabletEncrypt[tmp_byte + packet_buffer[0]];
	tmp_dl = TabletEncrypt[tmp_byte + packet_buffer[0] + 0x1000];

	tmp_byte += packet_buffer[0];

	//printf("BL -> %x DL -> %x\n", tmp_bl, tmp_dl);

	packet_buffer[4] = (unsigned char)tmp_dl;
	packet_encrypt[4] = (unsigned char)tmp_dl;

	if (RECV_DEBUG_MODE_TYPE() == 1)
		printf("************ Decrypt ************\n");
	else if (RECV_DEBUG_MODE_TYPE() == 2)
		AddStringToListBoxDebug("************ Decrypt *************");

	ShowPacketInHex(packet_buffer, size_packet);

	for (int i = 8; i <= 12; i++)
	{
		tmp = packet_buffer[i - 4];
		tmp ^= packet_buffer[i];
		packet_encrypt[i] = tmp;
	}

	for (unsigned i = 12; i < size_packet; i++)
	{
		tmp = packet_buffer[i - 4];
		tmp ^= packet_buffer[i];
		packet_encrypt[i] = tmp;
	}

	packet_buffer[4] ^= tmp_bl;
	packet_buffer[5] ^= 0x00;
	packet_buffer[6] ^= 0x00;
	packet_buffer[7] ^= 0x00;

	packet_encrypt[4] = packet_buffer[4];
	packet_encrypt[5] = packet_buffer[5];
	packet_encrypt[6] = packet_buffer[6];
	packet_encrypt[7] = packet_buffer[7];

	for (unsigned i = size_packet; i < size_packet + 4; i++)
	{
		packet_encrypt[i] = packet_buffer[i - 4];
	}

	if (RECV_DEBUG_MODE_TYPE() == 1)
		printf("************ Encrypt ************\n");
	else if (RECV_DEBUG_MODE_TYPE() == 2)
		AddStringToListBoxDebug("************ Encrypt *************");

	ShowPacketInHex(packet_encrypt, size_packet + 4);

	if (RECV_DEBUG_MODE_TYPE() == 1)
		printf("*********************************\n");
	else if (RECV_DEBUG_MODE_TYPE() == 2)
		AddStringToListBoxDebug("*********************************");
}

void string_copy(unsigned char* string1, unsigned char* string2, unsigned int size)
{
	for (unsigned i = 0; i <= size; i++)
	{
		string1[i] = string2[i];
	}
}

void PacketSoma(unsigned char* buffer, int size, bool SetIncrementTwoByte)
{
	unsigned short int tmp_byte;
	unsigned short int tmp_bl;
	unsigned short int tmp_dl;

	//buffer[0] = rand() % 0xFE + 0x01;

	tmp_byte = SetByteInfoEncrypt << 8;

	tmp_bl = TabletEncrypt[tmp_byte + buffer[0]];
	tmp_dl = TabletEncrypt[tmp_byte + buffer[0] + 0x1000];

	buffer[4] = tmp_dl ^ tmp_bl;
	buffer[3] = packet_key0_add;

	//if((buffer[size-2] == 0xF4) && (buffer[size-1] == 0x00))
	if (SetIncrementTwoByte)
	{
		buffer[size - 3] = packet_key1_add;
		packet_key1_add++;
	}

	packet_key0_add++;
}
