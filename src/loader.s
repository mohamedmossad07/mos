.set MAGIC,0x1BADB002
 /* magic number to tell the grub
;or  th boot loader that this is a kernel */
.set FALGS,3
.set CHECKSUM , -(MAGIC + FALGS)
.section .multiboot
    .long MAGIC
    .long FALGS
    .long CHECKSUM
.text
.extern Main 
/*calling(the kernel) the main function which 
;locates in kernel*/
.global loader

loader : 
    mov $kernel_stack ,%esp
    push %eax 
    /*this register has the multiboot structure
    ;which has some usefull info like memory size*/
    push %ebx 
    /*this regsiter has the magic number*/
    call Main
stop:
    cli 
    /*disable intrupts*/
    hlt 
    /*hlt proccessor for next intrupt(freezing)*/
    jmp stop 
    /*actually it is equev to shutdown */

.bss
.space 5*1024*1024,0
/*make space befor making */
/*the stack to dont ovveride data*/
kernel_stack:
