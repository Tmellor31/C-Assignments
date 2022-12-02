.section .text
.align 2
.global _start
_start: mov r0,#0
        mov r1,#1
        mov r2,#2
        mov r3,#3
        mul r4, r0, r3
        mul r5, r1, r2
        sub r4, r5
_end: b _end
