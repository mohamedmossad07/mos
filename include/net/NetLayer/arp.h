#ifndef _net_NetLayer_arp
#define _net_NetLayer_arp
#include <common/types.h>
#include <net/DLLayer/etherframe.h>
using namespace mos::common;
using namespace mos::net::DLLayer;
namespace mos
{
    namespace net
    {
        namespace NetLayer
        {
            struct ARPPacket
            {
                uint16_t hardwareType;
                uint16_t protocol;
                uint8_t MacSize;
                uint8_t ipSize;
                uint16_t command;
                uint64_t srcMac:48;
                uint32_t srcIP;
                uint64_t dstMac:48;
                uint32_t dstIP;
            } __attribute__((packed));
            
            class ARP : public EtherFrameHandler{
                uint32_t ipCache[128];
                uint64_t macCache[128];
                uint16_t cacheNum;
                uint64_t broadcastMac:48;
                EtherFrameProvider * _backend;
                public:
                    ARP(EtherFrameProvider*);
                    ~ARP();
                    virtual bool onFrameReceived(uint8_t *payload, uint32_t size);
                    void requestMacAddr(uint32_t ip);
                    uint64_t getMacFromCache(uint32_t ip);
                    uint64_t resolve(uint32_t ip);
                    void broadcastMyMac(uint32_t ip);
            };
        }
    }
}
#endif           