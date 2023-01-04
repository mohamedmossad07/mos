#include <drivers/Keyboard.h>
#include <hardware/interrupts.h>
#include <common/types.h>
#include <common/helper.h>
using namespace mos::drivers;
using namespace mos::hardware;
using namespace mos::common;
/**
 * @brief Construct a new Keyboard:: Keyboard object
 * This is a Driver for keyboard ps/2
 * Interrupt number 0x21
 * This KB Controller accept commands on port
 * 0x60 and accept data on port 0x64
 */
void printf(char*);
void printfHex(int8_t hex);
Keyboard::Keyboard(InterruptManager * int_mg,KeyboardEventHandler * handler):
InterruptHandler(0x21,int_mg),
CMDPort(0x64),
dataPort(0x60)
{ 
    this->handler=handler;
    
}
Keyboard::~Keyboard(){}

void Keyboard::install(){
   /**
     * @brief 
     * Initialize the keyboard 
     * 1) We should clear or empty the output buffer
     */
    // clear keyboard buffer
    while(CMDPort.read() & 0x1)
        dataPort.read();
    CMDPort.write(0xAE);//Keyboard activation
    CMDPort.write(0x20);//get state
    uint8_t state = (dataPort.read() | 1) & ~0x10;
    dataPort.write(0x60);
    dataPort.write(state);
    //enabling keyboard th start send us scan codes
    dataPort.write(0xf4);
}
uint32_t Keyboard::handel(uint32_t esp){
    uint8_t key = dataPort.read();
     if(handler == 0)
        return esp;
     if(key < 0x80)
    {
        switch(key)
        {
            case 0x02: handler->onKeyDown('1'); break;
            case 0x03: handler->onKeyDown('2'); break;
            case 0x04: handler->onKeyDown('3'); break;
            case 0x05: handler->onKeyDown('4'); break;
            case 0x06: handler->onKeyDown('5'); break;
            case 0x07: handler->onKeyDown('6'); break;
            case 0x08: handler->onKeyDown('7'); break;
            case 0x09: handler->onKeyDown('8'); break;
            case 0x0A: handler->onKeyDown('9'); break;
            case 0x0B: handler->onKeyDown('0'); break;
            case 0x10: handler->onKeyDown('q'); break;
            case 0x11: handler->onKeyDown('w'); break;
            case 0x12: handler->onKeyDown('e'); break;
            case 0x13: handler->onKeyDown('r'); break;
            case 0x14: handler->onKeyDown('t'); break;
            case 0x15: handler->onKeyDown('z'); break;
            case 0x16: handler->onKeyDown('u'); break;
            case 0x17: handler->onKeyDown('i'); break;
            case 0x18: handler->onKeyDown('o'); break;
            case 0x19: handler->onKeyDown('p'); break;
            case 0x1E: handler->onKeyDown('a'); break;
            case 0x1F: handler->onKeyDown('s'); break;
            case 0x20: handler->onKeyDown('d'); break;
            case 0x21: handler->onKeyDown('f'); break;
            case 0x22: handler->onKeyDown('g'); break;
            case 0x23: handler->onKeyDown('h'); break;
            case 0x24: handler->onKeyDown('j'); break;
            case 0x25: handler->onKeyDown('k'); break;
            case 0x26: handler->onKeyDown('l'); break;
            case 0x2C: handler->onKeyDown('y'); break;
            case 0x2D: handler->onKeyDown('x'); break;
            case 0x2E: handler->onKeyDown('c'); break;
            case 0x2F: handler->onKeyDown('v'); break;
            case 0x30: handler->onKeyDown('b'); break;
            case 0x31: handler->onKeyDown('n'); break;
            case 0x32: handler->onKeyDown('m'); break;
            case 0x33: handler->onKeyDown(','); break;
            case 0x34: handler->onKeyDown('.'); break;
            case 0x35: handler->onKeyDown('-'); break;
            case 0x1C: handler->onKeyDown('\n'); break;
            case 0x39: handler->onKeyDown(' '); break;

            default:
            {
                printf("KEYBOARD 0x");
                printfHex(key);
                break;
            }
        }
     }    


    // if(key<0x80){
    //     char * msg="KEYBOARD 0X00 ";
    //     char * hex="0123456789ABCDEF";
    //     msg[11]=hex[(key>>4)&0x0f] ;
    //     msg[12]=hex[key&0x0f];
    //     printf(msg);
    // }
    return esp;
}