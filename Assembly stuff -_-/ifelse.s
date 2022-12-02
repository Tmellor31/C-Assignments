.section .text
.align 2
.global_start

_start: 
   mov r0, #1 
   mov r1, #2  
   cmp r0, r1
   addne r0, r1, r1, lsl #1 
   rsbeq r1, r0, r0, lsl #4 
_end:
   b_end
