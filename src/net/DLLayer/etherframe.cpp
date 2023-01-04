#include<net/DLLayer/etherframe.h>
using namespace mos::net::DLLayer;
void printf(int8_t *str);
EtherFrameHandler::EtherFrameHandler(EtherFrameProvider* back,uint16_t _etherType):
backend(back)
{
    this->etherType=((_etherType&0x00ff)<<8)|((_etherType&0xff00)>>8);
    backend->setHandler(this,this->etherType);
}
EtherFrameHandler::~EtherFrameHandler(){
    backend->setHandler(0,etherType);
}
bool EtherFrameHandler::onFrameReceived(uint8_t * payload,uint32_t size){
    return false;
}
bool EtherFrameHandler::send(uint64_t dstMace,uint8_t *payload, uint32_t size){
    backend->send(dstMace,etherType,payload,size);
}
EtherFrameProvider::EtherFrameProvider(AMDNetCard* back):RawDataHandler(back){
   this->backend=back;
    for (int i = 0; i < 0xffff; i++)
    {
        handlers[i]=0;
    }
    
}
EtherFrameProvider::~EtherFrameProvider(){}
bool EtherFrameProvider::onRawDataReceived(uint8_t * buffer,uint32_t size){
if(size<sizeof(FrameHeader))return false;
FrameHeader* frame=(FrameHeader*)buffer;    
bool sendBack=false;
if(frame->dstMac==0xffffffffffff||frame->dstMac==backend->getMACAddress())
{
    if(handlers[frame->etherType]!=0){
        sendBack=handlers[frame->etherType]->onFrameReceived(buffer+sizeof(FrameHeader),size-sizeof(FrameHeader));
    }
    if(sendBack){
        frame->dstMac=frame->srcMac;    
        frame->srcMac=backend->getMACAddress();
    }
}
return sendBack;
}
void EtherFrameProvider::send(uint64_t dstMace,uint16_t etherType,uint8_t * buffer,uint32_t size){
    uint8_t* buffer2=(uint8_t*)  MemoryManager::activeMemMgr->malloc(sizeof(FrameHeader)+size);
    FrameHeader* frame = (FrameHeader*)buffer2;
    frame->dstMac=dstMace;
    frame->srcMac=backend->getMACAddress();
    frame->etherType=etherType;
    uint8_t* src=buffer;
    uint8_t* dst=buffer2+sizeof(FrameHeader);
    for (uint32_t i = 0; i < size; i++)
    {
        dst[i]=src[i];
    }
    backend->send(buffer2,size+sizeof(FrameHeader));
    MemoryManager::activeMemMgr->free(buffer2);
}
void EtherFrameProvider::setHandler(EtherFrameHandler* handler,uint16_t num){
    handlers[num]=handler;
}
uint32_t EtherFrameProvider::getMyIp(){
    return backend->getMyIp();
} 
uint64_t EtherFrameProvider::getMac(){
    return backend->getMACAddress();
} 