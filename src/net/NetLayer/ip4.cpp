#include <MemoryManager.h>
#include <net/NetLayer/ip4.h>
using namespace mos::net::NetLayer;

Ip4Handler::Ip4Handler(IP4Provider *back, uint8_t ip_protocol):
backend(back),ip_protocol(ip_protocol)
{
    backend->addHandler(this,ip_protocol);
}
Ip4Handler::~Ip4Handler(){
    backend->addHandler(0,ip_protocol);
}
bool Ip4Handler::onPacketReceived(uint32_t srcIp,uint32_t dstIp,uint8_t *payload, uint32_t size){
    return false;
}
bool Ip4Handler::send(uint32_t dstIp, uint8_t *payload, uint32_t size){
    backend->send(dstIp,ip_protocol,payload,size);
}

IP4Provider::IP4Provider(EtherFrameProvider *back,ARP*arp,uint32_t gip,uint32_t subnet):
EtherFrameHandler(back,0x0800),
arp(arp),
gip(gip),
subnet(subnet),
_backend(back)
{
 for (int i = 0; i < 0xff; i++)
    {
        handlers[i]=0;
    }
}

IP4Provider::~IP4Provider(){}
bool IP4Provider::onFrameReceived(uint8_t *payload, uint32_t size){
if(size<sizeof(IP4Packet))return false;
IP4Packet* pack=(IP4Packet*)payload;    
bool sendBack=false;
if(pack->dstIp==_backend->getMyIp())
{
    uint16_t length=pack->totalLength;
    if(length>size)length=size;
    if(handlers[pack->protocol]!=0){
        sendBack=handlers[pack->protocol]->onPacketReceived(pack->srcIp,pack->dstIp,4*pack->headerLength+payload,length-4*pack->headerLength);
    }
    if(sendBack){
        pack->dstIp=pack->srcIp;    
        pack->srcIp=_backend->getMyIp();
        pack->ttl=0x40;
        pack->checksum=0;
        pack->checksum=checksum((uint16_t*)pack,4*pack->totalLength);
    }
}
return sendBack;
}
void IP4Provider::send(uint32_t dstIp, uint8_t protocol, uint8_t *buffer, uint32_t size){
    uint8_t* buffer2=(uint8_t*)  MemoryManager::activeMemMgr->malloc(sizeof(IP4Packet)+size);
    IP4Packet* pack = (IP4Packet*)buffer2;
    pack->version=4;
    pack->headerLength=sizeof(IP4Packet)/4;
    pack->dstIp=dstIp;
    pack->tos=0;
    pack->totalLength=sizeof(IP4Packet)+size;
    pack->totalLength=((pack->totalLength & 0xFF00) >> 8)
                         | ((pack->totalLength & 0x00FF) << 8);
    pack->ident=0x0100;
    pack->flagsOffset= 0x0040;
    pack->ttl= 0x40;
    pack->protocol= protocol;
    pack->srcIp=_backend->getMyIp();
    pack->checksum=0;
    pack->checksum=checksum((uint16_t*)pack,sizeof(IP4Packet));
    uint8_t* dst=buffer2+sizeof(IP4Packet);
    for (uint32_t i = 0; i < size; i++)
    {
        dst[i]=buffer[i];
    }

    uint32_t route = dstIp;
    if((dstIp & subnet) != (pack->srcIp & subnet))
        route = gip;
    EtherFrameHandler::send(arp->resolve(route),buffer2,size+sizeof(IP4Packet));
    MemoryManager::activeMemMgr->free(buffer2);
}
void IP4Provider::addHandler(Ip4Handler * handler, uint8_t num){
    handlers[num]=handler;
}
uint16_t IP4Provider::checksum(uint16_t* data,uint32_t len){
    uint32_t temp=0;
    for (int i = 0; i < len/2; i++)
    {
        temp += ((data[i] & 0xFF00) >> 8) | ((data[i] & 0x00FF) << 8);
    }
     while(temp & 0xFFFF0000)
        temp = (temp & 0xFFFF) + (temp >> 16);
    
    return ((~temp & 0xFF00) >> 8) | ((~temp & 0x00FF) << 8);
}
uint32_t IP4Provider::getMyIp(){
    return _backend->getMyIp();
}