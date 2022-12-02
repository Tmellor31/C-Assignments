.section .text
.align 2
.global_start

_start: 
   mov r0, #0x66 @ASCII for f, F is 0x46 -> need to subtract 0x00000020, or add 0xFFFFFFE0   
   mov r1, #0x75 @ASCII for u 
   mov r2, #0x6e @ASCII for n

   sub r0, r0, #0x20  

_end:
   b_end
