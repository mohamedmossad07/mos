#ifndef _MOS_DRIVERS_KEYBOARD_H
#define _MOS_DRIVERS_KEYBOARD_H
#include <drivers/keyboardEventHandler.h>
#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>
using namespace mos::common;
using namespace mos::hardware;
using namespace mos::drivers;
namespace mos{
    namespace drivers{
            class Keyboard : public InterruptHandler,public mos::drivers::Driver {
                port8BitSlow dataPort;
                port8BitSlow CMDPort;
                KeyboardEventHandler * handler;
                public:
                    // Keyboard(InterruptManager * int_mg);
                    Keyboard(InterruptManager * int_mg,KeyboardEventHandler *);
                    ~Keyboard();
                    virtual uint32_t handel(uint32_t esp);
                    virtual void install();
            };
    };
};

#endif