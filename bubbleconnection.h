#ifndef BUBBLECONNECTION_H
#define BUBBLECONNECTION_H

#include "bubble.h"
#include "bubbleconnectionline.h"

class BubbleConnection
{
public:
    BubbleConnection(Bubble*, BubbleConnectionLine*);

    Bubble* GetNextBubble();
    BubbleConnectionLine* GetConnectionInfo();
private:
    Bubble* _nextBubble;
    BubbleConnectionLine* _connectionInfo;
};

#endif // BUBBLECONNECTION_H
