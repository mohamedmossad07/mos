#include <gui/widget.h>
using namespace mos::gui;
Widget::Widget(Widget * _parent,int32_t _x,int32_t _y,int32_t _w,int32_t _h,uint8_t _r,uint8_t _g,uint8_t _b)
:parent(_parent)
,x(_x)
,y(_y)
,w(_w)
,h(_h)
,r(_r)
,g(_g)
,b(_b)
,focusable(true)
,KeyboardEventHandler()
{}

Widget::~Widget(){

}
void Widget::getFocus(Widget * wd){
    if(parent!=0){
        parent->getFocus(wd);
    }
}
void Widget::modelToScreen(int32_t &x,int32_t &y){
    if(parent!=0){
        parent->modelToScreen(x,y);
    }
    x+=this->x;
    y+=this->y;
}
void Widget::draw(GraphicsContext * gc){
    int X=0;
    int Y=0;
    modelToScreen(X,Y);
    gc->fillRectangle(X,Y,w,h,r,g,b);
}
bool Widget::containsCoordinate(int32_t x,int32_t y){
    return this->x <= x && this->x+this->w>x &&this->y<=y&&this->y+h>y;
}
void Widget::onMouseDown(int32_t x,int32_t y,uint8_t btn){
    getFocus(this);
}
void Widget::onMouseUp(int32_t x,int32_t y,uint8_t btn){}
void Widget::onMouseMove(int32_t oldx,int32_t oldy,int32_t newx,int32_t newy){
    
}
void Widget::onKeyDown(char str){}
void Widget::onKeyUp(char str){}