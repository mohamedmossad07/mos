#include <common/types.h>
#include <common/helper.h>
using namespace mos::common;
void mos::common::clearScreen(){
        uint16_t * VideoMemory = (uint16_t *)0xb8000;
    for(int i=0;i<25;i++){
         for(int j=0;j<80;j++){
        VideoMemory[80*i+j] = (VideoMemory[80*i+j] & 0xff00) | ' ';
      }
    }
}