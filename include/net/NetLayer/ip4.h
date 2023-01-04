#ifndef _net_NetLayer_ip4
#define _net_NetLayer_ip4
#include <common/types.h>
#include <net/DLLayer/etherframe.h>
#include <net/NetLayer/arp.h>
using namespace mos::common;
using namespace mos::net::DLLayer;
using namespace mos::net::NetLayer;
namespace mos
{
    namespace net
    {
        namespace NetLayer
        {
            struct IP4Packet
            {
                uint8_t headerLength:4;
                uint8_t version:4;
                uint8_t tos;
                uint16_t totalLength;
                uint16_t ident;
                uint16_t flagsOffset;
                uint8_t ttl;
                uint8_t protocol;
                uint16_t checksum;
                uint32_t srcIp;
                uint32_t dstIp;
            }__attribute__((packed));
            class IP4Provider;
            class Ip4Handler
            {
                IP4Provider *backend;
                uint8_t ip_protocol;

            public:
                Ip4Handler(IP4Provider *back, uint8_t ip_protocol);
                ~Ip4Handler();
                virtual bool onPacketReceived(uint32_t srcIp,uint32_t dstIp,uint8_t *payload, uint32_t size);
                virtual bool send(uint32_t dstIp, uint8_t *payload, uint32_t size);
            };
            class IP4Provider : public EtherFrameHandler{
                Ip4Handler *handlers[0xff];
                EtherFrameProvider* _backend;
                ARP* arp;
                uint32_t gip;
                uint32_t subnet;
                public:
                    IP4Provider(EtherFrameProvider *back,ARP*,uint32_t gip,uint32_t subnet);
                    ~IP4Provider();
                    virtual bool onFrameReceived(uint8_t *payload, uint32_t size);
                    void send(uint32_t dstIp, uint8_t protocol, uint8_t *buffer, uint32_t size);
                    void addHandler(Ip4Handler *, uint8_t num);
                    static uint16_t checksum(uint16_t* data,uint32_t len);
                    uint32_t getMyIp();
            };
        }
    }
}
#endif           