#ifndef _MOS_gui_compositeWidget
#define _MOS_gui_compositeWidget
#include <common/types.h>
#include <common/graphicsContext.h>
#include <gui/widget.h>
using namespace mos::common;
using namespace mos::gui;
namespace mos{
    namespace gui{
        class CompositeWidget:public Widget{
            private:
                Widget * children[100];
                uint16_t numChildren;
                Widget * focusedChild;
            public:
                CompositeWidget(Widget * parent,int32_t x,int32_t y,int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b);
                ~CompositeWidget();
                virtual void draw(GraphicsContext * gc);
                virtual void getFocus(Widget * wd);
                virtual bool addChild(Widget * wd);
                virtual void onMouseDown(int32_t x,int32_t y,uint8_t btn);
                virtual void onMouseUp(int32_t x,int32_t y,uint8_t btn);
                virtual void onMouseMove(int32_t oldx,int32_t oldy,int32_t newx,int32_t newy);
                virtual void onKeyDown(char str);
                virtual void onKeyUp(char str);
       

        };
    }
}
#endif