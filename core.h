#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <iostream>
#include <QtConcurrent/QtConcurrent>
#include "bubble.h"
#include "controlBar.h"
#include "turingline.h"

class Core : public QObject
{
    Q_OBJECT
private:
    int start();
    int stop();
    int pause();

    int execute();

    Bubble* _current_bubble;
    int _finish_bubble_id = -1;
    int _current_cell_code;
    TuringLine* _turing_line;
    ControlBar* _control_bar;
    int run();
public:
    Core(TuringLine*, QWidget*, ControlBar*);

    void SetCurrentBubble(Bubble*);
    void SetCurrentCellCode(int);
    void SetFinishBubbleId(int);

public slots:
    int Run();
    int Stop();
    int Pause();
};

#endif // CORE_H
