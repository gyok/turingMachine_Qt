#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPoint>
#include <QColor>
#include <set>
#include <map>
#include <iostream>
#include "bubbleconnectionline.h"

using namespace std;

class Bubble
{
private:
    QPoint* _position;
    QColor* _color;
    QColor* _default_color;
    QColor* _select_color;
    QString* _name;
    float* _size;
    int* _id;
    set<Bubble*>* _connection_set;
    map<int, set<BubbleConnectionLine*>*>* _conn_info;
public:
    // position of bubble at bubble map
    QPoint* GetPosition();
    void SetPosition(QPoint*);

    // color of bubble
    QColor* GetColor();
    void SetColor(QColor*);

    QColor* GetDefaultColor();
    void SetDefaultColor(QColor*);

    QColor* GetSelectColor();
    void SetSelectColor(QColor*);

    // name of bubble
    QString* GetName();
    void SetName(QString*);

    // size of bubble
    float* GetBubbleSize();
    void SetBubbleSize(float*);

    // set of connections from this bubble to another ( this_bubble -> another_bubble)
    set<Bubble*>* GetConnectionBubbleSet();
    bool SetConnectionBubbleSet(set<Bubble*>*);

    // map with information what to do when see new bubble
    map<int, set<BubbleConnectionLine*>*>* GetConnectionInfo();

    int GetBubbleId();

    Bubble(int*, QPoint* = new QPoint(15, 15), QColor* = new QColor(112, 122, 116), QString* = new QString("default"), float* = new float(15));
    ~Bubble();
};

#endif // BUBBLE_H
