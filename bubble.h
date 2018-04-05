#ifndef BUBBLE_H
#define BUBBLE_H

#include <QPoint>
#include <QColor>

class Bubble
{
private:
    QPoint* _position;
    QColor* _color;
    QString* _name;
    float* _size;
public:
    // position of bubble at bubble map
    QPoint* GetPosition();
    void SetPosition(QPoint*);

    // color of bubble
    QColor* GetColor();
    void SetColor(QColor*);

    // name of bubble
    QString* GetName();
    void SetName(QString*);

    // size of bubble
    float* GetBubbleSize();
    void SetBubbleSize(float*);

    Bubble(QPoint*, QColor*, float*);
    ~Bubble();
};

#endif // BUBBLE_H
