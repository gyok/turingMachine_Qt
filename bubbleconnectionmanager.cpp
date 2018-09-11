#include "bubbleconnectionmanager.h"

BubbleConnectionManager::BubbleConnectionManager(TuringLine* turingLine) : QDialog()
{
    _connectionLineSet = new std::set<BubbleConnectionLine*>();
    _connectedTuringLine = turingLine;
    _allConnectionLine = new QVBoxLayout();
    QVBoxLayout* connectionManagerLayout = new QVBoxLayout();

    QPushButton* okButton = new QPushButton("&Ok", this);
    QPushButton* addButton = new QPushButton("&Add connection", this);
    connectionManagerLayout->addLayout(_allConnectionLine);
    connectionManagerLayout->addWidget(okButton);
    connectionManagerLayout->addWidget(addButton);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(AddLine()));
    _line_count = new int(0);

    this->setLayout(connectionManagerLayout);
    AddLine();
}


bool BubbleConnectionManager::AddLine()
{
    ++(*_line_count);
    BubbleConnectionLine* currentLine = new BubbleConnectionLine(this, _connectedTuringLine);
    _connectionLineSet->insert(currentLine);
    LinePushButton* removeButton = new LinePushButton("&-", this, currentLine);
    connect(removeButton, SIGNAL(clicked(bool)), currentLine, SLOT(RemoveConnectionLine()));

    QHBoxLayout* currentLineLayout = new QHBoxLayout();
    currentLineLayout->addWidget(currentLine->GetSymbolBeforeLine());
    currentLineLayout->addWidget(currentLine->GetComboBox());
    currentLineLayout->addWidget(currentLine->GetSymbolAfterLine());
    currentLineLayout->addWidget(removeButton);

    _allConnectionLine->addLayout(currentLineLayout);

    return true;
}

bool BubbleConnectionManager::RemoveLine(BubbleConnectionLine* connectionLineForDelete) {
    _connectionLineSet->erase(connectionLineForDelete);

    return true;
}


std::set<BubbleConnectionLine*>* BubbleConnectionManager::GetConnectionLineSet() {
    return _connectionLineSet;
}


int BubbleConnectionManager::execWindow() {
    return exec();
}
