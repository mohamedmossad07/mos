#ifndef _MOS_gui_desktop
#define _MOS_gui_desktop
#include <common/types.h>
#include <gui/CompositeWidget.h>
#include <drivers/mouseEventHandler.h>
using namespace mos::drivers;
using namespace mos::common;
using namespace mos::gui;
namespace mos{
    namespace gui{
        class Desktop:public MouseEventHandler,public CompositeWidget{
            protected:
                uint32_t mousex;
                uint32_t mousey;
            public :
                Desktop(int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b);
                ~Desktop();
                void draw(GraphicsContext * gc);
                void onMouseDown(uint8_t btn);
                void onMouseUp(uint8_t btn);
                void onMouseMove(int x,int y);
        };
    }
}
#endif