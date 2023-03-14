#include <drivers/Mouse.h>
/**
 * @brief Construct a new Mouse:: Mouse object
 * This is a Driver for Mouse ps/2
 * Interrupt number 0x21
 * This Mouse Controller accept commands on port
 * 0x60 and accept data on port 0x64
 */
using namespace mos::drivers;
void printf(char*);
Mouse::Mouse(InterruptManager * int_mg,MouseEventHandler * _handler):
InterruptHandler(0x2C,int_mg),
CMDPort(0x64),
dataPort(0x60),
handler(_handler)
    {
    printf("Initializing Mouse driver...\n");
}
Mouse::~Mouse(){}
void Mouse::install(){
     /**
     * @brief 
     * Initialize the Mouse 
     * 1) We should clear or empty the output buffer
     */
    offset=0;
    buttons=0;
  if(handler != 0)
        handler->onActivate();
    CMDPort.write(0xA8);//Mouse activation
    CMDPort.write(0x20);//get state
    uint8_t state = dataPort.read() | 2;
    CMDPort.write(0x60);
    dataPort.write(state);
    CMDPort.write(0xD4);
    // //enabling Mouse th start send us scan codes
    dataPort.write(0xF4);
    dataPort.read();
}
uint32_t Mouse::handel(uint32_t esp){
    uint8_t status = CMDPort.read();
    if(!(status & 0x20)){
        return esp;
    }
    static int8_t x=40,y=12;
    buffer[offset]=dataPort.read();
    offset=(offset+1)%3;
    if(offset==0){
        static uint16_t * VideoMemory = (uint16_t *)0xb8000;
        VideoMemory[80*y+x]=((VideoMemory[80*y+x]&0xf000)>>4)|
                            ((VideoMemory[80*y+x]&0x0f00)<<4)|
                            (VideoMemory[80*y+x]&0x00ff); 

        x += buffer[1];
        
        if(x<0)x=0;
        if(x >= 80) 
            x = 79;

        y -= buffer[2];
        if(y<0)y=0;
        if(y>=25)x=24;
        VideoMemory[80*y+x]=((VideoMemory[80*y+x] & 0xF000)>>4)|
                            ((VideoMemory[80*y+x]&0x0f00) << 4)|
                            (VideoMemory[80*y+x]&0x00ff); 
    }
     if(offset == 0)
        {
            if(buffer[1] != 0 || buffer[2] != 0)
            {
                handler->onMouseMove(buffer[1], -buffer[2]);
            }

            for(uint8_t i = 0; i < 3; i++)
            {
                if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
                {
                    if(buttons & (0x1<<i))
                        handler->onMouseUp(i+1);
                    else
                        handler->onMouseDown(i+1);
                }
            }
            buttons = buffer[0];
        }
    return esp;
}