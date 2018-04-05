#include "turingmachine.h"
#include "ui_turingmachine.h"

TuringMachine::TuringMachine() :
    ui(new Ui::TuringMachine)
{
    _parent = new QWidget();
    _bubble_window = new BubbleWindow(_parent);
    std::set<Bubble*>* bubble_set = new std::set<Bubble*>();
    bubble_set->insert(new Bubble(new QPoint(15, 15), new QColor(20, 22, 16), new float(15)));
    _bubble_window->SetBubbleSet(bubble_set);
    _bubble_window->resize(200, 200);

    QPushButton *addBulbButton = new QPushButton("add bulb", _parent);
    QPushButton *deleteBulbButton = new QPushButton("delete bulb", _parent);

    QVBoxLayout *fullLayout = new QVBoxLayout;
    QHBoxLayout *buttonControlPanelLayout = new QHBoxLayout;
    buttonControlPanelLayout->addWidget(addBulbButton);
    buttonControlPanelLayout->addWidget(deleteBulbButton);

    fullLayout->addLayout(buttonControlPanelLayout);

    _parent->setLayout(fullLayout);
    ui->setupUi(this);
}

TuringMachine::~TuringMachine()
{
    delete ui;
}
