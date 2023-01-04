#include <MemoryManager.h>
#include <net/TPLayer/udp.h>
using namespace mos::net::TPLayer;
void printfHex(int16_t hex);
void printf(int8_t *str);

UDPHandler::UDPHandler(){}
UDPHandler::~UDPHandler(){}
void UDPHandler::handleDatagram(UDPSocket* socket,uint8_t*data,uint16_t size){

}
UDPSocket::UDPSocket(UDPProvider* back):backend(back),handler(0),listening(false){}
UDPSocket::~UDPSocket(){}
void UDPSocket::handleDatagram(uint8_t*data,uint16_t size){
    if (handler!=0)
        handler->handleDatagram(this,data,size);
}
void UDPSocket::send(uint8_t*data,uint16_t size){
    backend->send(this,data,size);
}
void UDPSocket::disconnect(){
    backend->disconnect(this);
}
UDPProvider::UDPProvider(IP4Provider* back):Ip4Handler(back,0x11),_backend(back){
  for(int i = 0; i < 0xffff; i++)
        sockets[i] = 0;
   numSockets=0;     
   freePort=1024;
}
UDPProvider::~UDPProvider(){

}  
bool UDPProvider::onPacketReceived(uint32_t srcIp,uint32_t dstIp,uint8_t *payload, uint32_t size){
     if(size < sizeof(DATAGRAM))
        return false;
    DATAGRAM* dg=(DATAGRAM*)payload;
    UDPSocket* socket=0;
    for(uint16_t i = 0; i < numSockets && socket == 0; i++)
    {   if(sockets[i]->srcPort==dg->dstPort&&
            sockets[i]->srcIp==dstIp&&
            socket->listening){
                socket=sockets[i];
                socket->listening=false;
                socket->dstIp=srcIp;
                socket->dstPort=dg->srcPort;
            }

        else if(sockets[i]->srcPort==dg->dstPort&&
            sockets[i]->dstPort==dg->srcPort&&
            sockets[i]->srcIp==dstIp&&
            sockets[i]->dstIp==srcIp
        )socket=sockets[i];

    }
    if(socket!=0)socket->handleDatagram(payload+sizeof(DATAGRAM),size-sizeof(DATAGRAM));
    return false;
}
void UDPProvider::disconnect(UDPSocket* socket){

    for(uint16_t i = 0; i < numSockets && socket == 0; i++)
    {  
         if(sockets[i] == socket)
        {
            sockets[i] = sockets[--numSockets];
            MemoryManager::activeMemMgr->free(socket);
            break;
        }
    }  
}
UDPSocket* UDPProvider::connect(uint32_t ip,uint16_t port){
    UDPSocket* socket=(UDPSocket*)  MemoryManager::activeMemMgr->malloc(sizeof(UDPSocket));
    if(socket!=0)
    {
        new (socket) UDPSocket(this);
        socket->dstPort=port;
        socket->dstIp=ip;
        socket->srcIp=_backend->getMyIp();
        socket->srcPort=freePort++;
        socket -> dstPort = ((socket->dstPort & 0xFF00)>>8) | ((socket->dstPort & 0x00FF) << 8);
        socket -> srcPort = ((socket->srcPort & 0xFF00)>>8) | ((socket->srcPort & 0x00FF) << 8);
        sockets[numSockets++]=socket;
    }   
    return socket; 
}
UDPSocket* UDPProvider::listen(uint16_t port){
    UDPSocket* socket=(UDPSocket*)  MemoryManager::activeMemMgr->malloc(sizeof(UDPSocket));
    if(socket!=0)
    {
        new (socket) UDPSocket(this);
        socket->listening=true;
        socket->srcIp=_backend->getMyIp();
        socket->srcPort=port;
        socket -> dstPort = ((socket->dstPort & 0xFF00)>>8) | ((socket->dstPort & 0x00FF) << 8);
        socket -> srcPort = ((socket->srcPort & 0xFF00)>>8) | ((socket->srcPort & 0x00FF) << 8);
        sockets[numSockets++]=socket;
    }   
    return socket; 
}
void UDPProvider::send(UDPSocket* socket,uint8_t*data,uint16_t size){
    uint16_t totLength=size+sizeof(DATAGRAM);
    uint8_t* buffer=(uint8_t*)MemoryManager::activeMemMgr->malloc(totLength);
    uint8_t* buffer2 = buffer + sizeof(DATAGRAM);
    DATAGRAM* dg=(DATAGRAM*)buffer; 
    dg->srcPort=socket->srcPort;
    dg->dstPort=socket->dstPort; 
    dg->length= ((totLength & 0x00FF) << 8) | ((totLength & 0xFF00) >> 8);
    for(int i = 0; i < size; i++)
        buffer2[i] = data[i];
    dg->checksum = 0;   
    Ip4Handler::send(socket->dstIp,buffer,totLength); 
    MemoryManager::activeMemMgr->free(buffer);
}
void UDPProvider::bind(UDPSocket* socket, UDPHandler* handler)
{
    socket->handler = handler;
}
