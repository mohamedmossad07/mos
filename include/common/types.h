#ifndef _MOS_COMMON_TYPES_H
#define _MOS_COMMON_TYPES_H
namespace mos{
    namespace common{
        // We need to  make this file to control of datatypes in 
        // the whole program due to un standard compilers datatypes
        // for example some compilers compile unsigned int as 8 bytes
        // while others compile it as 4 byte
        typedef char int8_t;//8 bit type
        typedef unsigned char uint8_t;

        typedef short int16_t;//16 bit type
        typedef unsigned short uint16_t;

        typedef int int32_t;//32 bit type
        typedef unsigned int uint32_t;

        typedef long long int int64_t;//64 bit type
        typedef unsigned long long int uint64_t;

        typedef const char* string;
        typedef unsigned int size_t;
    };
};
#endif