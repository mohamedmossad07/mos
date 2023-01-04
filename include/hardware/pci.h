#ifndef _MOS_HARDWARE_PCI
#define _MOS_HARDWARE_PCI
#include <hardware/port.h>
#include <hardware/PCIDeviceDescriptor.h>
#include <hardware/interrupts.h>
#include <common/types.h>
#include <drivers/driverManager.h>
#include <hardware/BAR.h>
using namespace mos::hardware;
using namespace mos::common;
using namespace mos::drivers;
namespace mos{
    namespace hardware{
       
        class PCI{
            port32Bit CMDPort;
            port32Bit dataPort;
            public:
                PCI();
                ~PCI();
                uint32_t read(uint8_t bus,uint8_t device,uint8_t function,uint8_t offset );
                void write(uint8_t bus,uint8_t device,uint8_t function,uint8_t offset,uint32_t data );
                bool isDeviceHasFunc(uint8_t bus,uint8_t device);
                void selectDrivers(DriverManager *,InterruptManager * );
                PCIDeviceDescriptor  getDescriptor(uint8_t bus,uint8_t device,uint8_t function);
                BAR getBar(uint8_t bus,uint8_t device,uint8_t function,uint16_t bar);
                Driver * getDriver(PCIDeviceDescriptor,InterruptManager *);
        };
    }
}
#endif