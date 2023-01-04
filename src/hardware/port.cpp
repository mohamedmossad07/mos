#include <hardware/port.h>
#include <common/types.h>
using namespace mos::hardware;
using namespace mos::common;
port::port(uint16_t pn){
    this->portnumber=pn;
}
port::~port(){};

port8Bit::port8Bit(uint16_t pn):port(pn) {}
port8Bit::~port8Bit(){}
uint8_t port8Bit::read(){
    uint8_t result;
    __asm__ volatile("inb %1 , %0" : "=a" (result) : "Nd" (portnumber) );
    return result; 
}
void port8Bit::write(uint8_t data){
    __asm__ volatile("outb %0,%1" : : "a" (data) , "Nd" (portnumber) );
}

port8BitSlow::port8BitSlow(uint16_t pn):port8Bit(pn) {}
port8BitSlow::~port8BitSlow(){}
void port8BitSlow::write(uint8_t data){
    __asm__ volatile("outb %0,%1\njmp 1f\n1:jmp 1f\n1:" : : "a" (data) , "Nd" (portnumber) );
}

port16Bit::port16Bit(uint16_t pn):port(pn) {}
port16Bit::~port16Bit(){}
uint16_t port16Bit::read(){
    uint16_t result;
    __asm__ volatile("inw %1,%0" :  "=a" (result) : "Nd" (portnumber) );
    return result; 
}
void port16Bit::write(uint16_t data){
    __asm__ volatile("outw %0,%1" : : "a" (data) , "Nd" (portnumber) );
}

port32Bit::port32Bit(uint16_t pn):port(pn) {}
port32Bit::~port32Bit(){}
uint32_t port32Bit::read(){
    uint32_t result;
    __asm__ volatile("inl %1,%0" :  "=a" (result) : "Nd" (portnumber) );
    return result; 
}
void port32Bit::write(uint32_t data){
    __asm__ volatile("outl %0,%1" : : "a" (data) , "Nd" (portnumber) );
}