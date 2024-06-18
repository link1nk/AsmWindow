;************************************************************************
; Input/Output Library (Código Fonte)
;
; Autor: Lincoln Dias
;
; Criação:      08 Jan 2024
; Atualização:  11 Jan 2024
; Montador:   Nasm
;
; Descrição:  Essa biblioteca foi desenvolvida para facilitar a
;             interação com operações de entrada e saída de dados
;             fornecendo um conjunto de funções prontas que ajudam
;             a simplificar tarefas comuns, como leitura de dados,
;             exibição de resultados ou manipulação de arquivos.
;
;************************************************************************

extern ExitProcess
extern GetStdHandle
extern WriteConsoleA
extern ReadConsoleA
extern GetLastError 

;-----------------------------------------+
;        MACROS PARA GetStdHandle         |
;-----------------------------------------+
%define STD_INPUT_HANDLE  -10
%define STD_OUTPUT_HANDLE -11
%define STD_ERROR_HANDLE  -12
;------------------------------------------

segment .text

;-------------------------------------------------------------------------
; void exit(uint error);
;-------------------------------------------------------------------------
; RCX -> error -> Codigo de erro
;-------------------------------------------------------------------------
exit:
	call ExitProcess
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; uint string_length(char* str);
;-------------------------------------------------------------------------
; RCX -> str -> Ponteiro para string
;-------------------------------------------------------------------------
string_length:
	xor rax, rax
	push rsi
	lea rsi, [rcx]
.loop:
	cmp byte[rsi + rax], 0
	je .end	
	inc rax
	jmp .loop
.end:
	pop rsi
	ret
;-------------------------------------------------------------------------
; RAX -> Tamanho da string
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; void print_string(char* string);
;-------------------------------------------------------------------------
; RCX -> string -> String a ser impressa
;-------------------------------------------------------------------------
print_string:
	push rcx
	call string_length
	push rax
	
	sub rsp, 40
	mov rcx, STD_OUTPUT_HANDLE
	call GetStdHandle
	
	mov rcx, rax
	mov r8, [rsp+40]
	mov rdx, [rsp+48]
	mov r9, 0
	mov qword[rsp], 0
	call WriteConsoleA
	
	add rsp, 56
	ret
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; void print_stderr(char* string);
;-------------------------------------------------------------------------
; RCX -> string -> String a ser impressa
;-------------------------------------------------------------------------
print_stderr:
	push rcx
	call string_length
	push rax
	
	sub rsp, 40
	mov rcx, STD_ERROR_HANDLE
	call GetStdHandle
	
	mov rcx, rax
	mov r8, [rsp+40]
	mov rdx, [rsp+48]
	mov r9, 0
	mov qword[rsp], 0
	call WriteConsoleA
	
	add rsp, 56
	ret
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; void print_char(char c);
;-------------------------------------------------------------------------
; RCX -> Codigo ascii do caractere
;-------------------------------------------------------------------------
print_char:
	push rcx
	sub rsp, 32
	mov rcx, STD_OUTPUT_HANDLE
	call GetStdHandle
	
	mov rcx, rax
	lea rdx, [rsp+32]
	mov r8, 1
	mov r9, 0
	mov qword[rsp], 0
	call WriteConsoleA
	
	add rsp, 40
	ret
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; void print_newline(void);
;-------------------------------------------------------------------------
print_newline:
	mov rcx, 0xa
	call print_char
	ret
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; void print_uint(uint64 number);
;-------------------------------------------------------------------------
; RCX -> number -> Numero a ser impresso
;-------------------------------------------------------------------------
print_uint:
	push rdi
	push rsi
	
	lea rdi, [rsp]
	mov rax, rcx
	push 0
	sub rsp, 16
	
	dec rdi
	mov r8, 10

.loop:
	xor rdx, rdx
	div r8
	or dl, 0x30
	dec rdi
	mov [rdi], dl
	test rax, rax
	jnz .loop
	
	mov rcx, rdi
	call print_string
	
	add rsp, 24
	pop rsi
	pop rdi
	ret
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; void print_int(int64 number)
;-------------------------------------------------------------------------
; RCX -> number -> Numero a ser impresso
;-------------------------------------------------------------------------
print_int:
	test rcx, rcx
	js .signed
	jmp print_uint
	
.signed:
	push rcx
	mov rcx, "-"
	call print_char
	pop rcx
	neg rcx
	call print_uint
	ret
;------------------------------------------------------------------------


;------------------------------------------------------------------------
; char read_char(void);
;------------------------------------------------------------------------
read_char:
	push 0
	sub rsp, 40
	mov rcx, STD_INPUT_HANDLE
	call GetStdHandle
	
	mov rcx, rax
	lea rdx, [rsp+40]
	mov r8, 1
	lea r9, [rsp+32]
	mov qword[rsp], 0
	call ReadConsoleA
	
	mov rax, [rsp+40]
	add rsp, 48
	ret
;-------------------------------------------------------------------------
; RAX -> Caractere lido / 0 caso esteja vazia
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; char* read_word(char[] buffer, size_t size);
;-------------------------------------------------------------------------
; RCX -> buffer de destino
; RDX -> tamanho do buffer de destino
;-------------------------------------------------------------------------
read_word:
    push r14
    push r15
    xor r14, r14 
    mov r15, rdx
    dec r15

.lerEntrada:
    push rcx
    call read_char
    pop rcx
    cmp al, 0x20
    je .lerEntrada
    cmp al, 0x0A
    je .lerEntrada
    cmp al, 0x0D
    je .lerEntrada
    cmp al, 0x09
    je .lerEntrada
    test al, al
    jz .fim

.salvarCaractere:
    mov byte [rcx + r14], al
    inc r14
    push rcx
    call read_char
    pop rcx
    cmp al, 0x20
    je .fim
    cmp al, 0x0A
    je .fim
    cmp al, 0x0D
    je .fim
    cmp al, 0x09
    je .fim
    test al, al
    jz .fim
    cmp r14, r15
    je .tamanhoInsuficiente
    jmp .salvarCaractere

.fim:
    mov byte [rcx + r14], 0
    mov rax, rcx 
   
    mov rdx, r14 
    pop r15
    pop r14
    ret

.tamanhoInsuficiente:
    xor rax, rax
    pop r15
    pop r14
    ret
;-------------------------------------------------------------------------
; RAX -> Ponteiro para string / 0 em caso de falha
; RDX -> Tamanho da palavra lida
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; unsigned int parse_uint(char* string);
;-------------------------------------------------------------------------
; RCX -> string -> String numerica
;-------------------------------------------------------------------------
parse_uint:
	push rsi
	lea rsi, [rcx]
	mov r8, 10
	xor rax, rax
	xor rcx, rcx
.loop:
	movzx r9, byte[rsi + rcx]
	cmp r9b, "0"
	jb .end
	cmp r9b, "9"
	ja .end
	xor rdx, rdx
	mul r8
	and r9b, 0x0f
	add rax, r9
	inc rcx
	jmp .loop
.end:
	mov rdx, rcx
	pop rsi
	ret
;-------------------------------------------------------------------------
; RAX -> Numero
; RDX -> Tamanho
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; unsigned int parse_int(char* string);
;-------------------------------------------------------------------------
; RCX -> string -> String numerica
;-------------------------------------------------------------------------
parse_int:
	cmp byte[rcx], "-"
	jne parse_uint
	inc rcx
	call parse_uint
	neg rax
	inc rdx
	ret
;-------------------------------------------------------------------------
; RAX -> Numero
; RDX -> Tamanho
;-------------------------------------------------------------------------

;-------------------------------------------------------------------------
; bool string_equals(char* str1, char* str2);
;-------------------------------------------------------------------------
; RCX -> str1 -> Primeira string
; RDX -> str2 -> Segunda string
;-------------------------------------------------------------------------
string_equals:
	mov al, byte[rcx]
    cmp al, byte[rdx]
    jne .no
    inc rcx
    inc rdx
    test al, al
    jnz string_equals
    mov rax, 1
    ret
    .no:
    xor rax, rax
    ret 
;-------------------------------------------------------------------------
; RAX -> 0 (Não são iguais) ou 1 (são iguais)
;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
; string_copy(char* string, char[] buffer, size_t bufferSize00);
;-------------------------------------------------------------------------
; RCX -> string -> Endereço da string a ser copiada
; RDX -> buffer -> Endereço do buffer de destino da string copiada 
; R8  -> bufferSize -> Tamanho do buffer de destino
;-------------------------------------------------------------------------
string_copy:
	push rcx
	push rdx
	push r8
	call string_length
	pop r8
	pop rdx
	pop rcx
	
	cmp r8, rax
	jbe .tamanhoInsuficiente
	
	push rsi
	push rdi
	
	lea rsi, [rcx]
	lea rdi, [rdx]
	push rdi
	
.loop:
	mov al, byte[rsi]
	mov byte[rdi], al
	inc rsi
	inc rdi
	test al, al
	jnz .loop
	
	pop rax
	pop rdi
	pop rsi
	ret
	
.tamanhoInsuficiente:
	xor rax, rax
	ret
;-------------------------------------------------------------------------
; RAX -> Endereço do Buffer / 0 (caso o tamanho do buffer seja menor)
;-------------------------------------------------------------------------