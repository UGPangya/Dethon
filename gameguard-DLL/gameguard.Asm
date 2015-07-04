.386
.model flat,stdcall
option casemap:none

include gameguard.inc

.data

PacketCommandLine db 200 dup (0)

npgl	db 'npgl', 00h
npgg	db 'npgg', 00h
npgm	db 'npgm', 00h
npsc	db 'npsc', 00h
npgmup	db 'npgmup', 00h

PacketInfoTest db ' 0 PangyaUS ec 100 c44 0 70718 ', 00h

.code

DllEntry proc hInst:HINSTANCE, reason:DWORD, reserved1:DWORD

    push    hInst
    pop     hInstance
    mov     eax,TRUE
    ret

DllEntry Endp

gameguard_init proc PacketInfo:DWORD

	;push offset npgl
	;call function_test
	;add esp, 04h
	 
	;push offset npgg
	;call function_test
	;add esp, 04h
	 
	;push offset npgm
	;call function_test
	;add esp, 04h
	 
	;push offset npsc
	;call function_test
	;add esp, 04h
	 
	;push offset npgmup
	;call function_test
	;add esp, 04h
	
	;push offset PacketInfoTest
	push PacketInfo
	call function_test
	add esp, 04h
	
	ret 04h
gameguard_init endp

function_test proc
 push edi
 mov edi, dword ptr ss:[ESP+08h]
 push EDI
 call lstrlen
 
 db 085h, 0C0h, 00Fh, 084h, 083h, 000h, 000h, 000h, 08Bh, 0CFh, 053h, 081h, 0E1h, 0FFh, 000h, 000h, 000h, 056h

 MOV BYTE PTR DS:[PacketCommandLine+1],CL
 MOV BYTE PTR DS:[PacketCommandLine],1
 
 db 08Dh, 054h, 049h, 065h, 08Dh, 04Ch, 049h, 001h, 089h, 054h, 024h, 010h, 0BEh, 004h, 000h, 000h, 000h, 08Dh, 00Ch, 049h, 08Dh, 051h, 065h, 041h, 085h, 0C0h, 07Eh, 025h, 055h
 
 SUB EDI, offset PacketCommandLine+4
 MOV EBP,EAX
 
 JUMPED_PRIMARY:
 MOV BL,BYTE PTR DS:[EDI+ESI+PacketCommandLine]
 LEA ECX,DWORD PTR DS:[ECX+ECX*02h]
 MOV AL,CL
 ADD AL,65h
 XOR BL,AL
 
 MOV BYTE PTR DS:[ESI+PacketCommandLine],BL
 
 INC ESI
 INC ECX
 DEC EBP
 JNZ JUMPED_PRIMARY
 POP EBP
 MOV BL,BYTE PTR SS:[ESP+10h]
 LEA ECX,DWORD PTR DS:[ESI-04h]
 MOV EAX,ECX
 
 MOV BYTE PTR DS:[ESI+PacketCommandLine],0
 
 SAR EAX,08h
 XOR AL,BL
 XOR DL,CL
 POP ESI
 
 MOV BYTE PTR DS:[PacketCommandLine+02h],AL
 POP EBX
 MOV BYTE PTR DS:[PacketCommandLine+03h],DL
 MOV EAX, offset PacketCommandLine
 POP EDI
 RETN
 MOV EAX, offset PacketCommandLine
 POP EDI
 RETN
function_test endp

;--------------------------------------------------------------------------------

End DllEntry
