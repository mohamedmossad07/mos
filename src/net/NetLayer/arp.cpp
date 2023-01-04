#include <net/NetLayer/arp.h>
using namespace mos::net::NetLayer;

void printf(char *);
void printfHex(int8_t hex);
ARP::ARP(EtherFrameProvider* back):EtherFrameHandler(back,0x0806),cacheNum(0),
broadcastMac(0xffffffffffff),_backend(back){

}
ARP::~ARP(){

}
bool ARP::onFrameReceived(uint8_t *payload, uint32_t size){
    if(size<sizeof(ARPPacket))
            return false;
    ARPPacket * pack=(ARPPacket*)payload;
    if (pack->hardwareType==0x0100&&pack->protocol==0x0008&&
    pack->MacSize==6&&pack->ipSize==4&&
    pack->dstIP==_backend->getMyIp())
    {
        switch (pack->command)
        {
        case 0x0100:
            printf("ARP Request\n");
            pack->command=0x0200;
            pack->dstIP=pack->srcIP;
            pack->dstMac=pack->srcMac;
            pack->srcIP=_backend->getMyIp();
            pack->srcMac=_backend->getMac();
            break;
        case 0x0200:
            printf("ARP ReSponse\n");
            if(cacheNum<128){
                ipCache[cacheNum]=pack->srcIP;
                macCache[cacheNum]=pack->srcMac;
                cacheNum++;
            }
        break;
        default:
            break;
        }
    }
    return false;
}
void ARP::requestMacAddr(uint32_t ip){
    ARPPacket pack;
    pack.hardwareType=0x0100;
    pack.protocol=0x0008;
    pack.MacSize=6;
    pack.ipSize=4;
    pack.command=0x0100;//request
    pack.dstIP=ip;
    pack.dstMac=broadcastMac;
    pack.srcIP=_backend->getMyIp(); 
    pack.srcMac=_backend->getMac(); 
    this->send(pack.dstMac,(uint8_t*)&pack,sizeof(ARPPacket));
}
uint64_t ARP::getMacFromCache(uint32_t ip){
for (uint16_t i = 0; i < 128; i++)
{
    if(ipCache[i]==ip)
        {return macCache[i];}
}
     return broadcastMac;
}
uint64_t ARP::resolve(uint32_t ip){
   uint64_t res=getMacFromCache(ip);
    if(res==this->broadcastMac)     
    {
        requestMacAddr(ip);
    }
    while(res==broadcastMac)
        res=getMacFromCache(ip);
    return res;
}

void ARP::broadcastMyMac(uint32_t ip){
    ARPPacket pack;
    pack.hardwareType=0x0100;
    pack.protocol=0x0008;
    pack.MacSize=6;
    pack.ipSize=4;
    pack.command=0x0200;//res
    pack.dstIP=ip;
    pack.dstMac=resolve(ip);
    pack.srcIP=_backend->getMyIp(); 
    pack.srcMac=_backend->getMac(); 
    send(pack.dstMac,(uint8_t*)&pack,sizeof(ARPPacket));
}