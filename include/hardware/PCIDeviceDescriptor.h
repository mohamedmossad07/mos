#ifndef _MOS_HARDWARE_PCI_DEVICE_
#define _MOS_HARDWARE_PCI_DEVICE_
#include <common/types.h>
using namespace mos::common;
namespace mos{
    namespace hardware{
        class PCIDeviceDescriptor{
            public:
                uint32_t portBase;
                uint32_t interrupt;
                uint8_t bus;
                uint8_t device;
                uint8_t function;

                uint16_t  vendor_id;
                uint16_t  device_id;
                
                uint8_t  class_id;
                uint8_t  subclass_id;
                uint8_t  interface_id;

                uint8_t  revision;
                PCIDeviceDescriptor();
                ~PCIDeviceDescriptor();
        };
    }
}    
#endif