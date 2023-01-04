#include <syscall.h>
using namespace mos;
void printf(int8_t *str);
SysCallHandler::SysCallHandler(uint8_t int_n,InterruptManager * int_mg)
:InterruptHandler(int_n,int_mg){

}

SysCallHandler::~SysCallHandler(){}

uint32_t SysCallHandler::handel(uint32_t esp){
    CPUState *cpu=(CPUState*)esp;
    switch (cpu->eax)
    {
    case 4:
        printf((char*)cpu->ebx);
        break;
    
    default:
        break;
    }
    return esp;
}