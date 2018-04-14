#include "turingmachine.h"
#include "ui_turingmachine.h"

#define GRAPHICVIEW_W 2000
#define GRAPHICVIEW_H 2000

TuringMachine::TuringMachine(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::TuringMachine)
{
    ui->setupUi(this);
    QWidget* mainWindow = new QWidget;

    QGLFormat fmt;
    fmt.setSampleBuffers(true);
    fmt.setSamples(8);
    QGLFormat::setDefaultFormat(fmt);

    _bubble_window = new BubbleWindow(this);
    _bubble_window->resize(GRAPHICVIEW_W, GRAPHICVIEW_H);

    QPushButton *addBulbButton = new QPushButton("&add bubble", this);
    QPushButton *deleteBulbButton = new QPushButton("&delete bubble", this);
    connect(addBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(AddBubble()));
    connect(deleteBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(DeleteSelectedBubbles()));

    QVBoxLayout *fullLayout = new QVBoxLayout;
    QHBoxLayout *buttonControlPanelLayout = new QHBoxLayout;
    buttonControlPanelLayout->addWidget(addBulbButton);
    buttonControlPanelLayout->addWidget(deleteBulbButton);

    _bubble_window_sa = new QScrollArea();
    _bubble_window_sa->setBackgroundRole(QPalette::Light);
    _bubble_window_sa->setWidget(_bubble_window);
    _bubble_window->SetScrollArea(_bubble_window_sa);

    fullLayout->addWidget(_bubble_window_sa);
    fullLayout->addLayout(buttonControlPanelLayout);

    mainWindow->setLayout(fullLayout);
    setCentralWidget(mainWindow);
}

TuringMachine::~TuringMachine()
{
    delete ui;
}
