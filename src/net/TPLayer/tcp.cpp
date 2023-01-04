#include <MemoryManager.h>
#include <net/TPLayer/tcp.h>
using namespace mos::net::TPLayer;
void printfHex(int16_t hex);
void printf(int8_t *str);

uint32_t bigEndian32(uint32_t x)
{
    return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
}
TCPHandler::TCPHandler() {}
TCPHandler::~TCPHandler() {}
bool TCPHandler::handleSegment(TCPSocket *socket, uint8_t *data, uint16_t size) { return true; }
TCPSocket::TCPSocket(TCPProvider *back) : backend(back), handler(0), state(CLOSED) {}
TCPSocket::~TCPSocket() {}
bool TCPSocket::handleSegment(uint8_t *data, uint16_t size)
{
    if (handler != 0)
        return handler->handleSegment(this, data, size);
    return true;
}
void TCPSocket::send(uint8_t *data, uint16_t size)
{
    backend->send(this, data, size,ACK|PSH);
}
void TCPSocket::disconnect()
{
    backend->disconnect(this);
}
TCPProvider::TCPProvider(IP4Provider *back) : Ip4Handler(back, 0x06), _backend(back)
{
    for (int i = 0; i < 0xffff; i++)
        sockets[i] = 0;
    numSockets = 0;
    freePort = 1024;
}
TCPProvider::~TCPProvider()
{
}
bool TCPProvider::onPacketReceived(uint32_t srcIp, uint32_t dstIp, uint8_t *payload, uint32_t size)
{
    if (size < 20)
        return false;
    SEGMENT *seg = (SEGMENT *)payload;

    TCPSocket *socket = 0;
    for (uint16_t i = 0; i < numSockets && socket == 0; i++)
    {
        if (sockets[i]->srcPort == seg->dstPort &&
            sockets[i]->srcIp == dstIp &&
            socket->state == LISTENING && (((seg->flags) & (SYN | ACK)) == SYN))
            socket = sockets[i];
        else if (sockets[i]->srcPort == seg->dstPort &&
                 sockets[i]->dstPort == seg->srcPort &&
                 sockets[i]->srcIp == dstIp &&
                 sockets[i]->dstIp == srcIp)
            socket = sockets[i];
    }
    bool reset = false;
    if (socket != 0 && seg->flags & RST)
    {
        socket->state = CLOSED;
    }
    if (socket != 0 && socket->state != CLOSED)
    {
        switch ((seg->flags) & (ACK | SYN | FIN))
        {
        case SYN:
            if (socket->state == LISTENING)
            {
                socket->state = SYN_RECEIVED;
                socket->dstIp = srcIp;
                socket->dstPort = seg->srcPort;
                socket->ack = bigEndian32(seg->seqNum) + 1;
                socket->seqNum = 0xbeefcafe;
                send(socket, 0, 0, SYN | ACK);
                socket->seqNum++;
                return false;
            }
            else
                reset = true;
            break;
        case SYN | ACK:
            if (socket->state == SYN_SENT)
            {
                socket->state = ESTABLISHED;
                socket->ack = bigEndian32(seg->seqNum) + 1;
                socket->seqNum++;
                send(socket, 0, 0, ACK);
            }
            else
                reset = true;
            break;
        case SYN | FIN:
        case SYN | FIN | ACK:
            reset = true;
            break;
        case FIN:
        case FIN | ACK:
            if (socket->state == ESTABLISHED)
            {
                socket->state = CLOSE_WAIT;
                socket->ack++;
                send(socket, 0, 0, ACK);
                send(socket, 0, 0, ACK | FIN);
            }
            else if (socket->state == CLOSE_WAIT)
            {
                socket->state = CLOSED;
            }
            else if (socket->state == FIN_WAIT1 || socket->state == FIN_WAIT2)
            {
                socket->state = CLOSED;
                socket->ack++;
                send(socket, 0, 0, ACK);
            }
            else
                reset = true;

            break;
        case ACK:
            if (socket->state == SYN_RECEIVED)
            {
                socket->state = ESTABLISHED;
                return false;
            }
            else if (socket->state == FIN_WAIT1)
            {
                socket->state = FIN_WAIT2;
                return false;
            }
            else if (socket->state == CLOSE_WAIT

            )
            {
                socket->state = CLOSED;
                return false;
            }
            break;
        default:
            if (bigEndian32(seg->seqNum) == socket->ack)
            {
                reset = !socket->handleSegment(payload + seg->headerSize * 4, size - seg->headerSize * 4);
                if (!reset)
                {
                    socket->ack += size - seg->headerSize * 4;
                    send(socket, 0, 0, ACK);
                }
            }
            else
            {
                reset = true;
            }
            break;
        }
    }
    if (reset)
    {
        if(socket != 0)
        {
            send(socket, 0,0, RST);
        }
        else
        {
            TCPSocket socket(this);
            socket.dstPort = seg->srcPort;
            socket.dstIp = srcIp;
            socket.srcPort = seg->dstPort;
            socket.srcIp = dstIp;
            socket.seqNum = bigEndian32(seg->ack);
            socket.ack = bigEndian32(seg->seqNum) + 1;
            send(&socket, 0,0, RST);
        }
    }
    if (socket != 0 && socket->state == CLOSED)
    {
        for (uint16_t i = 0; i < numSockets && socket == 0; i++)
        {
            if (sockets[i] == socket)
            {
                sockets[i] = sockets[--numSockets];
                MemoryManager::activeMemMgr->free(socket);
                break;
            }
        }
    }
    return false;
}
void TCPProvider::disconnect(TCPSocket *socket)
{

    socket->state = FIN_WAIT1;
    send(socket, 0, 0, FIN | ACK);
    socket->seqNum++;
}
TCPSocket *TCPProvider::connect(uint32_t ip, uint16_t port)
{
    TCPSocket *socket = (TCPSocket *)MemoryManager::activeMemMgr->malloc(sizeof(TCPSocket));
    if (socket != 0)
    {
        new (socket) TCPSocket(this);
        socket->dstPort = port;
        socket->dstIp = ip;
        socket->srcIp = _backend->getMyIp();
        socket->srcPort = freePort++;
        socket->dstPort = ((socket->dstPort & 0xFF00) >> 8) | ((socket->dstPort & 0x00FF) << 8);
        socket->srcPort = ((socket->srcPort & 0xFF00) >> 8) | ((socket->srcPort & 0x00FF) << 8);
        sockets[numSockets++] = socket;
        socket->state = SYN_SENT;
        socket->seqNum = 0xbeefcafe;
        send(socket, 0, 0, SYN);
    }
    return socket;
}
TCPSocket *TCPProvider::listen(uint16_t port)
{
    TCPSocket *socket = (TCPSocket *)MemoryManager::activeMemMgr->malloc(sizeof(TCPSocket));
    if (socket != 0)
    {
        new (socket) TCPSocket(this);
        socket->state = LISTENING;
        socket->srcIp = _backend->getMyIp();
        socket->srcPort = port;
        socket->srcPort = ((socket->srcPort & 0xFF00) >> 8) | ((socket->srcPort & 0x00FF) << 8);
        sockets[numSockets++] = socket;
    }
    return socket;
}
void TCPProvider::send(TCPSocket *socket, uint8_t *data, uint16_t size, uint16_t flags)
{
    uint16_t totLength = size + sizeof(SEGMENT);
    uint16_t totLengthPs = totLength + sizeof(SEGMENT_PSEUDO_HEADER);
    uint8_t *buffer = (uint8_t *)MemoryManager::activeMemMgr->malloc(totLengthPs);
    uint8_t *buffer2 = buffer + sizeof(SEGMENT) + sizeof(SEGMENT_PSEUDO_HEADER);
    SEGMENT_PSEUDO_HEADER *dgp = (SEGMENT_PSEUDO_HEADER *)(buffer);
    SEGMENT *dg = (SEGMENT *)(buffer + sizeof(SEGMENT_PSEUDO_HEADER));
    dg->headerSize = sizeof(SEGMENT) / 4;
    dg->srcPort = socket->srcPort;
    dg->dstPort = socket->dstPort;
    dg->ack = bigEndian32(socket->ack);
    dg->seqNum = bigEndian32(socket->seqNum);
    dg->reserved = 0;
    dg->flags = flags;
    dg->windowSize = 0xffff;
    dg->urgPtr = 0;
    dg->options = ((flags & SYN) != 0) ? 0xB4050402 : 0;
    socket->seqNum += size;
    for (int i = 0; i < size; i++)
        buffer2[i] = data[i];
    dg->checksum = 0;
    dgp->srcIp = socket->srcIp;
    dgp->dstIp = socket->dstIp;
    dgp->protocol = 0x0600;
    dgp->totalLen = ((totLength & 0x00FF) << 8) | ((totLength & 0xFF00) >> 8);
    dg->checksum = IP4Provider::checksum((uint16_t *)buffer, totLengthPs);
    Ip4Handler::send(socket->dstIp, (uint8_t *)(dg), totLength);
    MemoryManager::activeMemMgr->free(buffer);
}
void TCPProvider::bind(TCPSocket *socket, TCPHandler *handler)
{
    socket->handler = handler;
}
