#ifndef _MOS_DRIVERS_AMDNetCard
#define _MOS_DRIVERS_AMDNetCard
#include <drivers/driver.h>
#include <hardware/PCIDeviceDescriptor.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <common/types.h>
using namespace mos::common;
using namespace mos::drivers;
using namespace mos::hardware;
namespace mos
{
    namespace drivers
    {
        class AMDNetCard;
        class RawDataHandler
        {
        protected:
            AMDNetCard *backend;

        public:
            RawDataHandler(AMDNetCard *);
            ~RawDataHandler();
            virtual bool onRawDataReceived(uint8_t *buffer, uint32_t size);
            virtual void send(uint8_t *buffer, uint32_t size);
        };
        class AMDNetCard : public Driver, public InterruptHandler
        {
            struct BufferDescriptor
            {
                uint32_t addr;
                uint32_t flags;
                uint32_t flags2;
                uint32_t avail;
            } __attribute__((packed));

            struct InitBlock
            {
                uint16_t mode;
                unsigned reserved1 : 4;
                unsigned transfer_length : 4;
                unsigned reserved2 : 4;
                unsigned receive_length : 4;
                uint64_t physical_address : 48;
                uint16_t reserved3;
                uint64_t logical_address;
                uint32_t receive_descriptor;
                uint32_t transmit_descriptor;
            } __attribute__((packed));

            port16Bit MACAddress0Port;
            port16Bit MACAddress2Port;
            port16Bit MACAddress4Port;
            port16Bit registerDataPort;
            port16Bit registerAddressPort;
            port16Bit resetPort;
            port16Bit busControlRegisterDataPort;
            InitBlock initBlock;
            BufferDescriptor *rcvBufferDesc;
            uint8_t rcvBufferDescMem[2048 + 15];
            uint8_t rcvBuffer[2048 + 15][8];
            uint8_t currentRcvBuffer;
            BufferDescriptor *sendBufferDesc;
            uint8_t sendBufferDescMem[2048 + 15];
            uint8_t sendBuffer[2048 + 15][8];
            uint8_t currentSendBuffer;
            RawDataHandler *handler;

        public:
            AMDNetCard(PCIDeviceDescriptor *dev, InterruptManager *im);
            ~AMDNetCard();
            void install();
            int32_t reset();
            uint32_t handel(uint32_t esp);
            void send(uint8_t *buffer, int count);
            void receive();
            void setHandler(RawDataHandler *_handler);
            uint64_t getMACAddress();
            void setMyIp(uint32_t ip);
            uint32_t getMyIp();
        };
    }
}
#endif