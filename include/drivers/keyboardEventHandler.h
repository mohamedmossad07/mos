#ifndef _MOS_DRIVERS_EVENT_KEYBOARD_HANDLER_
#define _MOS_DRIVERS_EVENT_KEYBOARD_HANDLER_
namespace mos{
    namespace drivers{
        class KeyboardEventHandler{
        public:
            KeyboardEventHandler();
            virtual void onKeyDown(char);
            virtual void onKeyUp(char);
        } ;
    } ;
} ;
#endif