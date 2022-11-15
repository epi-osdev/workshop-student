; Switching from real mode to protected mode

%include "src/boot_sector/gdt.asm"

[bits 16]                   ; real mode
switch_to_pm:
    cli                     ; 1. disable interrupts
    lgdt [GDT_descriptor]   ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1             ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; 4. far jump by using a different segment

[bits 32]                   ; protected mode
init_pm:                    ; we are now using 32-bit instructions
    mov ax, DATA_SEG        ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call entry_point        ; 7. Call a well-known label with useful code
