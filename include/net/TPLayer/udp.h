#ifndef _net_TPLayer_udp
#define _net_TPLayer_udp
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
        namespace TPLayer
        {
            struct DATAGRAM
            {
                uint16_t srcPort;
                uint16_t dstPort;
                uint16_t length;
                uint16_t checksum;
            }__attribute__((packed));
            
            class UDPSocket;
            class UDPProvider;
            class UDPHandler{
                public:
                    UDPHandler();
                    ~UDPHandler();
                    virtual void handleDatagram(UDPSocket* socket,uint8_t*data,uint16_t size);
            };

            class UDPSocket{
                friend class UDPProvider;
                uint16_t srcPort;
                uint32_t srcIp;
                uint16_t dstPort;
                uint32_t dstIp;
                bool listening;
                UDPProvider * backend;
                UDPHandler * handler;
                public:
                    UDPSocket(UDPProvider*);
                    ~UDPSocket();
                    virtual void handleDatagram(uint8_t*data,uint16_t size);
                    virtual void send(uint8_t*data,uint16_t size);
                    virtual void disconnect();

            };
            class  UDPProvider:public Ip4Handler{
                UDPSocket* sockets[0xffff];
                IP4Provider* _backend;
                uint16_t numSockets;
                uint16_t freePort;
                public:
                    UDPProvider(IP4Provider*);
                    ~UDPProvider();
                    virtual bool onPacketReceived(uint32_t srcIp,uint32_t dstIp,uint8_t *payload, uint32_t size);
                    virtual void disconnect(UDPSocket*);
                    virtual UDPSocket* connect(uint32_t ip,uint16_t port);
                    virtual UDPSocket* listen(uint16_t port);
                    virtual void send(UDPSocket*,uint8_t*data,uint16_t size);
                    virtual void bind(UDPSocket* socket, UDPHandler* handler); 
                
            };
            
        }
    }
}
#endif        