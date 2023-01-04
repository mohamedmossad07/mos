#include <gui/window.h>
Window::Window(Widget * parent,int32_t x,int32_t y,int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b):
CompositeWidget(parent,x,y,w,h,r,g,b){
dragging=false;
}
Window::~Window(){}
void Window::onMouseDown(int32_t x,int32_t y,uint8_t btn){
dragging=btn==1;
 CompositeWidget::onMouseDown(x,y,btn);
}
void Window::onMouseUp(int32_t x,int32_t y,uint8_t btn){
dragging=false;
    CompositeWidget::onMouseUp(x,y,btn);
}
void Window::onMouseMove(int32_t oldx,int32_t oldy,int32_t newx,int32_t newy){
  if(dragging)
    {
        this->x += newx-oldx;
        this->y += newy-oldy;
    }
    CompositeWidget::onMouseMove(oldx,oldy,newx, newy);
}
