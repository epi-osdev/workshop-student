[org 0x7c00]

[bits 16]
start:
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov bp, 0x7c00
    mov sp, bp

[bits 16]
run_16:
    mov [BOOT_DRIVE], dl
    call load_kernel
    call switch_to_pm

%include "src/boot_sector/disk.asm"
%include "src/boot_sector/switch_pm.asm"
%include "src/boot_sector/gdt.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 31
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
entry_point:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0
KERNEL_OFFSET equ 0x1000

times 510 - ($-$$) db 0
dw 0xaa55
