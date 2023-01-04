#include <drivers/AMDNetCard.h>
using namespace mos::drivers;

void printf(char *);
void printfHex(int8_t hex);
AMDNetCard::AMDNetCard(PCIDeviceDescriptor *dev, InterruptManager *im) : Driver(),
                                                                         InterruptHandler(0x20 + dev->interrupt, im),
                                                                         MACAddress0Port(dev->portBase),
                                                                         MACAddress2Port(dev->portBase + 0x02),
                                                                         MACAddress4Port(dev->portBase + 0x04),
                                                                         registerDataPort(dev->portBase + 0x10),
                                                                         registerAddressPort(dev->portBase + 0x12),
                                                                         resetPort(dev->portBase + 0x14),
                                                                         busControlRegisterDataPort(dev->portBase + 0x16),
                                                                         currentRcvBuffer(0),
                                                                         currentSendBuffer(0)
{
    this->handler = 0;
    uint64_t MAC0 = MACAddress0Port.read() % 256;
    uint64_t MAC1 = MACAddress0Port.read() / 256;
    uint64_t MAC2 = MACAddress2Port.read() % 256;
    uint64_t MAC3 = MACAddress2Port.read() / 256;
    uint64_t MAC4 = MACAddress4Port.read() % 256;
    uint64_t MAC5 = MACAddress4Port.read() / 256;

    uint64_t MAC = MAC5 << 40 | MAC4 << 32 | MAC3 << 24 | MAC2 << 16 | MAC1 << 8 | MAC0;
    registerAddressPort.write(20);
    busControlRegisterDataPort.write(0x102);
    registerAddressPort.write(0);
    registerDataPort.write(0x04);
    initBlock.mode = 0x0000;
    initBlock.reserved1 = 0;
    initBlock.transfer_length = 3; // this accepts number of bits; this means 8 buffers
    initBlock.reserved2 = 0;
    initBlock.receive_length = 3;
    initBlock.physical_address = MAC;
    initBlock.reserved3 = 0;
    initBlock.logical_address = 0;

    sendBufferDesc = (BufferDescriptor *)((((uint32_t)&sendBufferDescMem[0]) + 15) & ~((uint32_t)0xF));
    initBlock.transmit_descriptor = (uint32_t)sendBufferDesc;

    rcvBufferDesc = (BufferDescriptor *)((((uint32_t)&rcvBufferDescMem[0]) + 15) & ~((uint32_t)0xF));
    initBlock.receive_descriptor = (uint32_t)rcvBufferDesc;

    for (uint8_t i = 0; i < 8; i++)
    {
        sendBufferDesc[i].addr = (((uint32_t)&sendBuffer[i]) + 15) & ~(uint32_t)0xF;
        sendBufferDesc[i].flags = 0x7FF | 0xF000;
        sendBufferDesc[i].flags2 = 0;
        sendBufferDesc[i].avail = 0;

        rcvBufferDesc[i].addr = (((uint32_t)&rcvBuffer[i]) + 15) & ~(uint32_t)0xF;
        rcvBufferDesc[i].flags = 0xF7FF | 0x80000000;
        rcvBufferDesc[i].flags2 = 0;
        sendBufferDesc[i].avail = 0;
    }
    registerAddressPort.write(1);
    registerDataPort.write((uint32_t)(&initBlock) & 0xFFFF);
    registerAddressPort.write(2);
    registerDataPort.write(((uint32_t)(&initBlock) >> 16) & 0xFFFF);
}
AMDNetCard::~AMDNetCard() {}

void AMDNetCard::install()
{
    registerAddressPort.write(0);
    registerDataPort.write(0x41);

    registerAddressPort.write(4);
    uint32_t temp = registerDataPort.read();
    registerAddressPort.write(4);
    registerDataPort.write(temp | 0xC00);

    registerAddressPort.write(0);
    registerDataPort.write(0x42);
}
int32_t AMDNetCard::reset()
{
    resetPort.read();
    resetPort.write(0);
    return 10;
}
uint32_t AMDNetCard::handel(uint32_t esp)
{
    registerAddressPort.write(0);
    uint32_t temp = registerDataPort.read();

    if ((temp & 0x8000) == 0x8000)
        printf("AMD am79c973 ERROR\n");
    if ((temp & 0x2000) == 0x2000)
        printf("AMD am79c973 COLLISION ERROR\n");
    if ((temp & 0x1000) == 0x1000)
        printf("AMD am79c973 MISSED FRAME\n");
    if ((temp & 0x0800) == 0x0800)
        printf("AMD am79c973 MEMORY ERROR\n");
    if ((temp & 0x0400) == 0x0400)
        receive();
    // if ((temp & 0x0200) == 0x0200)
        // printf(" SENT\n");
    registerAddressPort.write(0);
    registerDataPort.write(temp);
    // if ((temp & 0x0100) == 0x0100)
    //     printf("AMD NETCARD INIT DONE ");

    return esp;
}
void AMDNetCard::send(uint8_t *buffer, int count)
{
    int sndDesc = currentSendBuffer;
    currentSendBuffer = (currentSendBuffer + 1) % 8;
    if (count > 1518)
        count = 1518;
    for (uint8_t *src = buffer + count - 1,
                 *dst = (uint8_t *)(sendBufferDesc[sndDesc].addr + count - 1);
         src >= buffer;
         src--, dst--)
        *dst = *src;
    printf("Sending data :\n");
    for (int i = 34; i < count; i++)
    {
        printfHex(buffer[i]);
        printf(" ");
    }
    printf("\n");
    sendBufferDesc[sndDesc].avail = 0;
    sendBufferDesc[sndDesc].flags2 = 0;
    sendBufferDesc[sndDesc].flags = 0x8300f000 | ((uint16_t)((-count) & 0xfff));
    registerAddressPort.write(0);
    registerDataPort.write(0x48);
}
void AMDNetCard::receive()
{
    // printf("AMD am79c973 DATA RECEIVED\n");
    for (; (rcvBufferDesc[currentRcvBuffer].flags & 0x80000000) == 0;
         currentRcvBuffer = (currentRcvBuffer + 1) % 8)
    {
        if (!(rcvBufferDesc[currentRcvBuffer].flags & 0x40000000) && (rcvBufferDesc[currentRcvBuffer].flags & 0x03000000) == 0x03000000)
        {
            uint32_t size = rcvBufferDesc[currentRcvBuffer].flags & 0xfff;
            if (size > 64)
            {
                size -= 4;
            }
            uint8_t *buffer = (uint8_t *)(rcvBufferDesc[currentRcvBuffer].addr);
            printf("Received data :\n");
            for (int i = 34; i < 64; i++)
            {
                printfHex(buffer[i]);
                printf(" ");
            }
            printf("\n");
            if (handler != 0)
            {
                if (handler->onRawDataReceived(buffer, size))
                {
                    send(buffer, size);
                }
            }
        }
        rcvBufferDesc[currentRcvBuffer].flags2 = 0;
        rcvBufferDesc[currentRcvBuffer].flags = 0xF7FF | 0x80000000;
    }
}
void AMDNetCard::setHandler(RawDataHandler *_handler)
{
    this->handler = _handler;
}
uint64_t AMDNetCard::getMACAddress()
{
    return initBlock.physical_address;
}
uint32_t AMDNetCard::getMyIp()
{
    return initBlock.logical_address;
}
void AMDNetCard::setMyIp(uint32_t ip)
{
    initBlock.logical_address = ip;
}

RawDataHandler::RawDataHandler(AMDNetCard *back) : backend(back)
{
    backend->setHandler(this);
}
RawDataHandler::~RawDataHandler()
{
    backend->setHandler(0);
}
bool RawDataHandler::onRawDataReceived(uint8_t *buffer, uint32_t size)
{
    return false;
}
void RawDataHandler::send(uint8_t *buffer, uint32_t size)
{
    backend->send(buffer, size);
}