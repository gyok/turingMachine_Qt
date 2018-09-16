#ifndef BUBBLECONNECTIONMANAGER_H
#define BUBBLECONNECTIONMANAGER_H

#include <QDialog>
#include <QPushButton>
#include <QTimer>
#include <set>
#include "linepushbutton.h"
#include "bubbleconnectionline.h"

class LinePushButton;

class BubbleConnectionManager : QDialog
{
    Q_OBJECT
public:
    BubbleConnectionManager(TuringLine*);
    std::set<BubbleConnectionLine*>* GetConnectionLineSet();
    int execWindow();
public slots:
    bool AddLine();
    bool RemoveLine(BubbleConnectionLine*, LinePushButton*);
    bool fixSize();
private:
    std::set<BubbleConnectionLine*>* _connectionLineSet;
    TuringLine* _connectedTuringLine;
    QVBoxLayout* _allConnectionLine;
    int* _line_count;
};

#endif // BUBBLECONNECTIONMANAGER_H
