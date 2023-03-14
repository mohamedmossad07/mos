#include <hardware/pci.h>
#include <drivers/AMDNetCard.h>
#include <MemoryManager.h>
using namespace mos;
void printf(int8_t *str);
void printfHex(int16_t hex);

PCI::PCI():
CMDPort(0xCF8),
dataPort(0XCFC)
{
    printf("Initializing PCI...\n");
}
PCI::~PCI(){}

uint32_t PCI::read(uint8_t bus,uint8_t device,uint8_t function,uint8_t offset ){
    uint32_t id= (0x1<<31)|((bus&0xff)<<16)|((device&0x1f)<<11)|((function&0x07)<<8)|(offset&0xfc);
    CMDPort.write(id);
    uint32_t res=dataPort.read();
     return res>> (8*(offset%4));
};
void PCI::write(uint8_t bus,uint8_t device,uint8_t function,uint8_t offset,uint32_t data ){
    uint32_t id= (0x1<<31)|((bus&0xff)<<16)|((device&0x1f)<<11)|((function&0x07)<<8)|(offset&0xfc);
    CMDPort.write(id);
    dataPort.write(data);
}   
bool PCI::isDeviceHasFunc(uint8_t bus,uint8_t device){
    
    return read(bus,device,0,0x0E)&(1<<7);
}
void PCI::selectDrivers(DriverManager * dm,InterruptManager * im){
    for(uint8_t bus=0;bus<8;bus++){
         for(uint8_t device=0;device<32;device++){
            uint8_t nFunc=isDeviceHasFunc(bus,device)?8:1;
             for(uint8_t function =0;function<nFunc;function++){
                    PCIDeviceDescriptor dev=getDescriptor(bus,device,function);
                    if(dev.vendor_id==0x0000||dev.vendor_id==0xffff)continue;   
                  
                    for(uint8_t barNum=0;barNum<6;barNum++){
                        BAR bar = getBar(bus,device,function,barNum);
                        if(bar.address&&(bar.bt==BARType::IO)){
                                dev.portBase=(uint32_t) bar.address;
                        }
                    }
                    Driver * driver=getDriver(dev,im);
                        if(driver!=0){ 
                            dm->addDriver(driver);
                        }
                    // printf("PCI BUS "); 
                    // printfHex(bus&0xff);
                    // printf(" PCI DEVICE ");
                    // printfHex(device&0xff);
                    // printf(" PCI FUNCTION ");
                    // printfHex(function&0xff);
                    // printf("\n");
             }
        }
          
  }
}
PCIDeviceDescriptor  PCI::getDescriptor(uint8_t bus,uint8_t device,uint8_t function){
    PCIDeviceDescriptor res;
    res.bus=bus;
    res.device=device;
    res.function=function;

    res.vendor_id=read(bus,device,function,0x00);
    res.device_id=read(bus,device,function,0x02);
    res.class_id=read(bus,device,function,0x0b);
    res.subclass_id=read(bus,device,function,0x0a);
    res.interface_id=read(bus,device,function,0x09);
    res.revision=read(bus,device,function,0x08);
    res.interrupt=read(bus,device,function,0x3c);
    return res;
}
Driver* PCI::getDriver(PCIDeviceDescriptor dev,InterruptManager * im){
    Driver* driver = 0;
    switch(dev.vendor_id)
    {
        case 0x1022: // AMD
            switch(dev.device_id)
            {
                case 0x2000: // am79c973
                    // printf("AMD am79c973 \n"); 
                    // printf("AMD am79c973 ");
                driver = (AMDNetCard*)MemoryManager::activeMemMgr->malloc(sizeof(AMDNetCard));
                if(driver != 0)
                    new (driver) AMDNetCard(&dev, im);
                else
                    printf("instantiation failed"); 
                return driver;
                break;
            }
            break;

        case 0x8086: // Intel
            break;
    }
    
    
    switch(dev.class_id)
    {  
        case 0x03: // graphics
            switch(dev.subclass_id)
            {
                case 0x00: // VGA
                    // printf("VGA \n");
                    break;
            }
            break;
    }
    return driver;
}
BAR PCI::getBar(uint8_t bus,uint8_t device,uint8_t function,uint16_t bar){
    BAR res;
    uint32_t headerType=read(bus,device,function,0x0e)&0x7f;
    uint8_t maxBar=6-(4*headerType);
    if(bar>=maxBar){
        return res;
    }
    uint32_t bar_v=read(bus,device,function,0x10+4*bar);
    res.bt=(bar_v&0x01)?BARType::IO:BARType::MM;
    uint32_t tmp;
    if(res.bt==BARType::MM){
        // res.preFetchable=((bar_v>>3)&0x1)==0x1;
        switch((bar_v>>1)&0x3){
            // case 0:
            // case 1:
            // case 2:
        }
    } else{
        res.address=(uint8_t *)(bar_v&~0x3);
        res.preFetchable=false;
    }
    return res;
}