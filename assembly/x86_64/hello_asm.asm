; compile: nasm -felf64 hello_asm.asm -o hello.o
; ld -o hello hello.o
; chmod +x hello

global _start

section .data
message: db 'hello world', 10

section .text
_start:
    mov rax, 1          ;armazena numero da chamada de sistema em rax
    mov rdi, 1         
    mov rsi, message
    mov rdx, 14
    syscall
