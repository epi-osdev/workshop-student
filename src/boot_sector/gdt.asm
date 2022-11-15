; Global Descriptor Table

                                ; empty segment because GDT starts with that
GDT_start:                      ; don't remove the labels, they're needed to compute sizes and jumps
                                ; the GDT starts with a null 8-byte
    dd 0x0                      ; 4 byte
    dd 0x0                      ; 4 byte

                                ; code segment
GDT_code: 
    dw 0xffff                   ; segment length, bits 0-15
    dw 0x0                      ; segment base, bits 0-15
    db 0x0                      ; segment base, bits 16-23
    db 10011010b                ; flags (8 bits)
    db 11001111b                ; flags (4 bits) + segment length, bits 16-19
    db 0x0                      ; segment base, bits 24-31

                                ; data segment, same as GDT_code except for one flag
GDT_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b                ; flag differs from code segment
    db 11001111b
    db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1  ; size (16 bit), always one less of its true size
    dd GDT_start                ; address (32 bit)

; offsets to the segments in the GDT
CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start
