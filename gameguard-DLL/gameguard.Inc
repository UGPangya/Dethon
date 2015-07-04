include windows.inc
include user32.inc
include kernel32.inc
include gdi32.inc

includelib user32.lib
includelib kernel32.lib
includelib gdi32.lib

include masm32.inc
includelib masm32.lib

gameguard_init PROTO :DWORD
function_test PROTO

.data?

hInstance			dd ?