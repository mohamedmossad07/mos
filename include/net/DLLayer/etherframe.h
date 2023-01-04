#ifndef _net_DLLayer_etherframe
#define _net_DLLayer_etherframe
#include <common/types.h>
#include <MemoryManager.h>
#include <drivers/AMDNetCard.h>
using namespace mos::common;
using namespace mos::drivers;
namespace mos
{
    namespace net
    {
        namespace DLLayer
        {
            /*
            Application 	Message
            Transport 	    Segment(TCP),Datagram(UDP)
            Network 	    Datagram(IP)
            DLL 	        Frame
            Physical 	    stream of bits
            */
            struct FrameHeader
            {
                uint64_t dstMac : 48;
                uint64_t srcMac : 48;
                uint16_t etherType;
            } __attribute__((packed));
            typedef uint32_t FrameFooter;
            class EtherFrameProvider;
            class EtherFrameHandler
            {
                EtherFrameProvider *backend;
                uint16_t etherType;

            public:
                EtherFrameHandler(EtherFrameProvider *back, uint16_t etherType);
                ~EtherFrameHandler();
                virtual bool onFrameReceived(uint8_t *payload, uint32_t size);
                virtual bool send(uint64_t dstMac, uint8_t *payload, uint32_t size);
            };

            class EtherFrameProvider : public RawDataHandler
            {
                EtherFrameHandler *handlers[0xffff];
                AMDNetCard *backend;

            public:
                EtherFrameProvider(AMDNetCard *back);
                ~EtherFrameProvider();
                bool onRawDataReceived(uint8_t *buffer, uint32_t size);
                void send(uint64_t dstMace, uint16_t etherType, uint8_t *buffer, uint32_t size);
                void setHandler(EtherFrameHandler *, uint16_t num);
                uint32_t getMyIp();
                uint64_t getMac();
            };

        } // namespace DDLayer

    } // namespace net
}
#endif