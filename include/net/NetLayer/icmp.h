#ifndef _net_NetLayer_icmp
#define _net_NetLayer_icmp
#include <common/types.h>
#include <net/DLLayer/etherframe.h>
#include <net/NetLayer/ip4.h>
using namespace mos::common;
using namespace mos::net::DLLayer;
using namespace mos::net::NetLayer;
namespace mos
{
    namespace net
    {
        namespace NetLayer
        {
            struct ICMPPacket
            {
                uint8_t type;
                uint8_t code;
                uint16_t checksum;
                uint32_t data;
            }__attribute__((packed));
            
            class ICMP :public Ip4Handler
            {
                IP4Provider *_backend;
                public:
                    ICMP(IP4Provider *back);
                    ~ICMP();
                    virtual bool onPacketReceived(uint32_t srcIp,uint32_t dstIp,uint8_t *payload, uint32_t size);
                    void ping(uint32_t ip);
            
            }; 
        }   
    }
}
#endif        