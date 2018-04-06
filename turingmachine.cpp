#include "turingmachine.h"
#include "ui_turingmachine.h"

TuringMachine::TuringMachine(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::TuringMachine)
{
    ui->setupUi(this);
    QWidget* mainWindow = new QWidget;
    _bubble_window = new BubbleWindow(this);
    _bubble_window->SetBubbleSet(new std::set<Bubble*>());

    QPushButton *addBulbButton = new QPushButton("add bubble", this);
    QPushButton *deleteBulbButton = new QPushButton("delete bubble", this);
    connect(addBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(AddBubble()));

    QVBoxLayout *fullLayout = new QVBoxLayout;
    QHBoxLayout *buttonControlPanelLayout = new QHBoxLayout;
    buttonControlPanelLayout->addWidget(addBulbButton);
    buttonControlPanelLayout->addWidget(deleteBulbButton);

    fullLayout->addWidget(_bubble_window);
    fullLayout->addLayout(buttonControlPanelLayout);

    mainWindow->setLayout(fullLayout);
    setCentralWidget(mainWindow);
}

TuringMachine::~TuringMachine()
{
    delete ui;
}
