#include <net/NetLayer/icmp.h>
using namespace mos::net::NetLayer;

void printf(char *);
void printfHex(int8_t hex);
ICMP::ICMP(IP4Provider *back) : Ip4Handler(back, 0x01), _backend(back)
{
}
ICMP::~ICMP()
{
}
bool ICMP::onPacketReceived(uint32_t srcIp, uint32_t dstIp, uint8_t *payload, uint32_t size)
{
    if (size < sizeof(ARPPacket))
        return false;
    ICMPPacket *pack = (ICMPPacket *)payload;
    switch (pack->type)
    {
    case 0:
        printf("ping response from ");
        printfHex(srcIp & 0xFF);
        printf(".");
        printfHex((srcIp >> 8) & 0xFF);
        printf(".");
        printfHex((srcIp >> 16) & 0xFF);
        printf(".");
        printfHex((srcIp >> 24) & 0xFF);
        break;

    case 8:
        pack->type = 0;
        pack->code = 0;
        pack->checksum = 0;
        pack->checksum = _backend->checksum((uint16_t *)&pack, sizeof(ICMPPacket));
        return true;
        break;
    default:
        break;
    }
    return false;
}
void ICMP::ping(uint32_t ip)
{
    ICMPPacket pack;
    pack.type = 8;
    pack.code = 0;
    pack.data = 0x3713;
    pack.checksum = 0;
    pack.checksum = _backend->checksum((uint16_t *)&pack, sizeof(ICMPPacket));
    send(ip, (uint8_t *)&pack, sizeof(ICMPPacket));
}