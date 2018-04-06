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
    explicit TuringMachine(QWidget* parent = 0);
    ~TuringMachine();

private:
    BubbleWindow* _bubble_window;
    Ui::TuringMachine *ui;
};

#endif // TURINGMACHINE_H
