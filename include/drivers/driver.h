#ifndef _MOS_DRIVERS_DRIVER_H
#define _MOS_DRIVERS_DRIVER_H
#include <common/types.h>
using namespace mos::common;
namespace mos{
    namespace drivers{

        class Driver{
            public:
                Driver();
                ~Driver();
                virtual void install();
                virtual int32_t reset();
                virtual void uninstall();
        };
    };
};
#endif