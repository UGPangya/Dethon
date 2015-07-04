include windows.inc
include user32.inc
include kernel32.inc
include gdi32.inc

includelib user32.lib
includelib kernel32.lib
includelib gdi32.lib

include masm32.inc
includelib masm32.lib

call_uncompress PROTO :DWORD, :DWORD, :DWORD
uncompress PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
function_test PROTO :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
function_encrypt PROTO
call_encrypt PROTO :DWORD, :DWORD

.data?

hInstance			dd ?