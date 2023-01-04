#ifndef _MOS_DRIVERS_DRIVER_MANAGER_H
#define _MOS_DRIVERS_DRIVER_MANAGER_H
#include <drivers/driver.h>
#include <common/types.h>
using namespace mos::common;
using namespace mos::drivers;
namespace mos{
    namespace drivers{
        class DriverManager{
            private:    
                uint16_t numDrivers;
            public:
                Driver * drivers[256];
                DriverManager();
                void addDriver(Driver*);
                void installAll();
        };
    };
};
#endif