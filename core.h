#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <iostream>
#include "bubble.h"
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
public:
    Core(TuringLine*, QWidget*);

    void SetCurrentBubble(Bubble*);
    void SetCurrentCellCode(int);
    void SetFinishBubbleId(int);

public slots:
    int Run();
    int Pause();
};

#endif // CORE_H
