[org 0x7c00]
[bits 16]

start:
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32
    
 
[bits 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000

%include "src/boot_sector/disk.asm"
%include "src/boot_sector/gdt.asm"

times 510-($ - $$) db 0
dw 0xAA55
