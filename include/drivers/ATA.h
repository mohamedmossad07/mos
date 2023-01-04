#ifndef _MOS_DRIVERS_ATA
#define _MOS_DRIVERS_ATA 
#include <common/types.h>
#include <hardware/port.h>
using namespace mos::common; 
using namespace mos::hardware; 
namespace mos{
    namespace drivers{
        class ATA{
            port16Bit dataPort;
            port8Bit errorPort;
            port8Bit sectorCountPort;
            port8Bit lbaLowPort;
            port8Bit lbaMidPort;
            port8Bit lbaHiPort;
            port8Bit devicePort;
            port8Bit commandPort;
            port8Bit controlPort;
            bool master;
            uint16_t bytePSec;//bytes per sector 512
            public:
                ATA(uint16_t,bool);
                ~ATA();
                void identify();
                void read28(uint32_t sec,uint8_t* data,int count);
                void write28(uint32_t sec,uint8_t* data,int count);
                void flush();
        };
    }
}
#endif