#ifndef MEMORY_CHUNK
#define MEMORY_CHUNK
#include <common/types.h>
using namespace mos::common;
namespace mos{
    struct MemoryChunk {
        MemoryChunk* next; 
        MemoryChunk* prev;
        bool allocated;
        size_t size; 
    };
}
#endif     