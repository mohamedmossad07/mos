#ifndef _MOS_DRIVERS_VGA
#define _MOS_DRIVERS_VGA
#include <hardware/port.h>
#include <common/types.h>
#include <drivers/driver.h> 
using namespace mos::hardware;
using namespace mos::common;
using namespace mos::drivers;
namespace mos{
    namespace drivers{
        class VGA{
            protected:
                port8Bit miscPort; 
                port8Bit crtcIndexPort; 
                port8Bit crtcDataPort; 
                port8Bit seqIndexPort; 
                port8Bit seqDataPort; 
                port8Bit graphicControllerIndexPort; 
                port8Bit graphicControllerDataPort; 
                port8Bit attrControllerIndexPort; 
                port8Bit attrControllerReadPort; 
                port8Bit attrControllerWritePort; 
                port8Bit attrControllerResetPort; 
                void writeRegisters(uint8_t *  regs);
                uint8_t * getSegmentPosition();
                virtual uint8_t getColorIndex(uint8_t r,uint8_t g,uint8_t b); 
            public:
                VGA();
                ~VGA();
                virtual bool isModeSupported(uint32_t width,uint32_t height,uint32_t colorDepth); 
                virtual bool setMode(uint32_t width,uint32_t height,uint32_t colorDepth); 
                virtual void putPixel(int32_t x,int32_t y,uint8_t r,uint8_t g,uint8_t b); 
                virtual void putPixel(int32_t x,int32_t y,uint8_t color); 
                virtual void fillRectangle(uint32_t x,uint32_t y,uint32_t w,uint32_t h,uint8_t r,uint8_t g,uint8_t b); 

        };
    }
}
#endif