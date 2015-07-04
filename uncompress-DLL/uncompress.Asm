.386
.model flat,stdcall
option casemap:none

include uncompress.inc

.data

packet_compress	db 000h, 000h, 000h, 037h, 00Dh, 040h, 000h, 000h, 010h, 000h, 028h, 065h, 034h, 031h, 029h, 045h, 06Dh, 065h, 06Ch, 06Ch, 079h, 08Bh, 001h, 020h, 000h, 041h, 03Dh, 000h, 000h, 011h, 000h, 000h
local_decompress db 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h

.code

DllEntry proc hInst:HINSTANCE, reason:DWORD, reserved1:DWORD

	;mov eax, offset packet_compress
	;push eax
	;mov eax, sizeof packet_compress
	;push eax
	;mov eax, offset local_decompress
	;push eax
	;call call_uncompress
	
    push    hInst
    pop     hInstance
    mov     eax,TRUE
    ret

DllEntry Endp

call_uncompress proc local_uncompress:DWORD, size_uncompress:DWORD, local_packet:DWORD
	push ebp
	mov ecx, local_uncompress
	mov edx, size_uncompress
	mov edi, local_packet

	push 0
	LEA EAX,DWORD PTR SS:[EBP-8Ch]
	PUSH EAX
	PUSH ECX
	ADD EDX, -4h
	PUSH EDX
	ADD EDI, 4h
	PUSH EDI
	
	call function_test
	add esp, 14h
	
	pop ebp
	ret 12
call_uncompress endp

function_test proc Param1:DWORD, Param2:DWORD, Param3:DWORD, Param4:DWORD, Param5:DWORD
 db 055h, 08Bh, 0ECh, 08Bh, 04Dh, 014h, 08Bh, 045h, 00Ch, 053h, 056h, 057h, 08Bh, 07Dh, 008h, 0C7h, 001h, 000h, 000h, 000h, 000h, 08Ah, 017h, 08Dh, 01Ch, 007h, 08Bh, 045h, 010h, 08Bh, 0CFh, 080h
 db 0FAh, 011h, 076h, 01Dh, 00Fh, 0B6h, 0F2h, 083h, 0EEh, 011h, 08Dh, 04Fh, 001h, 083h, 0FEh, 004h, 00Fh, 082h, 0CAh, 000h, 000h, 000h, 08Ah, 011h, 088h, 010h, 040h, 041h, 04Eh, 075h, 0F7h, 0EBh
 db 078h, 00Fh, 0B6h, 031h, 041h, 083h, 0FEh, 010h, 00Fh, 083h, 0D2h, 000h, 000h, 000h, 085h, 0F6h, 075h, 022h, 080h, 039h, 000h, 075h, 015h, 0EBh, 007h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
 db 041h, 081h, 0C6h, 0FFh, 000h, 000h, 000h, 080h, 039h, 000h, 074h, 0F4h, 00Fh, 0B6h, 011h, 08Dh, 074h, 016h, 00Fh, 041h, 08Bh, 011h, 089h, 010h, 083h, 0C0h, 004h, 083h, 0C1h, 004h, 04Eh, 074h
 db 038h, 083h, 0FEh, 004h, 072h, 02Ah, 08Bh, 011h, 089h, 010h, 083h, 0EEh, 004h, 083h, 0C0h, 004h, 083h, 0C1h, 004h, 083h, 0FEh, 004h, 073h, 0EEh, 085h, 0F6h, 074h, 01Dh, 08Dh, 064h, 024h, 000h
 db 08Ah, 011h, 088h, 010h, 040h, 041h, 04Eh, 075h, 0F7h, 0EBh, 00Eh, 0EBh, 003h, 08Dh, 049h, 000h, 08Ah, 011h, 088h, 010h, 040h, 041h, 04Eh, 075h, 0F7h, 00Fh, 0B6h, 031h, 041h, 083h, 0FEh, 010h
 db 073h, 05Eh, 00Fh, 0B6h, 011h, 003h, 0D2h, 003h, 0D2h, 0C1h, 0EEh, 002h, 08Bh, 0F8h, 02Bh, 0FEh, 02Bh, 0FAh, 00Fh, 0B6h, 097h, 0FFh, 0F7h, 0FFh, 0FFh, 088h, 010h, 00Fh, 0B6h, 097h, 000h, 0F8h
 db 0FFh, 0FFh, 088h, 050h, 001h, 00Fh, 0B6h, 097h, 001h, 0F8h, 0FFh, 0FFh, 041h, 088h, 050h, 002h, 083h, 0C0h, 003h, 00Fh, 0B6h, 071h, 0FEh, 083h, 0E6h, 003h, 00Fh, 084h, 041h, 0FFh, 0FFh, 0FFh
 db 08Ah, 011h, 088h, 010h, 040h, 041h, 083h, 0FEh, 001h, 076h, 011h, 08Ah, 011h, 088h, 010h, 040h, 041h, 083h, 0FEh, 002h, 076h, 006h, 08Ah, 011h, 088h, 010h, 040h, 041h, 00Fh, 0B6h, 031h, 041h
 db 083h, 0FEh, 040h, 072h, 03Ah, 08Bh, 0D6h, 0C1h, 0EAh, 002h, 083h, 0E2h, 007h, 08Bh, 0F8h, 02Bh, 0FAh, 00Fh, 0B6h, 011h, 003h, 0D2h, 003h, 0D2h, 003h, 0D2h, 02Bh, 0FAh, 0C1h, 0EEh, 005h, 04Fh
 db 041h, 04Eh, 00Fh, 0B6h, 017h, 088h, 010h, 00Fh, 0B6h, 057h, 001h, 088h, 050h, 001h, 083h, 0C0h, 002h, 083h, 0C7h, 002h, 08Ah, 017h, 088h, 010h, 040h, 047h, 04Eh, 075h, 0F7h, 0EBh, 094h, 083h
 db 0FEh, 020h, 072h, 030h, 083h, 0E6h, 01Fh, 075h, 01Bh, 080h, 039h, 000h, 075h, 00Eh, 08Bh, 0FFh, 041h, 081h, 0C6h, 0FFh, 000h, 000h, 000h, 080h, 039h, 000h, 074h, 0F4h, 00Fh, 0B6h, 011h, 08Dh
 db 074h, 016h, 01Fh, 041h, 00Fh, 0B7h, 011h, 0C1h, 0EAh, 002h, 08Bh, 0F8h, 02Bh, 0FAh, 04Fh, 083h, 0C1h, 002h, 0EBh, 048h, 08Bh, 0F8h, 083h, 0FEh, 010h, 00Fh, 082h, 08Fh, 000h, 000h, 000h, 08Bh
 db 0D6h, 083h, 0E2h, 008h, 0C1h, 0E2h, 00Bh, 02Bh, 0FAh, 083h, 0E6h, 007h, 075h, 019h, 080h, 039h, 000h, 075h, 00Ch, 041h, 081h, 0C6h, 0FFh, 000h, 000h, 000h, 080h, 039h, 000h, 074h, 0F4h, 00Fh
 db 0B6h, 011h, 08Dh, 074h, 016h, 007h, 041h, 00Fh, 0B7h, 011h, 0C1h, 0EAh, 002h, 02Bh, 0FAh, 083h, 0C1h, 002h, 03Bh, 0F8h, 074h, 07Bh, 081h, 0EFh, 000h, 040h, 000h, 000h, 083h, 0FEh, 006h, 00Fh
 db 082h, 05Dh, 0FFh, 0FFh, 0FFh, 08Bh, 0D0h, 02Bh, 0D7h, 083h, 0FAh, 004h, 00Fh, 08Ch, 050h, 0FFh, 0FFh, 0FFh, 08Bh, 017h, 089h, 010h, 083h, 0C0h, 004h, 083h, 0C7h, 004h, 083h, 0EEh, 002h, 090h
 db 08Bh, 017h, 089h, 010h, 083h, 0EEh, 004h, 083h, 0C0h, 004h, 083h, 0C7h, 004h, 083h, 0FEh, 004h, 073h, 0EEh, 085h, 0F6h, 00Fh, 084h, 0D9h, 0FEh, 0FFh, 0FFh, 08Dh, 09Bh, 000h, 000h, 000h, 000h
 db 08Ah, 017h, 088h, 010h, 040h, 047h, 04Eh, 075h, 0F7h, 0E9h, 0C5h, 0FEh, 0FFh, 0FFh, 00Fh, 0B6h, 011h, 003h, 0D2h, 0C1h, 0EEh, 002h, 003h, 0D2h, 02Bh, 0FEh, 02Bh, 0FAh, 00Fh, 0B6h, 057h, 0FFh
 db 088h, 010h, 00Fh, 0B6h, 017h, 041h, 088h, 050h, 001h, 083h, 0C0h, 002h, 0E9h, 0A2h, 0FEh, 0FFh, 0FFh, 02Bh, 045h, 010h, 08Bh, 055h, 014h, 089h, 002h, 03Bh, 0CBh, 075h, 007h, 05Fh, 05Eh, 033h
 db 0C0h, 05Bh, 05Dh, 0C3h, 01Bh, 0C0h, 05Fh, 083h, 0E0h, 0FCh, 05Eh, 083h, 0C0h, 0FCh, 05Bh, 05Dh, 0C3h
 ret
function_test endp

call_encrypt proc Param1:DWORD, Param2:DWORD
 mov esi, Param1 ;  (Packet Byte para Encrypt)
 push esi
 mov ecx, Param2 ; (Packet para encrypt)
 push ecx
 MOV ECX,0E37BA4h
 call function_encrypt
 ret 08h
call_encrypt endp

function_encrypt proc
 PUSH ebp
 MOV EBP,ESP
 SUB ESP,2Ch
 
 MOV EAX,04A3E31C7h
 XOR EAX,EBP
 
 db 089h, 045h, 0FCh
 
 db 08Bh, 055h, 008h, 053h, 056h, 057h, 0B9h, 009h, 000h, 000h, 000h, 08Bh, 0F2h, 08Dh, 07Dh, 0D4h
 db 0F3h, 0A5h, 0A4h, 08Bh, 075h, 00Ch, 033h, 0C0h, 033h, 0C9h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Ah, 01Ch, 030h, 030h, 05Ch, 00Dh, 0D4h, 040h, 083h, 0F8h, 010h, 07Ch, 002h, 033h, 0C0h, 041h
 db 083h, 0F9h, 025h, 07Ch, 0EBh, 0B9h, 009h, 000h, 000h, 000h, 08Dh, 075h, 0D4h, 08Bh, 0FAh, 0F3h, 0A5h, 08Bh, 04Dh, 0FCh, 0A4h, 05Fh, 05Eh, 033h, 0CDh, 05Bh
 MOV esp,ebp
 pop ebp
 ret 08h
function_encrypt endp



;--------------------------------------------------------------------------------

End DllEntry
