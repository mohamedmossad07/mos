#ifndef _MOS_HARDWARE_BAR
#define _MOS_HARDWARE_BAR
#include <common/types.h>
using namespace mos::common;
namespace mos{
    namespace hardware{
          // types of baseAddr
        enum BARType{
            MM=0,
            IO=1
        };

        // BAR
        class BAR{
            public:
                bool preFetchable;
                uint8_t * address;
                uint32_t * size;
                BARType bt; 
        };
    }
 }
#endif