#include <gui/desktop.h>
using namespace mos::gui;

Desktop::Desktop(int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b):
CompositeWidget(0,0,0,w,h,r,g,b),
MouseEventHandler(){
mousex=w/2;
mousey=h/2;
}
Desktop::~Desktop(){

}
void Desktop::draw(GraphicsContext * gc){
CompositeWidget::draw(gc);
for (uint8_t i = 0; i < 4; i++)
{
     gc->putPixel(mousex-i,mousey,0xff,0xff,0xff);
     gc->putPixel(mousex+i,mousey,0xff,0xff,0xff);
     gc->putPixel(mousex,mousey-i,0xff,0xff,0xff);
     gc->putPixel(mousex,mousey+i,0xff,0xff,0xff);
}

}
void Desktop::onMouseDown(uint8_t btn){
CompositeWidget::onMouseDown(mousex,mousey,btn);
}
void Desktop::onMouseUp(uint8_t btn){
CompositeWidget::onMouseDown(mousex,mousey,btn);
}
void Desktop::onMouseMove(int x,int y){
x/=4;
y/=4;    
int32_t newx=mousex+x;
if(newx<0)newx=0;
if(newx>w)newx=w-1;
int32_t newy=mousey+y;
if(newy<0)newy=0;
if(newy>w)newy=h-1;
CompositeWidget::onMouseMove(mousex,mousey,newx,newy);
mousex=newx;
mousey=newy;
}