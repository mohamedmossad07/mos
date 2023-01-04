#include <gui/CompositeWidget.h>

CompositeWidget::CompositeWidget(Widget * parent,int32_t x,int32_t y,int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b):
focusedChild(0)
,numChildren(0)
,Widget(parent,x,y,w,h,r,g,b){

}

CompositeWidget::~CompositeWidget(){

}
void CompositeWidget::getFocus(Widget * wd){
    this->focusedChild=wd;
    if(parent!=0){
        parent->getFocus(this);
    }    
}
void CompositeWidget::onKeyDown(char str){
    if(focusedChild!=0)
        focusedChild->onKeyDown(str);
}
void CompositeWidget::onKeyUp(char str){
  if(focusedChild!=0)
        focusedChild->onKeyUp(str);
}
void CompositeWidget::draw(GraphicsContext * gc){
   Widget::draw(gc);
   for(int16_t i=numChildren-1;i>=0;i--){
        children[i]->draw(gc);
   }
}

void CompositeWidget::onMouseDown(int32_t x,int32_t y,uint8_t btn){
    for(int i = 0; i < numChildren; ++i)
        if(children[i]->containsCoordinate(x - this->x, y - this->y))
        {
            children[i]->onMouseDown(x - this->x, y - this->y,btn);
            break;
        }
}
void CompositeWidget::onMouseUp(int32_t x,int32_t y,uint8_t btn){
      for(int i = 0; i < numChildren; ++i)
        if(children[i]->containsCoordinate(x - this->x, y - this->y))
        {
            children[i]->onMouseUp(x - this->x, y - this->y,btn);
            break;
        }
}
void CompositeWidget::onMouseMove(int32_t oldx,int32_t oldy,int32_t newx,int32_t newy){
      int firstchild = -1;
    for(int i = 0; i < numChildren; ++i)
        if(children[i]->containsCoordinate(oldx - this->x, oldy - this->y))
        {
            children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            firstchild = i;
            break;
        }

    for(int i = 0; i < numChildren; ++i)
        if(children[i]->containsCoordinate(newx - this->x, newy - this->y))
        {
            if(firstchild != i)
                children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            break;
        }
}
bool  CompositeWidget::addChild(Widget * wd){
    if(numChildren>=100)return false;
    children[numChildren++]=wd;
    return true;
}
