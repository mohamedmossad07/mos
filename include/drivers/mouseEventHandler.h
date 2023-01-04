#ifndef _MOS_DRIVERS_MOUSE_EVENT_HANDLER_
#define _MOS_DRIVERS_MOUSE_EVENT_HANDLER_
#include <common/types.h>
using namespace mos::common;
namespace mos{
    namespace drivers{
        class MouseEventHandler{
        public:
            MouseEventHandler();
            virtual void onActivate();
            virtual void onMouseDown(uint8_t btn);
            virtual void onMouseMove(int x,int y);
            virtual void onMouseUp(uint8_t btn);
        } ;
    } ;
} ;
#endif