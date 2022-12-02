.section .text
.align 2
.global_start

_start: 
   mov r0, #0 @a
   mov r1, #1 @b
   
_loop: 
   add r0, r0, #1 
   cmp r0, r1 
   blt _loop  

_end:
   b_end
