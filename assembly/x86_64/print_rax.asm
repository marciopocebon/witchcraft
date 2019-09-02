section .data

codes:
    db '0123456789ABCDEF'

section .text
global _start

_start:
    ; numero 112233... em hexadecimal
    mov rax, 0x1122334455667788

    mov rdi, 1
    mov rdx, 1
    mov rcx, 64

    ; cada 4 bits devem ser exibidos como um digito hexadecimal
    ; Use deslocamento (shift) e a operacao bit a bit AND para isola-los
    ; O resultado e offset no array codes

.loop:
    push rax
    sub rcx, 4
    sar rax, cl
    and rax, 0xf

    lea rsi, [codes + rax]
    mov rax, 1

    ;syscall

    push rcx
    syscall
    pop rcx

    pop rax
    test rcx, rcx
    jnz .loop

    mov rax, 60 ; faz chamada da syscall exit
    xor rdi, rdi
    syscall