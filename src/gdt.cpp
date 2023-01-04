#include <gdt.h>
#include <common/types.h>
using namespace mos;

GlobalDescriptorTable::GlobalDescriptorTable():
nullSegmentSelector(0,0,0),
unusedSegmentSelector(0,0,0),
codeSegmentSelector(0,64*1024*1024,0x9A),
dataSegmentSelector(0,64*1024*1024,0x92){

    uint32_t i[2];//array of 2 spaces
    i[0]=sizeof(GlobalDescriptorTable)<<16;//shift left 16 digit
    i[1]=(uint32_t) this;//reference to the current object
    asm volatile("lgdt (%0)": :"p" (((uint8_t*) i )+2));//load global descriptor table
}

GlobalDescriptorTable::~GlobalDescriptorTable(){

}
uint16_t GlobalDescriptorTable::DataSegmentSelector(){
    return (uint8_t*) &dataSegmentSelector-(uint8_t*)(this);
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector(){
    return (uint8_t*) &codeSegmentSelector-(uint8_t*)(this);
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base , uint32_t limit,uint8_t access){
     uint8_t* target = (uint8_t*)this;

	// 16-bit address space
    /**
     * @brief 
     * !!! This part is very important
     * 
     *1) first note is that the limit is actually 20-bit but here we have 32-bit
     * so we need to bit-slice it 
     * 
     * 2) second note is that depend on this limit i will set the 4-bit flags
     * if limit is  < 64KB 0XFFFF that means we have toke the first 16-bit
     * only so we should clear the fourth bit in flags Granularity flage
     * which specify if the we dealing with byte-Granularity  so that the only 
     * bit which will be set in the 6`th byte now the byte should be like 
     * 0100 0000 ==> equals 0X40 hex
     * 
     * 3) if the limit is >0xffff that means we should deal with 32-bit mode 
     * so we should set the page-Granularity every limit will be multiplied 
     * by 4KB now the 6`byte should be like 1100 0000 =>> 0XC0 hex
     * 
     * 4) Now we should bit-slice the limit we need just the most 20-bit
     * so we`ll shift the least 12-bit to right 
     * 
     */
    if (limit <= 65536) {
        target[6] = 0x40;
    } else {
        // 32-bit address space
        // Now we have to squeeze the (32-bit) limit into 2.5 register (20-bit).
        // This is done by discarding the 12 least significant bits, but this
        // is only legal, if they are all ==1, so they are implicitly still there

        // so if the last bits aren't all 1, we have to set them to 1, but this
        // would increase the limit (cannot do that, because we might go beyond
        // the physical limit or get overlap with other segments) so we have to
        // compensate this by decreasing a higher bit (and might have up to
        // 4095 wasted bytes behind the used memory)

        if((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12)-1;
        else
            limit = limit >> 12;

        target[6] = 0xC0;
    }
    

    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Type
    target[5] = access;
}
uint32_t GlobalDescriptorTable::SegmentDescriptor::Base() {
    uint8_t* target = (uint8_t*)this;
    /**
     * @brief 
     * Here we forming the Base 4-byte 
     * the base existing in the byte 2,3,4,7
     * so we should forming it by reverse we are filling the least byte first 
     * then we shift it to left and etc..
     * 
     **/
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit() {
    uint8_t* target = (uint8_t*)this;

    uint32_t result = target[6] & 0xF;//here we get only the first 4-bits
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0)//if we are in 32-bit mode 
        result = (result << 12) | 0xFFF;

    return result;
}