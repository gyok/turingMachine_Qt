#ifndef BUBBLEWINDOW_H
#define BUBBLEWINDOW_H

#include <QOpenGLWidget>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QScrollArea>
#include <QPainter>
#include <set>
#include <math.h>
#include <iostream>
#include "bubble.h"
#include "bubbleconnectionmanager.h"
#include "turingline.h"
#include "core.h"

class BubbleWindow : public QOpenGLWidget
{
    Q_OBJECT

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

    void DrawBubble(Bubble*, QPainter*);
    double PointDistance(QPoint, QPoint);
    int InRange(int min, int current, int max);
    void RenameBubble(Bubble*);
    void SkipRenameBubble(bool saveBubbleName);
    void DrawArrowToPoint(Bubble*, QPoint*);
    void DrawArrowTextDescription(Bubble* bubble_from, Bubble* bubble_to, QString font_name, QPainter*);
    Bubble* FindBubbleAtPoint(QPoint, bool*);
    bool BubbleArrowConnect(Bubble*, Bubble*, QPainter*);

    QScrollArea* _scrollArea;
    bool _rename_bubble_mode = false;
    int* _bubble_count;
    bool* _bubbleDrag;
    bool* _bubbleConnect;
    int* _max_id;
    QString _rename_bubble_name = "";
    QString _before_rename_bubble_name = "";
    Bubble* _rename_bubble;
    Bubble* _dragged_bubble;
    Bubble* _connecting_bubble;
    std::set<Bubble*>* _bubble_set;
    std::set<Bubble*>* _selected_bubble_set;

    // bubbles which starts end finish execution of program
    Bubble* _start_bubble = 0;
    Bubble* _finish_bubble = 0;
    Core* _core;

    QFont* _name_label_font;
    QPoint _current_position = QPoint(0,0);
    TuringLine* _turingLine;

signals:
    void BubbleMove(Bubble*);

public slots:
    void AddBubble();
    void DeleteSelectedBubbles();
    // move startup and end of program execution at selected bubble
    void MakeStartSelectedBubble();
    void MakeFinishSelectedBubble();

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

    // return new id for bubble
    int GetNewId();

    BubbleWindow(Core*, TuringLine*, QWidget* pwgt = 0);
};

#endif // BUBBLEWINDOW_H
