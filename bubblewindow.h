#ifndef BUBBLEWINDOW_H
#define BUBBLEWINDOW_H

#include <QGLWidget>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QScrollArea>
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
    int InRange(int min, int current, int max);

    Bubble* FindBubbleAtPoint(QPoint, bool*);
    bool BubbleArrowConnect(Bubble*, Bubble*);

    QScrollArea* _scrollArea;
    int* _bubble_count;
    bool* _bubbleDrag;
    bool* _bubbleConnect;
    Bubble* _draggedBubble;
    Bubble* _connectingBubble;
    QColor _selected_bubble_color = QColor(144, 14, 249);
    QColor _default_bubble_color = QColor(112, 122, 116);
    std::set<Bubble*>* _bubble_set;
    std::set<Bubble*>* _selected_bubble_set;
    QFont* _name_label_font;

signals:
    void BubbleMove(Bubble*);

public slots:
    void AddBubble();
    void DeleteSelectedBubbles();

public:
    // set of bubbles
    std::set<Bubble*>* GetBubbleSet();
    void SetBubbleSet(std::set<Bubble*>*);

    // set of selected bubbles
    std::set<Bubble*>* GetSelectedBubbleSet();
    void SetSelectedBubbleSet(std::set<Bubble*>*);

    // add thecond bubble to connection
    bool ConnectBubble(Bubble*);
    // add first bubble to connection
    bool StartConnectBubble(Bubble*);
    bool StopConnectBubble();

    bool StartDragBubble(Bubble*);
    bool StopDragBubble();


    void SelectBubble(Bubble*);
    void DeselectBubble(Bubble*);

    void SetScrollArea(QScrollArea*);

    BubbleWindow(QWidget* pwgt = 0);
};

#endif // BUBBLEWINDOW_H
