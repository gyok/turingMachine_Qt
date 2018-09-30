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

    QSurfaceFormat fmt;
    fmt.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(fmt);

    _line = new TuringLine;
    ControlBar* controlBarManager = new ControlBar(this);
    QVBoxLayout* controlBar = new QVBoxLayout(parent);
    QVBoxLayout* leftLineControl = new QVBoxLayout(parent);
    QVBoxLayout* rightLineControl = new QVBoxLayout(parent);
    // QVBoxLayout* controlButton = new QVBoxLayout(parent);
    QHBoxLayout* turingLineControl = new QHBoxLayout(parent);
    QPushButton* addLeftCell = new QPushButton("+", this);
    QPushButton* removeLeftCell = new QPushButton("-", this);
    QPushButton* addRightCell = new QPushButton("+", this);
    QPushButton* removeRightCell = new QPushButton("-", this);
    QPushButton* runButton = controlBarManager->GetRunButton();
    QPushButton* pauseButton = controlBarManager->GetPauseButton();

    leftLineControl->addWidget(removeLeftCell);
    leftLineControl->addWidget(addLeftCell);
    rightLineControl->addWidget(addRightCell);
    rightLineControl->addWidget(removeRightCell);
    controlBar->addWidget(runButton);
    controlBar->addWidget(pauseButton);

    turingLineControl->addLayout(leftLineControl);
    turingLineControl->addWidget(_line);
    turingLineControl->addLayout(rightLineControl);
    turingLineControl->addLayout(controlBar);

    connect(runButton, SIGNAL(clicked(bool)), controlBarManager, SLOT(RunButtonClicked()));
    connect(pauseButton, SIGNAL(clicked(bool)), controlBarManager, SLOT(PauseButtonClicked()));
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

    QPushButton *startBulbButton = new QPushButton(QIcon("../TuringMachine/img/start_bubble.xpm"), "&start bubble", this);
    QPushButton *finishBulbButton = new QPushButton(QIcon("../TuringMachine/img/finish_bubble.xpm"), "&finish bubble", this);
    connect(startBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(MakeStartSelectedBubble()));
    connect(finishBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(MakeFinishSelectedBubble()));

    QVBoxLayout *fullLayout = new QVBoxLayout;
    QHBoxLayout *buttonControlPanelLayout = new QHBoxLayout;
    buttonControlPanelLayout->addWidget(addBulbButton);
    buttonControlPanelLayout->addWidget(startBulbButton);
    buttonControlPanelLayout->addWidget(finishBulbButton);
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
