#ifndef _MOS_DRIVERS_MOUSE_H
#define _MOS_DRIVERS_MOUSE_H
#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <drivers/mouseEventHandler.h>
#include <hardware/port.h>
using namespace mos::common;
using namespace mos::hardware;
using namespace mos::drivers;
namespace mos{
    namespace drivers{
        class Mouse : public InterruptHandler,public mos::drivers::Driver {
            port8Bit dataPort;
            port8Bit CMDPort;
            uint8_t buffer[3];
            uint8_t offset;
            uint8_t buttons;
            MouseEventHandler * handler;
            public:
                // Mouse(InterruptManager * int_mg);
                Mouse(InterruptManager * int_mg,MouseEventHandler * handler);
                ~Mouse();
                virtual uint32_t handel(uint32_t esp);
                virtual void install();

        };
    };
};

#endif