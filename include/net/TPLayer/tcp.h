#ifndef _net_TPLayer_tcp
#define _net_TPLayer_tcp
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
            enum TCPSocketState{
                CLOSED,
                LISTENING,
                SYN_SENT,
                SYN_RECEIVED,
                ESTABLISHED,
                FIN_WAIT1,
                FIN_WAIT2,
                TIME_WAIT,
                CLOSE_WAIT
            };
            enum TCPFlags{
                FIN=1,
                SYN=2,
                RST=4,
                PSH=8,
                ACK=16,
                URG=32,
                ECE=64,
                CWR=128,
                NS=256
            };
            struct SEGMENT
            {
                uint16_t srcPort;
                uint16_t dstPort;
                uint32_t seqNum;
                uint32_t ack;
                uint8_t reserved:4;
                uint8_t headerSize:4;
                uint8_t flags;
                uint16_t windowSize;
                uint16_t checksum;
                uint16_t urgPtr;
                uint32_t options;
            }__attribute__((packed));
              struct SEGMENT_PSEUDO_HEADER
            {
                uint32_t srcIp;
                uint32_t dstIp;
                uint16_t protocol;
                uint16_t totalLen;
            }__attribute__((packed));
            
            class TCPSocket;
            class TCPProvider;
            class TCPHandler{
                public:
                    TCPHandler();
                    ~TCPHandler();
                    virtual bool handleSegment(TCPSocket* socket,uint8_t*data,uint16_t size);
            };

            class TCPSocket{
                friend class TCPProvider;
                uint16_t srcPort;
                uint32_t srcIp;
                uint16_t dstPort;
                uint32_t dstIp;
                uint32_t seqNum;
                uint32_t ack;
                TCPProvider * backend;
                TCPHandler * handler;
                TCPSocketState state;
                public:
                    TCPSocket(TCPProvider*);
                    ~TCPSocket();
                    virtual bool handleSegment(uint8_t*data,uint16_t size);
                    virtual void send(uint8_t*data,uint16_t size);
                    virtual void disconnect();

            };
            class  TCPProvider:public Ip4Handler{
                TCPSocket* sockets[0xffff];
                IP4Provider* _backend;
                uint16_t numSockets;
                uint16_t freePort;
                public:
                    TCPProvider(IP4Provider*);
                    ~TCPProvider();
                    virtual bool onPacketReceived(uint32_t srcIp,uint32_t dstIp,uint8_t *payload, uint32_t size);
                    virtual void disconnect(TCPSocket*);
                    virtual TCPSocket* connect(uint32_t ip,uint16_t port);
                    virtual TCPSocket* listen(uint16_t port);
                    virtual void send(TCPSocket*,uint8_t*data,uint16_t size,uint16_t flags=0);
                    virtual void bind(TCPSocket* socket, TCPHandler* handler); 
                
            };
            

        }
    }
}
#endif           