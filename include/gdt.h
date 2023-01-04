#ifndef GDT_H
#define GDT_H
#include <common/types.h>
using namespace mos::common;
namespace mos{
    class GlobalDescriptorTable {
        public:
            class SegmentDescriptor {
    /*
        ;-------------------------------------;
        ;         Segment Descriptor          ;  
        ;-------------------------------------; 
        Segments can be described like a table of data
        ;-----------------------------------------------------------
        ;           (1)         ;       (2)               ;     (3)
        ;Base(begin of segment );Limit(size of segment)   ;Base mid bits
        ;       16 bit            ;16 bit                 ;     8 bit
        ;first 16 bit contains    ;contains first 16 bit  ;
        ;low 16 bit of 32bit base ;of 20 bit limit        ; 
        ;-------------------------------------------------------------------------  
        ;           (4)           ;         (5)     ;           (6)
        ;Acces byte (type)        ;Limit high       ;Segment Flags      ;
        ;       8 bit             ;  4 bit          ;4 bit              ;
        ;-------------------------------------------------------------------
        ;           (7)         ;
        ; Base high byte        ;
        ;  8 bit                ;
        ;---------------------------------------------
    */
                private :
                    mos::common::uint16_t limit_lo;//(1)
                    uint16_t base_lo;//(2)
                    uint8_t base_mid;//(3)
                    uint8_t access;//(4)
                    uint8_t flags_limit_hi;//(5,6)
                    uint8_t base_hi;//(7)
                public :
                    SegmentDescriptor (uint32_t base , uint32_t limit,uint8_t access);    
                    uint32_t Base();
                    uint32_t Limit();
            } __attribute__((packed));
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();
        uint16_t CodeSegmentSelector();
        uint16_t DataSegmentSelector();
    };
};

#endif