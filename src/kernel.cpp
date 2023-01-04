#include <common/types.h>
#include <gdt.h>
#include <multitasking.h>
#include <syscall.h>
#include <MemoryManager.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/driverManager.h>
#include <drivers/Keyboard.h>
#include <drivers/keyboardEventHandler.h>
#include <drivers/Mouse.h>
#include <drivers/ATA.h>
#include <drivers/AMDNetCard.h>
#include <drivers/mouseEventHandler.h>
#include <common/helper.h>
#include <drivers/vga.h>
#include <common/helper.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <net/DLLayer/etherframe.h>
#include <net/NetLayer/arp.h>
#include <net/NetLayer/ip4.h>
#include <net/NetLayer/icmp.h>
#include <net/TPLayer/udp.h>
#include <net/TPLayer/tcp.h>
using namespace mos;
using namespace mos::hardware;
using namespace mos::drivers;
using namespace mos::common;
using namespace mos::gui;
using namespace mos::net::DLLayer;
using namespace mos::net::NetLayer;
using namespace mos::net::TPLayer;
// #define GRAPHICSMODE
void printf(int8_t *str)
{
    // writing to graphic card directly which its memory stars at
    // location 0xb8000
    static uint16_t *VideoMemory = (uint16_t *)0xb8000;
    static uint8_t x = 0, y = 0;
    for (int32_t i = 0; str[i] != '\0'; i++)
    {
        if (y >= 25)
        {
            clearScreen();
            y = 0;
            x = 0;
        }
        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;
        default:
            // copy the old color and zeros the lowest byte and ors the char with it
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xff00) | str[i];
            x++;
            break;
        }
        if (x >= 80)
        {
            y++;
            x = 0;
        }
    }
}
void printfHex(int8_t hex)
{
    char *foo = "00";
    char *h = "0123456789ABCDEF";
    foo[0] = h[(hex >> 4) & 0x0f];
    foo[1] = h[hex & 0x0f];
    printf(foo);
}
void printfHex(int16_t hex)
{
    char *foo = "0000";
    char *h = "0123456789ABCDEF";
    foo[3] = h[hex & 0x0f];
    foo[2] = h[(hex >> 4) & 0x0f];
    foo[1] = h[(hex >> 8) & 0x0f];
    foo[0] = h[(hex >> 12) & 0x0f];
    printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void onKeyDown(char c)
    {
        char *foo = " ";
        foo[0] = c;
        printf(foo);
    }
};
void sysprintf(char *str)
{
    asm("int $0x80"
        :
        : "a"(4), "b"(str));
}
void taskA()
{
    while (true)
        sysprintf("A");
}

void taskB()
{
    while (true)
        sysprintf("B");
}

class PrintfUDPHandler : public TCPHandler
{
public:
    void handleDatagram(UDPSocket* socket, uint8_t* data,uint16_t size)
    {
        char* foo = " ";
        for(int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
    }
};

class PrintfTCPHandler : public TCPHandler
{
public:
    bool HandleTransmissionControlProtocolMessage(TCPSocket* socket, uint8_t* data,uint16_t size)
    {
        char* foo = " ";
        for(int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
         
        // if(size > 9
        //     && data[0] == 'G'
        //     && data[1] == 'E'
        //     && data[2] == 'T'
        //     && data[3] == ' '
        //     && data[4] == '/'
        //     && data[5] == ' '
        //     && data[6] == 'H'
        //     && data[7] == 'T'
        //     && data[8] == 'T'
        //     && data[9] == 'P'
        // )
        // {
        //     socket->send((uint8_t*)"HTTP/1.1 200 OK\r\nServer: MyOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>My Operating System</title></head><body><b>My Operating System</b> http://www.AlgorithMan.de</body></html>\r\n",184);
        //     socket->disconnect();
        // }
        
        
        return true;
    }
};

extern "C" void Main(void *MB_structure, uint32_t magic_num)
{
    printf("Initializing hardware..\n");
    GlobalDescriptorTable gdt;
    size_t heap = 10 * 1024 * 1024;
    size_t s = (((size_t)MB_structure) + 8) * 1024 - heap - 1024 * 10;
    MemoryManager memMgr(heap, (((size_t)MB_structure) + 8) * 1024 - heap - 1024 * 10);
    // printf("heap: 0x");
    // printfHex((heap >> 24) & 0xFF);
    // printfHex((heap >> 16) & 0xFF);
    // printfHex((heap >> 8 ) & 0xFF);
    // printfHex((heap      ) & 0xFF);

    // void* allocated = memMgr.malloc(4033);
    // printf("\nallocated: 0x");
    // printfHex(((size_t)allocated >> 24) & 0xFF);
    // printfHex(((size_t)allocated >> 16) & 0xFF);
    // printfHex(((size_t)allocated >> 8) & 0xFF);
    // printfHex(((size_t)allocated      ) & 0xFF);
    // printf("\n");

    TaskManager tm;
    // Task task1(&gdt,(uint32_t)taskA);
    // Task task2(&gdt,(uint32_t)taskB);
    // tm.addTask(&task1);
    // tm.addTask(&task2);

    InterruptManager intManager(&gdt, &tm);
    // SysCallHandler syscall(0x80+0x20,&intManager);

#ifdef GRAPHICSMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xa8);
    Keyboard kb(&intManager, &desktop);
#else
    PrintfKeyboardEventHandler kh;
    Keyboard kb(&intManager, &kh);
#endif
    DriverManager driverManager;
    driverManager.addDriver(&kb);
#ifdef GRAPHICSMODE
    Mouse mouse(&intManager, &desktop);
#else
    MouseEventHandler mh;
    Mouse mouse(&intManager, &mh);
#endif
    driverManager.addDriver(&mouse);
    PCI pci;
    pci.selectDrivers(&driverManager, &intManager);
#ifdef GRAPHICSMODE
    VGA vga;
    vga.setMode(320, 200, 8);
    desktop.draw(&vga);
    Window w1(&desktop, 10, 10, 20, 20, 0xa8, 0x00, 0x00);
    desktop.addChild(&w1);
    Window w2(&desktop, 40, 15, 30, 30, 0x00, 0xa8, 0x00);
    desktop.addChild(&w2);
#endif
    driverManager.installAll();
    // IP Address
    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
    // IP Address of the default gateway
    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t ip_be = ((uint32_t)ip4 << 24)
                | ((uint32_t)ip3 << 16)
                | ((uint32_t)ip2 << 8)
                | (uint32_t)ip1;
    uint32_t gip_be = ((uint32_t)gip4 << 24)
                   | ((uint32_t)gip3 << 16)
                   | ((uint32_t)gip2 << 8)
                   | (uint32_t)gip1;
    
    uint32_t subnet_be = ((uint32_t)subnet4 << 24)
                   | ((uint32_t)subnet3 << 16)
                   | ((uint32_t)subnet2 << 8)
                   | (uint32_t)subnet1;
                   
    AMDNetCard *e = (AMDNetCard *)(driverManager.drivers[2]);
    EtherFrameProvider eth(e);
    ARP arp(&eth);
    e->setMyIp(ip_be);
    IP4Provider ipv4(&eth,&arp,gip_be,subnet_be);
    // ICMP icmp(&ipv4);
    UDPProvider udp(&ipv4);
    TCPProvider tcp(&ipv4);
    
    // e->send((uint8_t*)"Hello",5);

    // printf("Primary Master ata \n");
    ATA ataMp(0x1f0,true);//master primary
    ataMp.identify();

    // printf("Primary Slave ata \n");
    // ATA ataSp(0x1f0,false);//slave primary  
    // ataSp.identify();

    // ATA ataMs(0x170,true);//master secondary
    // ataMs.identify();
    // ATA ataSs(0x170,false);//slave secondary
    // ataSs.identify();
    intManager.activate();
    // arp.resolve(gip_be);
    // ipv4.send(gip_be,0x01,(uint8_t*)"hello",5); 
    // arp.broadcastMyMac(gip_be); 
    // TCPSocket* socket= tcp.connect(gip_be,1234);
    // PrintfTCPHandler handler;
    // PrintfUDPHandler handler;
    // tcp.bind(socket,&handler);
    // socket->send((uint8_t*)"Hello tcp",9);
    // icmp.ping(gip_be);
    // UDPSocket* socket= udp.listen(1234);
    // PrintfUDPHandler handler;
    // udp.bind(socket,&handler);
    // socket->send((uint8_t*)"Hello",5);
    // UDPSocket* socket= udp.connect(gip_be,1234);
    // PrintfUDPHandler handler;
    // udp.bind(socket,&handler);
    // socket->send((uint8_t*)"Hello",5);

    while (1)
    {
#ifdef GRAPHICSMODE
        desktop.draw(&vga);
#endif
    }
}
