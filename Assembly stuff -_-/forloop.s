.section .text 
.align 2
.global_start 

_start: 
  mov r0, #0
  mov r1, #1
  mov r2, #4 @b
  mov r3, r1 @i 
 
_loop:
  cmp r3, r2 @ if i >= then go to end
  bge _end @ is r3 >= r2? 
  add r0, #1 
  add r3, #1
  b _loop  


_end: 
  b _end 
