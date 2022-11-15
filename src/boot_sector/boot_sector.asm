[org 0x7c00]                ; boot sector offset because BIOS put boot_sector at this adress
                            ; it will put this offset for all code

mov [BOOT_DRIVE], dl        ; Remember that the BIOS sets us the boot drive in 'dl' on boot
mov bp, 0x9000              ; set the stack far from boot sector avoding overlapping
mov sp, bp

mov bx, WELCOME_MESSAGE
call print
call print_newline

call load_kernel            ; read the kernel from disk
call switch_to_pm           ; switching from real mode to protected mode

%include "src/boot_sector/print.asm"
%include "src/boot_sector/disk.asm"
%include "src/boot_sector/switch_pm.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET   ; Read from disk and store in 0x1000
    mov dh, 31              ; 1 sector = 512 bytes, PAY attention to kernel size
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
entry_point:
    call KERNEL_OFFSET      ; Give control to the kernel
    jmp $                   ; infinite loop until kernel give control back

BOOT_DRIVE db 0             ; It is a good idea to store it in memory because 'dl' may get overwritten
KERNEL_OFFSET equ 0x1000    ; The same one we used when linking the kernel in the Makefile

WELCOME_MESSAGE:
    db "Welcome to JackOS !", 0

times 510 - ($-$$) db 0     ; fill memory with zeros until we rich 510
dw 0xaa55                   ; magic number, BIOS needs it to know that's an os
