;name: sleep.asm
;
;build: nasm -felf64 sleep.asm -o sleep.o
;
;description: running the program will pause execution s useconds.

global usleep

section .text

usleep:
    push ebp
    mov ebp,esp
    mov eax,ebp
    mov ah,0x86
    mov edx,eax
    int 0x15
    mov esp,ebp
    pop ebp
    ret