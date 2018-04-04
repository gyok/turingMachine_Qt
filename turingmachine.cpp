#include "turingmachine.h"
#include "ui_turingmachine.h"

TuringMachine::TuringMachine() :
    ui(new Ui::TuringMachine)
{
    _parent = new QWidget();
    _openGL_screen = new QOpenGLWidget();
    QPushButton *addBulbButton = new QPushButton("add bulb", _parent);
    QPushButton *deleteBulbButton = new QPushButton("delete bulb", _parent);

    QVBoxLayout *fullLayout = new QVBoxLayout;
    QHBoxLayout *buttonControlPanelLayout = new QHBoxLayout;
    buttonControlPanelLayout->addWidget(addBulbButton);
    buttonControlPanelLayout->addWidget(deleteBulbButton);

    fullLayout->addWidget(_openGL_screen);
    fullLayout->addLayout(buttonControlPanelLayout);

    _parent->setLayout(fullLayout);
    ui->setupUi(this);

    _parent->show();
}

TuringMachine::~TuringMachine()
{
    delete ui;
}
