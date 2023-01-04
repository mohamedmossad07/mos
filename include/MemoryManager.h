#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER
#include <common/types.h>
#include <MemoryChunk.h>
using namespace mos;
using namespace mos::common;
namespace mos{
    class MemoryManager {
        protected:
            MemoryChunk* first;
        public:
            static MemoryManager* activeMemMgr;
            MemoryManager(size_t first,size_t size);
            ~MemoryManager();
            void* malloc(size_t size);
            void free(void* ptr);
    };
}
void* operator new(unsigned size);
void* operator new[](unsigned size);

// placement new
void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);




#endif     