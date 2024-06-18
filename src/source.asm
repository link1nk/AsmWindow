[bits 64]
default rel

%include "include\IO_lib.inc"
%include "include\WND_lib.inc"

%define WindowWidth  640
%define WindowHeight 480
%define NULL 0

section .data
	WindowName:  db "Beauty Window xD", 0
	ClassName:   db "Window", 0

section .bss
	alignb 8
	
	hInstance: resq 1
	wnd: resb Size_WNDCLASSEXA
	msg: resb Size_MSG
	hWnd: resq 1

section .text

	global Start

Start:
	sub rsp, 40

	xor ecx, ecx
	call GetModuleHandleA
	mov qword[hInstance], rax

	call  WinMain

.Exit:
	add rsp, 32
	xor ecx, ecx
	jmp exit

WinMain:
	push rbp
	mov rbp, rsp
	sub rsp, 32

	mov dword[wnd + WNDCLASSEXA.cbSize], 80
	mov dword[wnd + WNDCLASSEXA.style], CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW
	lea rax, [REL WndProc]
	mov qword[wnd + WNDCLASSEXA.lpfnWndProc], rax
	mov dword[wnd + WNDCLASSEXA.cbClsExtra], NULL
	mov dword[wnd + WNDCLASSEXA.cbWndExtra], NULL
	mov rax, qword[REL hInstance]
	mov qword[wnd + WNDCLASSEXA.hInstance], rax

	sub rsp, 16
	xor ecx, ecx
	mov edx, IDI_APPLICATION
	mov r8d, IMAGE_ICON
	xor r9d, r9d
	mov qword[rsp + 4 * 8], NULL
	mov qword[rsp + 5 * 8], LR_SHARED
	call LoadImageA
	mov qword [wnd + WNDCLASSEXA.hIcon], rax
	add rsp, 16

	sub rsp, 16
	xor ecx, ecx
	mov edx, IDC_ARROW
	mov r8d, IMAGE_CURSOR
	xor r9d, r9d
	mov qword[rsp + 4 * 8], NULL
	mov qword[rsp + 5 * 8], LR_SHARED
	call LoadImageA
	mov qword[wnd + WNDCLASSEXA.hCursor], rax
	add rsp, 16

	mov qword[wnd + WNDCLASSEXA.hbrBackground], COLOR_WINDOW + 1
	mov qword[wnd + WNDCLASSEXA.lpszMenuName], NULL
	lea rax, [ClassName]
	mov qword[wnd + WNDCLASSEXA.lpszClassName], rax

	sub rsp, 16
	xor ecx, ecx
	mov edx, IDI_APPLICATION
	mov r8d, IMAGE_ICON
	xor r9d, r9d
	mov qword[rsp + 4 * 8], NULL
	mov qword[rsp + 5 * 8], LR_SHARED
	call LoadImageA
	mov qword[wnd + WNDCLASSEXA.hIconSm], rax
	add rsp, 16

	lea rcx, [wnd]
	call RegisterClassExA

	sub rsp, 64
	mov ecx, WS_EX_COMPOSITED
	lea rdx, [ClassName]
	lea r8, [WindowName]
	mov r9d, WS_OVERLAPPEDWINDOW
	mov dword[rsp + 4 * 8], CW_USEDEFAULT
	mov dword[rsp + 5 * 8], CW_USEDEFAULT
	mov dword[rsp + 6 * 8], WindowWidth
	mov dword[rsp + 7 * 8], WindowHeight
	mov qword[rsp + 8 * 8], NULL
	mov qword[rsp + 9 * 8], NULL
	mov rax, qword[hInstance]
	mov qword[rsp + 10 * 8], rax
	mov qword[rsp + 11 * 8], NULL
	call CreateWindowExA
	mov qword[hWnd], rax
	add rsp, 64

	mov rcx, qword[hWnd]
	mov edx, SW_SHOWNORMAL
	call ShowWindow

	mov rcx, qword[hWnd]
	call UpdateWindow

.MessageLoop:
	lea rcx, [msg]
	xor edx, edx
	xor r8d, r8d
	xor r9d, r9d
	call GetMessageA
	cmp rax, 0
	je .Done

	mov rcx, qword[hWnd]
	lea rdx, [msg]
	call IsDialogMessageA  ; For keyboard strokes
	test rax, rax
	jnz .MessageLoop       ; Skip TranslateMessage and DispatchMessageA

	lea rcx, [msg]
	call TranslateMessage

	lea rcx, [msg]
	call DispatchMessageA
	jmp .MessageLoop

.Done:
	mov rsp, rbp
	pop rbp
	xor eax, eax
	ret

WndProc:
	push rbp
	mov rbp, rsp

%define hWnd   rbp + 16    ; Localização do Shadow Space setado
%define uMsg   rbp + 24    ; pela Calling Function
%define wParam rbp + 32
%define lParam rbp + 40

	mov qword[hWnd], rcx   ; Free up RCX RDX R8 R9 by spilling the
	mov qword[uMsg], rdx   ; 4 passed parameters to the shadow space
	mov qword[wParam], r8
	mov qword[lParam], r9

	cmp qword[uMsg], WM_DESTROY
	je WMDESTROY

.DefaultMessage:
	sub rsp, 32
	mov rcx, qword[hWnd]
	mov rdx, qword[uMsg]
	mov r8, qword[wParam]
	mov r9, qword[lParam]
	call DefWindowProcA
	add rsp, 32

	mov rsp, rbp
	pop rbp
	ret

WMDESTROY:
	sub rsp, 32
	xor ecx, ecx
	call PostQuitMessage
	add RSP, 32

	xor eax, eax
	mov rsp, rbp
	pop rbp
	ret
