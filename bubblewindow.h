#ifndef BUBBLEWINDOW_H
#define BUBBLEWINDOW_H

#include <QGLWidget>
#include <bubble.h>
#include <set>
#include <math.h>

class BubbleWindow : public QGLWidget
{
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    void DrawBubble(Bubble*);
    std::set<Bubble*>* _bubble_set;
public:
    // set of bubbles
    std::set<Bubble*>* GetBubbleSet();
    void SetBubbleSet(std::set<Bubble*>*);

    BubbleWindow(QWidget* pwgt = 0);
};

#endif // BUBBLEWINDOW_H
