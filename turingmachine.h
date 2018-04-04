#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QtOpenGL>

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
    QOpenGLWidget* _openGL_screen;
    Ui::TuringMachine *ui;
};

#endif // TURINGMACHINE_H
