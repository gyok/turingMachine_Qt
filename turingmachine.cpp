#include "turingmachine.h"
#include "ui_turingmachine.h"

#define GRAPHICVIEW_W 2000
#define GRAPHICVIEW_H 2000
#define UNABLE_OPEN_FILE_FOR_WRITE 1
#define UNABLE_OPEN_FILE_FOR_READ 3
#define FILE_NOT_SELECTED 2
#define XML_READER_ERROR 4

TuringMachine::TuringMachine(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::TuringMachine)
{
    ui->setupUi(this);
    delete ui->mainToolBar;
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

    _core = new Core(_line, this);
    _bubble_window = new BubbleWindow(_core, _line, this);
    _bubble_window->resize(GRAPHICVIEW_W, GRAPHICVIEW_H);

    connect(runButton, SIGNAL(clicked(bool)), _core, SLOT(Run()));
    connect(pauseButton, SIGNAL(clicked(bool)), _core, SLOT(Pause()));

    QPushButton *addBulbButton = new QPushButton("&add bubble", this);
    QPushButton *deleteBulbButton = new QPushButton("&delete bubble", this);
    connect(addBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(AddBubble()));
    connect(deleteBulbButton, SIGNAL(clicked(bool)), this->_bubble_window, SLOT(DeleteSelectedBubbles()));

    QPushButton *startBulbButton = new QPushButton(QIcon("../TuringMachine/img/start_bubble.xpm"), "&begin bubble", this);
    QPushButton *finishBulbButton = new QPushButton(QIcon("../TuringMachine/img/finish_bubble.xpm"), "&end bubble", this);
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

//     adding menubar
     QMenu* file_menu = new QMenu("&File", ui->menuBar);
     file_menu->addAction(QIcon("../TuringMachine/img/load.xpm"), "&load", this, SLOT(LoadBubbles()));
     file_menu->addAction(QIcon("../TuringMachine/img/save.xpm"), "&save", this, SLOT(SaveBubbles()));
     file_menu->addAction(QIcon("../TuringMachine/img/exit.xpm"), "&exit", this, SLOT(ExitProgram()));
     ui->menuBar->addMenu(file_menu);

    mainWindow->setLayout(fullLayout);
    setCentralWidget(mainWindow);
    centerAndResizeWindow();
}

TuringMachine::~TuringMachine()
{
    delete ui;
}

int TuringMachine::SaveBubbles() {
    QString file_name = QFileDialog::getSaveFileName(this,
            QString("Save file"),
            QString(QDir::homePath() + "/document/document/tmSave/tm_save.xml"),
            QString("XML file (*.xml)"));
    if (file_name.isEmpty()) return FILE_NOT_SELECTED;
    QFile save_file(file_name, this);
    if (!save_file.open(QIODevice::WriteOnly | QIODevice::Text)) return UNABLE_OPEN_FILE_FOR_WRITE;
    QXmlStreamWriter* save_stream_writer = new QXmlStreamWriter(&save_file);
    save_stream_writer->setAutoFormatting(true);
    save_stream_writer->writeStartDocument();
    save_stream_writer->writeStartElement("bubble-set");
    for (set<Bubble*>::iterator it= _bubble_window->GetBubbleSet()->begin();
             it != _bubble_window->GetBubbleSet()->end(); it++) {
        save_stream_writer->writeStartElement("bubble");
        save_stream_writer->writeTextElement(QString("id"), QString::number((*it)->GetBubbleId()));
        save_stream_writer->writeTextElement(QString("name"), *(*it)->GetName());
        save_stream_writer->writeTextElement(QString("size"), QString::number(*(*it)->GetBubbleSize()));
        save_stream_writer->writeStartElement("position");
        save_stream_writer->writeTextElement(QString("x"), QString::number((*it)->GetPosition()->x()));
        save_stream_writer->writeTextElement(QString("y"), QString::number((*it)->GetPosition()->y()));
        save_stream_writer->writeEndElement(); // position
        save_stream_writer->writeTextElement(QString("default-color"), (*it)->GetDefaultColor()->name());
        save_stream_writer->writeTextElement(QString("select-color"), (*it)->GetSelectColor()->name());
        save_stream_writer->writeStartElement("connected-bubbles");
        for (set<Bubble*>::iterator connected_bubble= (*it)->GetConnectionBubbleSet()->begin();
             connected_bubble != (*it)->GetConnectionBubbleSet()->end(); connected_bubble++) {
            save_stream_writer->writeTextElement(QString("connected-bubble-id"),
                QString::number((*connected_bubble)->GetBubbleId()));
            save_stream_writer->writeStartElement("connected-bubble-commands");
            set<BubbleConnectionLine*>* connection_info_set = (*(*it)->GetConnectionInfo())[(*connected_bubble)->GetBubbleId()];
            for (set<BubbleConnectionLine*>::iterator connection_it = connection_info_set->begin();
                 connection_it != connection_info_set->end();
                 connection_it++) {
                save_stream_writer->writeStartElement("connected-bubble-command");
                save_stream_writer->writeTextElement(QString("symbol-before-line"),
                    (*connection_it)->GetSymbolBeforeLine()->text());
                save_stream_writer->writeTextElement(QString("way-to-move"),
                    (*connection_it)->GetSelectedWay());
                save_stream_writer->writeTextElement(QString("symbol-after-line"),
                    (*connection_it)->GetSymbolAfterLine()->text());
                save_stream_writer->writeEndElement(); // connected-bubble-command
            }
            save_stream_writer->writeEndElement(); // connected-bubble-commands
        }
        save_stream_writer->writeEndElement(); // connected-bubbles
        save_stream_writer->writeEndElement(); // bubble
    }
    save_stream_writer->writeEndElement(); // bubble-set
    save_stream_writer->writeEndDocument();
    save_file.close();

    return 0;
}


int TuringMachine::LoadBubbles() {
    QString file_name = QFileDialog::getOpenFileName(this,
            QString("Load file"),
            QString(QDir::homePath() + "/document/document/tmSave/tm_save.xml"),
            QString("XML file (*.xml)"));
    QFile save_file(file_name, this);
    if (file_name.isEmpty()) return FILE_NOT_SELECTED;
    if (!save_file.open(QIODevice::ReadOnly | QIODevice::Text)) return UNABLE_OPEN_FILE_FOR_READ;
    QXmlStreamReader* save_stream_reader = new QXmlStreamReader(&save_file);
    int id = 0;
    QPoint position = QPoint(0, 0);
    QColor color = QColor(0, 0, 0);
    QColor select_color = QColor(0, 0, 0);
    QString name = QString("default");
    double size = 15;
    set<Bubble*>* bubble_set = new set<Bubble*>();

    while (!save_stream_reader->atEnd()) {
        if (save_stream_reader->readNext() == QXmlStreamReader::EndDocument) break;
        if (save_stream_reader->isStartElement()
            && save_stream_reader->name().toString().toStdString() == "connected-bubbles") {
            Bubble* bubble = new Bubble(
                new int(id),
                new QPoint(position),
                new QColor(color),
                new QString(name),
                new double(size));
            bubble->SetDefaultColor(new QColor(color));
            bubble->SetSelectColor(new QColor(select_color));
            readBubbleConnections(bubble, save_stream_reader);
            bubble_set->insert(bubble);
        }
        if (!save_stream_reader->isStartElement()) continue;
        if (save_stream_reader->isStartElement()
            && save_stream_reader->name().toString().toStdString() == "bubble") {
            id = 0;
            position = QPoint(0, 0);
            color = QColor(0, 0, 0);
            select_color = QColor(0, 0, 0);
            name = QString("default");
            size = 15;
        }
        if (save_stream_reader->name().toString().toStdString() == "id") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            id = save_stream_reader->readElementText().toInt();
        }
        if (save_stream_reader->name().toString().toStdString() == "name") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            name = save_stream_reader->readElementText();
        }
        if (save_stream_reader->name().toString().toStdString() == "size") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            size = save_stream_reader->readElementText().toDouble();
        }
        if (save_stream_reader->name().toString().toStdString() == "x") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            position.setX(save_stream_reader->readElementText().toInt());
        }
        if (save_stream_reader->name().toString().toStdString() == "y") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            position.setY(save_stream_reader->readElementText().toInt());
        }
        if (save_stream_reader->name().toString().toStdString() == "default-color") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            color.setNamedColor(save_stream_reader->readElementText());
        }
        if (save_stream_reader->name().toString().toStdString() == "select-color") {
//            cout << save_stream_reader->readElementText().toStdString() << endl;
            select_color.setNamedColor(save_stream_reader->readElementText());
        }
//        if (save_stream_reader->name().toString().toStdString() == "connected-bubbles") {
//            cout << save_stream_reader->name().toString().toStdString() << endl;
//        }
    }
    // connecting bubbles with each other

    _bubble_window->SetBubbleSet(bubble_set);
    for (set<Bubble*>::iterator it = bubble_set->begin(); it != bubble_set->end(); it++) {
        set<Bubble*>* connection_set = new set<Bubble*>();
        for (const auto& connection_info: *(*it)->GetConnectionInfo()) {
            for (const auto& bubble : *bubble_set) {
                if (bubble->GetBubbleId() == connection_info.first) {
                    connection_set->insert(bubble);
                }
            }
        }
        (*it)->SetConnectionBubbleSet(connection_set);
    }

    if (save_stream_reader->hasError()) {
        cout << save_stream_reader->errorString().toStdString() << endl;
        return XML_READER_ERROR;
    }
    save_file.close();
    _bubble_window->update();
    return 0;
}


int TuringMachine::ExitProgram() {
    QCoreApplication::quit();
    return 0;
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


int TuringMachine::readBubbleConnections(
        Bubble* bubble, QXmlStreamReader* save_stream_reader) {
    int current_connected_bubble = 0;
    while (!save_stream_reader->isEndElement()
           || !(save_stream_reader->name().toString().toStdString() == "connected-bubbles")) {
        save_stream_reader->readNext();
        if (!save_stream_reader->isStartElement()) continue;
        if (save_stream_reader->name().toString().toStdString() == "connected-bubble-id") {
            current_connected_bubble = save_stream_reader->readElementText().toInt();
        }
        if (save_stream_reader->name().toString().toStdString() == "connected-bubble-commands") {
            // read connection commands
            (*bubble->GetConnectionInfo())[current_connected_bubble] = new set<BubbleConnectionLine*>();
            QString symbol_before = QString("");
            QString way_to_move = QString("");
            QString symbol_after = QString("");
            while (!save_stream_reader->isEndElement()
                   || !(save_stream_reader->name().toString().toStdString() == "connected-bubble-commands")) {
                save_stream_reader->readNext();
                if (save_stream_reader->isStartElement()
                    && save_stream_reader->name().toString().toStdString() == "symbol-before-line") {
                    symbol_before = save_stream_reader->readElementText();
                }
                if (save_stream_reader->isStartElement()
                    && save_stream_reader->name().toString().toStdString() == "way-to-move") {
                    way_to_move = save_stream_reader->readElementText();
                }
                if (save_stream_reader->isStartElement()
                    && save_stream_reader->name().toString().toStdString() == "symbol-after-line") {
                    symbol_after = save_stream_reader->readElementText();
                }
                if (save_stream_reader->isEndElement()
                    && save_stream_reader->name().toString().toStdString() == "connected-bubble-command") {
                    BubbleConnectionLine* bubble_connection_line = new BubbleConnectionLine(_bubble_window, _line);
                    bubble_connection_line->GetSymbolBeforeLine()->setText(symbol_before);
                    bubble_connection_line->GetComboBox()->setCurrentIndex(way_to_move == QString("L")
                                                                           ? 0
                                                                           : way_to_move == QString("R")
                                                                             ? 1
                                                                             : 2);
                    bubble_connection_line->GetSymbolAfterLine()->setText(symbol_after);
                    (*bubble->GetConnectionInfo())[current_connected_bubble]->insert(bubble_connection_line);
                }
            }
        }
    }
    map<int, set<BubbleConnectionLine*>*>* d = bubble->GetConnectionInfo();
    return 0;
}
