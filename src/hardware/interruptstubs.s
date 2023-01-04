 .set IRQ_BASE ,0X20
.text
/**
this function(handelInterrupt) exists in InterruptManager class
but i can`t extern it as its name so i just compiled it and 
got its name after compilation
*/
.extern _ZN3mos8hardware16InterruptManager15handelInterruptEhj

.macro  _handelInterruptRequest num
.global _ZN3mos8hardware16InterruptManager26handelInterruptRequest\num\()Ev
_ZN3mos8hardware16InterruptManager26handelInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE,(int_n) 
    pushl $0
    jmp context_switching
.endm


.macro  _handelInterruptException num
.global _ZN3mos8hardware16InterruptManager19handleException\num\()Ev
_ZN3mos8hardware16InterruptManager26handelInterruptException\num\()Ev:
    movb $\num ,(int_n) 
    jmp context_switching
.endm


; _handelInterruptException 0x00
; _handelInterruptException 0x01
; _handelInterruptException 0x02
; _handelInterruptException 0x03
; _handelInterruptException 0x04
; _handelInterruptException 0x05
; _handelInterruptException 0x06
; _handelInterruptException 0x07
; _handelInterruptException 0x08
; _handelInterruptException 0x09
; _handelInterruptException 0x0A
; _handelInterruptException 0x0B
; _handelInterruptException 0x0C
; _handelInterruptException 0x0D
; _handelInterruptException 0x0E
; _handelInterruptException 0x0F 

_handelInterruptRequest 0x00
_handelInterruptRequest 0x01
_handelInterruptRequest 0x02
_handelInterruptRequest 0x03
_handelInterruptRequest 0x04
_handelInterruptRequest 0x05
_handelInterruptRequest 0x06
_handelInterruptRequest 0x07
_handelInterruptRequest 0x08
_handelInterruptRequest 0x09
_handelInterruptRequest 0x0A
_handelInterruptRequest 0x0B
_handelInterruptRequest 0x0C
_handelInterruptRequest 0x0D
_handelInterruptRequest 0x0E
_handelInterruptRequest 0x0F
_handelInterruptRequest 0x31
_handelInterruptRequest 0x80


context_switching:
/*
before we switching from current code to the interrupt handelr
we should reserve registers data
*/ 
/*
this is our args to handelInterrupt function
1`arg is interrupt number
2`arg is current location of stack pointer 
after all of these we can take the return of the function by getting 
the value of the eax register
*/
pushl %ebp
pushl %edi
pushl %esi

pushl %edx
pushl %ecx
pushl %ebx
pushl %eax

pushl %esp
push (int_n)
call _ZN3mos8hardware16InterruptManager15handelInterruptEhj
mov %eax , %esp

popl %eax
popl %ebx
popl %ecx
popl %edx

popl %esi
popl %edi
popl %ebp

 
add $4, %esp

.global _ZN3mos8hardware16InterruptManager22ignoreInterruptRequestEv
_ZN3mos8hardware16InterruptManager22ignoreInterruptRequestEv:
iret

.data
    int_n : .byte 0
