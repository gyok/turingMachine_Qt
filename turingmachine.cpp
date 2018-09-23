#include "turingmachine.h"
#include "ui_turingmachine.h"

#define GRAPHICVIEW_W 2000
#define GRAPHICVIEW_H 2000
#define CONTROL_BUTTON_SIZE 25

TuringMachine::TuringMachine(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::TuringMachine)
{
    ui->setupUi(this);
    QWidget* mainWindow = new QWidget;

    QSurfaceFormat fmt;
    fmt.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

    _line = new TuringLine;
    QVBoxLayout* leftLineControl = new QVBoxLayout(parent);
    QVBoxLayout* rightLineControl = new QVBoxLayout(parent);
    QVBoxLayout* controlButton = new QVBoxLayout(parent);
    QHBoxLayout* turingLineControl = new QHBoxLayout(parent);
    QPushButton* addLeftCell = new QPushButton("+", this);
    QPushButton* removeLeftCell = new QPushButton("-", this);
    QPushButton* addRightCell = new QPushButton("+", this);
    QPushButton* removeRightCell = new QPushButton("-", this);
    QPushButton* startButton = new QPushButton(this);
    QPushButton* stopButton = new QPushButton(this);
    startButton->setIcon(QIcon("../TuringMachine/img/start.xpm"));
    stopButton->setIcon(QIcon("../TuringMachine/img/stop.xpm"));
    startButton->setIconSize(QSize(CONTROL_BUTTON_SIZE, CONTROL_BUTTON_SIZE));
    stopButton->setIconSize(QSize(CONTROL_BUTTON_SIZE, CONTROL_BUTTON_SIZE));
    leftLineControl->addWidget(removeLeftCell);
    leftLineControl->addWidget(addLeftCell);
    rightLineControl->addWidget(addRightCell);
    rightLineControl->addWidget(removeRightCell);
    controlButton->addWidget(startButton);
    controlButton->addWidget(stopButton);
    turingLineControl->addLayout(leftLineControl);
    turingLineControl->addWidget(_line);
    turingLineControl->addLayout(rightLineControl);
    turingLineControl->addLayout(controlButton);
    connect(addLeftCell, SIGNAL(clicked(bool)), _line, SLOT(AddCellFromLeft()));
    connect(removeLeftCell, SIGNAL(clicked(bool)), _line, SLOT(RemoveCellFromLeft()));
    connect(addRightCell, SIGNAL(clicked(bool)), _line, SLOT(AddCellFromRight()));
    connect(removeRightCell, SIGNAL(clicked(bool)), _line, SLOT(RemoveCellFromRight()));

    _bubble_window = new BubbleWindow(_line, this);
    _bubble_window->resize(GRAPHICVIEW_W, GRAPHICVIEW_H);

    QPushButton *addBulbButton = new QPushButton("&add bubble", this);
    QPushButton *deleteBulbButton = new QPushButton("&delete bubble", this);
    connect(addBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(AddBubble()));
    connect(deleteBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(DeleteSelectedBubbles()));

    QVBoxLayout *fullLayout = new QVBoxLayout;
    QHBoxLayout *buttonControlPanelLayout = new QHBoxLayout;
    buttonControlPanelLayout->addWidget(addBulbButton);
    buttonControlPanelLayout->addWidget(deleteBulbButton);

    _bubble_window_sa = new QScrollArea;
    _bubble_window_sa->setBackgroundRole(QPalette::Light);
    _bubble_window_sa->setWidget(_bubble_window);
    _bubble_window->SetScrollArea(_bubble_window_sa);

    fullLayout->addWidget(_bubble_window_sa);
    fullLayout->addLayout(buttonControlPanelLayout);
    fullLayout->addLayout(turingLineControl);

    mainWindow->setLayout(fullLayout);
    setCentralWidget(mainWindow);
    centerAndResizeWindow();
}

TuringMachine::~TuringMachine()
{
    delete ui;
}

int TuringMachine::centerAndResizeWindow()
{
    QRect desktopSize = QApplication::desktop()->screenGeometry();
    QSize appSize = QSize(desktopSize.width()*0.7, desktopSize.height()*0.7);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                          Qt::AlignCenter,
                                          appSize,
                                          desktopSize));

    return 0;
}
