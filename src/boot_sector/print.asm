; print generic function
; parameters:
;   - string to print -> bx

print:
    pusha

.print_loop:
    mov al, [bx]                ; al register contains char to print
    cmp al, 0
    je .print_done
    mov ah, 0x0e                ; put in tty mode
    int 0x10                    ; call BIOS
    inc bx
    jmp .print_loop

.print_done:
    popa
    ret

print_newline:
    pusha
    mov ah, 0x0e                ; put in tty mode
    mov al, 0x0a                ; newline
    int 0x10                    ; call BIOS
    mov al, 0x0d                ; carriage return
    int 0x10                    ; call BIOS
    popa
    ret