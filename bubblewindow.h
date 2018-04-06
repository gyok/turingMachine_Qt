#ifndef BUBBLEWINDOW_H
#define BUBBLEWINDOW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <bubble.h>
#include <set>
#include <math.h>
#include <iostream>

class BubbleWindow : public QGLWidget
{
    Q_OBJECT

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    void DrawBubble(Bubble*);
    double PointDistance(QPoint, QPoint);

    bool* _bubbleDrag;
    Bubble* _draggedBubble;
    std::set<Bubble*>* _bubble_set;

signals:
    void BubbleMove(Bubble*);

public slots:
    void AddBubble();

public:
    // set of bubbles
    std::set<Bubble*>* GetBubbleSet();
    void SetBubbleSet(std::set<Bubble*>*);

    BubbleWindow(QWidget* pwgt = 0);
};

#endif // BUBBLEWINDOW_H
