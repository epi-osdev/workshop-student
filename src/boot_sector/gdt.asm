CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0
                                            ; offset 0x8
gdt_code:                                   ; CS SHOULD POINT TO THIS
    dw 0xffff                               ; Segment limit first 0-15 bits
    dw 0                                    ; Base first 0-15 bits
    db 0                                    ; Base 16-23 bits
    db 0x9a                                 ; Access byte
    db 11001111b                            ; High 4 bit flags and the low 4 bit flags
    db 0                                    ; Base 24-31 bits

                                            ; offset 0x10
gdt_data:                                   ; DS, SS, ES, FS, GS
    dw 0xffff                               ; Segment limit first 0-15 bits
    dw 0                                    ; Base first 0-15 bits
    db 0                                    ; Base 16-23 bits
    db 0x92                                 ; Access byte
    db 11001111b                            ; High 4 bit flags and the low 4 bit flags
    db 0                                    ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start
