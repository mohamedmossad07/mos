#include <drivers/vga.h>
using namespace mos::drivers;

VGA::VGA():
miscPort(0x3c2),
crtcIndexPort(0x3d4), 
crtcDataPort(0x3d5), 
seqIndexPort(0x3c4), 
seqDataPort(0x3c5), 
graphicControllerIndexPort(0x3ce), 
graphicControllerDataPort(0x3cf), 
attrControllerIndexPort(0x3c0), 
attrControllerReadPort(0x3c1), 
attrControllerWritePort(0x3c0), 
attrControllerResetPort(0x3da){}
 
VGA::~VGA(){} 
void VGA::writeRegisters(uint8_t *  regs){
    miscPort.write(*(regs++));
    for(uint8_t i=0;i<5;i++){
        seqIndexPort.write(i);
        seqDataPort.write(*(regs++));
    }
    // warning
    crtcIndexPort.write(0x03);
    crtcDataPort.write(crtcDataPort.read()|0x80);
    crtcIndexPort.write(0x11);
    crtcDataPort.write(crtcDataPort.read()& ~0x80);
    regs[0x03]=regs[0x03] | 0x80;
    regs[0x11]=regs[0x11] & ~0x80;
    for(uint8_t i=0;i<25;i++){
        crtcIndexPort.write(i);
        crtcDataPort.write(*(regs++));
    }
     for(uint8_t i=0;i<9;i++){
        graphicControllerIndexPort.write(i);
        graphicControllerDataPort.write(*(regs++));
    }
   for(uint8_t i=0;i<21;i++){
        attrControllerResetPort.read();
        attrControllerIndexPort.write(i);
        attrControllerWritePort.write(*(regs++));
    }
        attrControllerResetPort.read();
        attrControllerIndexPort.write(0x20);

}

uint8_t * VGA::getSegmentPosition(){
    graphicControllerIndexPort.write(0x06);
    uint8_t sNumber=((graphicControllerDataPort.read()>>2)&0x03);
    switch(sNumber){
        default:
        case 0: return (uint8_t*)0x00000;
        case 1: return (uint8_t*)0xA0000;
        case 2: return (uint8_t*)0xB0000;
        case 3: return (uint8_t*)0xB8000;
    }
}

void VGA::putPixel(int32_t x,int32_t y,uint8_t color){
    uint8_t * pixelAddr=getSegmentPosition() + 320*y+x;
    *pixelAddr=color;
}

uint8_t VGA::getColorIndex(uint8_t r,uint8_t g,uint8_t b){
    if(r == 0x00 && g==0x00 && b==0x00)return 0x00;//black
    if(r == 0x00 && g==0x00 && b==0xa8)return 0x01;//blue   
    if(r == 0x00 && g==0xa8 && b==0x00)return 0x02;//green    
    if(r == 0xa8 && g==0x00 && b==0x00)return 0x04;//red    
    if(r == 0xff && g==0xff && b==0xff)return 0x3f;//white    
    return 0x00;
}


bool VGA::isModeSupported(uint32_t width,uint32_t height,uint32_t colorDepth){
    return width==320 && height == 200&& colorDepth==8;
}

bool VGA::setMode(uint32_t width,uint32_t height,uint32_t colorDepth){
    if(!isModeSupported(width,height,colorDepth))return false;
      unsigned char g_320x200x256[] =
    {
        /* MISC */
            0x63,
        /* SEQ */
            0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
            0xFF,
        /* GC */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
        /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00
    };
    writeRegisters(g_320x200x256);
    return true;
}

void VGA::putPixel(int32_t x,int32_t y,uint8_t r,uint8_t g,uint8_t b){
    putPixel(x,y,getColorIndex(r,g,b)); 
}


void VGA::fillRectangle(uint32_t X,uint32_t Y,uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b){
     for(uint8_t y=Y;y<Y+h;y++){
        for(uint16_t x=X;x<X+w;x++){
           putPixel(x,y,r,g,b); 
    }
    }
}