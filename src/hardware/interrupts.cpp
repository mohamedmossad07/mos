#include <hardware/interrupts.h>
#include <gdt.h>
using namespace mos::hardware;
using namespace mos;
void printf(char * str);
void printfHex(int8_t hex);

InterruptHandler::InterruptHandler(uint8_t int_n,InterruptManager * int_mg){
    this->int_mg=int_mg;
    this->int_n=int_n; 
    this->int_mg->handlers[int_n]=this;
}
InterruptHandler::~InterruptHandler(){
    if(int_mg->handlers[int_n]==this){
        int_mg->handlers[int_n]=0;
    }
}
 uint32_t InterruptHandler::handel(uint32_t esp){
   
    return esp;
}
 
InterruptManager::GateDescriptor InterruptManager::idt[256];
InterruptManager * InterruptManager::instance=0;
void InterruptManager::setIDTentry(
    uint8_t int_n,
    uint16_t codeSeg,
    void (*handler)(),
    uint8_t dpl,
    uint8_t type)
{
    const uint8_t PRESENT = 0x80;
    idt[int_n].handler_low = ((uint32_t) handler)& 0xffff;
    idt[int_n].handler_high = (((uint32_t) handler)>> 16)&0xffff;
    idt[int_n].cSeg=codeSeg;
    idt[int_n].zeros = 0;
    idt[int_n].access = PRESENT | type | ((dpl & 3)<< 5);
}
InterruptManager::InterruptManager(GlobalDescriptorTable *gdt,TaskManager * taskMgr):
picMasterCMD(0x20),
picMasterData(0x21),
picSlaveCMD(0xA0),
picSlaveData(0xA1),
gdt(gdt)
{
    this->taskMgr=taskMgr;
    uint32_t cSeg= gdt->CodeSegmentSelector();
    const uint8_t type=0xE;
    for (uint16_t i = 0; i < 256; i++)
    {
        handlers[i]=0;
        setIDTentry(i,cSeg,&ignoreInterruptRequest,0,type);
    }
        // setIDTentry(0x00,cSeg,&handleException0x00,0,type);
        // setIDTentry(0x01,cSeg,&handleException0x01,0,type);
        // setIDTentry(0x02,cSeg,&handleException0x02,0,type);
        // setIDTentry(0x03,cSeg,&handleException0x03,0,type);
        // setIDTentry(0x04,cSeg,&handleException0x04,0,type);
        // setIDTentry(0x05,cSeg,&handleException0x05,0,type);
        // setIDTentry(0x06,cSeg,&handleException0x06,0,type);
        // setIDTentry(0x07,cSeg,&handleException0x07,0,type);
        // setIDTentry(0x08,cSeg,&handleException0x08,0,type);
        // setIDTentry(0x09,cSeg,&handleException0x09,0,type);
        // setIDTentry(0x0a,cSeg,&handleException0x0A,0,type);
        // setIDTentry(0x0b,cSeg,&handleException0x0B,0,type);
        // setIDTentry(0x0c,cSeg,&handleException0x0C,0,type);
        // setIDTentry(0x0d,cSeg,&handleException0x0D,0,type);
        // setIDTentry(0x0e,cSeg,&handleException0x0E,0,type);
        // setIDTentry(0x0f,cSeg,&handleException0x0F,0,type);

        setIDTentry(0x20,cSeg,&handelInterruptRequest0x00,0,type);
        setIDTentry(0x20+0x01,cSeg,&handelInterruptRequest0x01,0,type);
        setIDTentry(0x20+0x02,cSeg,&handelInterruptRequest0x02,0,type);
        setIDTentry(0x20+0x03,cSeg,&handelInterruptRequest0x03,0,type);
        setIDTentry(0x20+0x04,cSeg,&handelInterruptRequest0x04,0,type);
        setIDTentry(0x20+0x05,cSeg,&handelInterruptRequest0x05,0,type);
        setIDTentry(0x20+0x06,cSeg,&handelInterruptRequest0x06,0,type);
        setIDTentry(0x20+0x07,cSeg,&handelInterruptRequest0x07,0,type);
        setIDTentry(0x20+0x08,cSeg,&handelInterruptRequest0x08,0,type);
        setIDTentry(0x20+0x09,cSeg,&handelInterruptRequest0x09,0,type);
        setIDTentry(0x20+0x0a,cSeg,&handelInterruptRequest0x0A,0,type);
        setIDTentry(0x20+0x0b,cSeg,&handelInterruptRequest0x0B,0,type);
        setIDTentry(0x20+0x0c,cSeg,&handelInterruptRequest0x0C,0,type);
        setIDTentry(0x20+0x0d,cSeg,&handelInterruptRequest0x0D,0,type);
        setIDTentry(0x20+0x0e,cSeg,&handelInterruptRequest0x0E,0,type);
        setIDTentry(0x20+0x0f,cSeg,&handelInterruptRequest0x0F,0,type);
        setIDTentry(0x80,cSeg,&handelInterruptRequest0x80,0,type);
       
        
        picMasterCMD.write(0x11);
        picSlaveCMD.write(0x11);
        picMasterData.write(0x20);
        picSlaveData.write(0x28);

        picMasterData.write(0x04);
        picSlaveData.write(0x02);

        picMasterData.write(0x01);
        picSlaveData.write(0x01);

        picMasterData.write(0x00);
        picSlaveData.write(0x00);

        IDTptr idtPtr;
        idtPtr.sz=256*sizeof(GateDescriptor)-1;
        idtPtr.IDT=(uint32_t)idt;
        asm volatile("lidt %0" : : "m" (idtPtr));
}
InterruptManager::~InterruptManager(){

}
void InterruptManager::activate(){
    if(instance!=0)
        instance->deactivate();
    instance=this;
    asm("sti");
};

void InterruptManager::deactivate(){
    if(instance==this)
     {   
        instance=0;
        asm("cli");
     }
};
uint32_t InterruptManager::handelInterrupt(uint8_t int_n,uint32_t esp){
  if(instance!=0){
    return instance->handel(int_n,esp);
  } 
    return  esp;
}
uint32_t InterruptManager::handel(uint8_t int_n,uint32_t esp){
    if(handlers[int_n]!=0){   
        esp=handlers[int_n]->handel(esp);
    }else if(int_n!=0x20){
        printf("UNHANDLED INTERRUPT 0x");
        printfHex(int_n);
        printf("\n");
    }
    // multitasking
    if(int_n == 0x20){
        // printf("Scheduled");    
        esp =(uint32_t) taskMgr->schedule(gdt,(CPUState*)esp);
    }
    if(int_n>=0x20&&int_n<0x20+16){
        picMasterCMD.write(0x20);
        if(int_n>=0x28){
        picSlaveCMD.write(0x20);
     }
    }
    return  esp;
}