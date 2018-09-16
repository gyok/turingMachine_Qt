#ifndef LINEPUSHBUTTON_H
#define LINEPUSHBUTTON_H

#include <QPushButton>
#include "bubbleconnectionline.h"
#include "bubbleconnectionmanager.h"

class LinePushButton : public QPushButton
{
    Q_OBJECT
public:
    BubbleConnectionLine* GetConnectionLine();
    LinePushButton(char*, QWidget*, BubbleConnectionLine*);
    void SetLineLayout(QHBoxLayout*);
    QHBoxLayout* GetLineLayout();
public slots:
    void RemoveCurrentLine();
private:
    BubbleConnectionLine* _current_line;
    QHBoxLayout* _line_layout;
};

#endif // LINEPUSHBUTTON_H
