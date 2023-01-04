#ifndef _mos_syscall
#define _mos_syscall
#include <common/types.h>
#include <hardware/interrupts.h>
#include <multitasking.h>
using namespace mos;
using namespace mos::common;
namespace mos{
     class SysCallHandler:public InterruptHandler{
        public:
            SysCallHandler(uint8_t int_n,InterruptManager * int_mg);
            ~SysCallHandler();
            virtual uint32_t handel(uint32_t esp);
     };
}
#endif