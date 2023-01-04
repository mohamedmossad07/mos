#include <drivers/ATA.h>

using namespace mos::drivers;
// void printfHex(int8_t hex);
void printfHex(int16_t hex);
void printf(int8_t *str);
ATA::ATA(uint16_t portBase,bool master)
:dataPort(portBase)
,errorPort(portBase+1)
,sectorCountPort(portBase+2)
,lbaLowPort(portBase+3)
,lbaMidPort(portBase+4)
,lbaHiPort(portBase+5)
,devicePort(portBase+6)
,commandPort(portBase+7)
,controlPort(portBase+0x206)
{
bytePSec=512;
this->master=master;
}
ATA::~ATA(){

}
void ATA::identify(){
     printfHex(sectorCountPort.read());printf("\n");
     printfHex(lbaHiPort.read());printf("\n");
     printfHex(lbaMidPort.read());printf("\n");
     printfHex(lbaLowPort.read());

}
void ATA::read28(uint32_t sec,uint8_t* data,int count){
    if(sec&0xf0000000)return;
    if(count>bytePSec)return
    devicePort.write((master?0xe0:0xf0)| ((sec & 0x0F000000) >> 24));
    errorPort.write(0);
    sectorCountPort.write(1);

    lbaLowPort.write(sec&0x000000FF);
    lbaMidPort.write(( sec& 0x0000FF00) >> 8);
    lbaHiPort.write( (sec & 0x00FF0000) >> 16);
    commandPort.write(0x20);
    uint8_t status=commandPort.read();
    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = commandPort.read();
    if(status&0x01){
        printf("Error in reading ATA driver.\n");
        return;
    }
    printf("Reading to ATA Drive: ");
    for(int i = 0; i < count; i += 2)
    {
      uint16_t wdata = dataPort.read();
        
        char *text = "  \0";
        text[0] = wdata & 0xFF;
        
        if(i+1 < count)
            text[1] = (wdata >> 8) & 0xFF;
        else
            text[1] = '\0';
        
        printf(text);
    }
    for(int i = count + (count%2); i < 512; i += 2)
            dataPort.read(); 
}
void ATA::write28(uint32_t sec,uint8_t* data,int count){
    
    if(sec&0xf0000000)return;
    if(count>bytePSec)return
    devicePort.write((master?0xe0:0xf0)| ((sec & 0x0F000000) >> 24));
    errorPort.write(0);
    sectorCountPort.write(1);

    lbaLowPort.write(sec&0x000000FF);
    lbaMidPort.write(( sec& 0x0000FF00) >> 8);
    lbaHiPort.write( (sec & 0x00FF0000) >> 16);
    commandPort.write(0x30);
    printf("Writing to ATA Drive: ");
    for(int i = 0; i < count; i += 2)
    {
        uint16_t wdata = data[i];
        if(i+1 < count)
            wdata |= ((uint16_t)data[i+1]) << 8;
        dataPort.write(wdata);
        
        char *text = "  \0";
        text[0] = (wdata >> 8) & 0xFF;
        text[1] = wdata & 0xFF;
        printf(text);
    }
    for(int i = count + (count%2); i < 512; i += 2)
            dataPort.write(0x0000); 
}
void ATA::flush(){
  devicePort.write((master?0xe0:0xf0));
    commandPort.write(0xe7);
    uint8_t status=commandPort.read();
    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = commandPort.read();
    if(status&0x01){
        printf("Error in flush ATA driver.\n");
        return;
    }
}
/**
 * void ATA::identify(){
    devicePort.write(master?0xA0:0xB0);
    controlPort.write(0);
    devicePort.write(0xa0);
    uint16_t status=commandPort.read();
    if(status==0xff)return;
    devicePort.write(master?0xa0:0xb0);
    sectorCountPort.write(0);
    lbaLowPort.write(0);
    lbaMidPort.write(0);
    lbaHiPort.write(0);
    commandPort.write(0xec);
    status=commandPort.read();
    if(status==0x00)return;
    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = commandPort.read();
    if(status&0x01){
        printf("Error in identifying ATA driver.\n");
        return;
    }
    for(uint16_t i=0;i<256;i++){
        uint16_t data=dataPort.read();
        char* f="  \0";
        f[1]=(data>>8)&0x00ff;
        f[0]=data&0x00ff;
        printf(f);
    }
}
*/