#include "bubbleconnection.h"

BubbleConnection::BubbleConnection(Bubble* nextBubble, BubbleConnectionLine* connectionInfo)
{
    _nextBubble = nextBubble;
    _connectionInfo = connectionInfo;
}


Bubble* BubbleConnection::GetNextBubble() {
    return _nextBubble;
}


BubbleConnectionLine* BubbleConnection::GetConnectionInfo() {
    return _connectionInfo;
}
