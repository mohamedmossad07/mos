#ifndef _MOS_gui_window
#define _MOS_gui_window
#include <common/types.h>
#include <gui/widget.h>
#include <gui/CompositeWidget.h>
using namespace mos::drivers;
using namespace mos::common;
using namespace mos::gui;
namespace mos{
    namespace gui{
        class Window:public CompositeWidget{
            protected:
                bool dragging;
            public:
                Window(Widget * parent,int32_t x,int32_t y,int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b);
                ~Window();
                virtual void onMouseDown(int32_t x,int32_t y,uint8_t btn);
                virtual void onMouseUp(int32_t x,int32_t y,uint8_t btn);
                virtual void onMouseMove(int32_t oldx,int32_t oldy,int32_t newx,int32_t newy);
               
        };
    }
    }
#endif        