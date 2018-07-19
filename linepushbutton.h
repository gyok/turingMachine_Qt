#ifndef LINEPUSHBUTTON_H
#define LINEPUSHBUTTON_H

#include <QPushButton>
#include "bubbleconnectionline.h"
#include "bubbleconnectionmanager.h"

class LinePushButton : public QPushButton
{
public:
    BubbleConnectionLine* GetConnectionLine();
signals:


    LinePushButton(char*, QWidget*, BubbleConnectionLine*);
private:
    BubbleConnectionLine* _current_line;
};

#endif // LINEPUSHBUTTON_H
