#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <bubblewindow.h>

namespace Ui {
class TuringMachine;
}

class TuringMachine : public QMainWindow
{
    Q_OBJECT

public:
    explicit TuringMachine();
    ~TuringMachine();

private:
    QWidget* _parent;
    BubbleWindow* _bubble_window;
    Ui::TuringMachine *ui;
};

#endif // TURINGMACHINE_H
