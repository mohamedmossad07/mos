#ifndef _MOS_gui_widget
#define _MOS_gui_widget
#include <common/types.h>
#include <common/graphicsContext.h>
#include <drivers/keyboardEventHandler.h>
using namespace mos::common;
using namespace mos::drivers;
namespace mos{
    namespace gui{
        class Widget:public KeyboardEventHandler{
            protected:
                Widget * parent;
                int32_t x;
                int32_t y;
                int32_t w;
                int32_t h;
                uint8_t r;
                uint8_t g;
                uint8_t b;
                bool focusable;
            public:
                Widget(Widget * parent,int32_t x,int32_t y,int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b);
                ~Widget();
                virtual void draw(GraphicsContext * gc);
                virtual void getFocus(Widget * wd);
                virtual void modelToScreen(int32_t &x,int32_t &y);
                virtual bool containsCoordinate(int32_t x,int32_t y);
                virtual void onMouseDown(int32_t x,int32_t y,uint8_t btn);
                virtual void onMouseUp(int32_t x,int32_t y,uint8_t btn);
                virtual void onMouseMove(int32_t oldx,int32_t oldy,int32_t newx,int32_t newy);
                virtual void onKeyDown(char);
                virtual void onKeyUp(char);
        };
    }
}
#endif        