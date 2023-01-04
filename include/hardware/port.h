#ifndef _MOS_HARDWARE_PORT_H
#define _MOS_HARDWARE_PORT_H
#include <common/types.h>
using namespace mos::common;
namespace mos{
        namespace hardware{
                class port{
                protected:
                        port(uint16_t pn);
                        ~port();
                        uint16_t portnumber;//this means we can set only
                        // 16-bit ports ~= 65k port
                };

                class port8Bit:public  port {
                public:
                        port8Bit(uint16_t pn);
                        ~port8Bit();
                        virtual uint8_t read();
                        virtual void write(uint8_t data);
                };

                class port8BitSlow:public port8Bit{
                public:
                        port8BitSlow(uint16_t pn);
                        ~port8BitSlow();
                        virtual void write(uint8_t data);
                };

                class port16Bit:port{
                public:
                        port16Bit(uint16_t pn);
                        ~port16Bit();
                        virtual uint16_t read();
                        virtual void write(uint16_t data);
                };
                class port32Bit:port{
                public:
                        port32Bit(uint16_t pn);
                        ~port32Bit();
                        virtual uint32_t read();
                        virtual void write(uint32_t data);
                };
        };
}; 
#endif