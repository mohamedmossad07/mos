#include <MemoryManager.h>
using namespace mos;
MemoryManager* MemoryManager::activeMemMgr=0;
MemoryManager::MemoryManager(size_t _first,size_t size){
activeMemMgr=this;
if(size<sizeof(MemoryChunk)){
    first=0;
}else{
first=(MemoryChunk*)_first;
first->allocated=false;
first->next=0;
first->prev=0;
first->size=size-sizeof(MemoryChunk);
}
}
MemoryManager::~MemoryManager(){
if(activeMemMgr==this)activeMemMgr=0;
}
void* MemoryManager::malloc(size_t size){

MemoryChunk* result=0;
for(MemoryChunk* chunk=first;chunk!=0&&result==0;chunk=chunk->next){
        if(chunk->size>size&&!chunk->allocated){
            result=chunk;     
        } 
    }
    if(result==0){
        return 0;
    }
    if(result!=0&&result->size>=size+sizeof(MemoryChunk)+1){
        MemoryChunk* nextChunk=(MemoryChunk*)((size_t)result+size+sizeof(MemoryChunk));
        nextChunk->allocated=false;
        nextChunk->next=result->next;
        nextChunk->prev=result;
        nextChunk->size=result->size-size-sizeof(MemoryChunk);
        result->next=nextChunk;
        result->size=size;
        result->allocated=true;
    }
        return (void*)(((size_t)result)+sizeof(MemoryChunk));
}
void MemoryManager::free(void* ptr){
    MemoryChunk* chunk=(MemoryChunk*) ((size_t)ptr-sizeof(MemoryChunk));
    chunk->allocated=false;
    if(chunk->prev!=0&&!chunk->prev->allocated){
        chunk->prev->size+=chunk->size+sizeof(MemoryChunk);
        chunk->prev->next=chunk->next;
        if(chunk->next!=0){
            chunk->next->prev=chunk->prev;
        }
        chunk=chunk->prev;
        if(chunk->next!=0&&!chunk->next->allocated){
            chunk->size+=chunk->next->size+sizeof(MemoryChunk);
            chunk->next=chunk->next->next;
            if(chunk->next->next!=0){
                chunk->next->next->prev=chunk;
            }
        }
    }
}
void* operator new(unsigned size)
{
    if(MemoryManager::activeMemMgr == 0)
        return 0;
    return MemoryManager::activeMemMgr->malloc(size);
}

void* operator new[](unsigned size)
{
    if(MemoryManager::activeMemMgr == 0)
        return 0;
    return MemoryManager::activeMemMgr->malloc(size);
}

void* operator new(unsigned size, void* ptr)
{
    return ptr;
}

void* operator new[](unsigned size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    if(MemoryManager::activeMemMgr != 0)
        MemoryManager::activeMemMgr->free(ptr);
}

void operator delete[](void* ptr)
{
    if(MemoryManager::activeMemMgr != 0)
        MemoryManager::activeMemMgr->free(ptr);
}