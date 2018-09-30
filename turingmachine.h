#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QScrollArea>
#include <QTableWidget>

#include "bubblewindow.h"
#include "turingline.h"
#include "controlBar.h"
#include "core.h"

namespace Ui {
class TuringMachine;
}

class TuringMachine : public QMainWindow
{
    Q_OBJECT

public:
    explicit TuringMachine(QWidget* parent = 0);
    ~TuringMachine();

private:
    BubbleWindow* _bubble_window;
    QScrollArea* _bubble_window_sa;

    TuringLine* _line;
    QTableWidget* _line_widget;
    QScrollArea* _line_sa;
    Core* _core;

    Ui::TuringMachine *ui;

    int centerAndResizeWindow();
};

#endif // TURINGMACHINE_H
