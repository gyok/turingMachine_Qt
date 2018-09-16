#include "bubbleconnectionmanager.h"

BubbleConnectionManager::BubbleConnectionManager(TuringLine* turingLine) : QDialog()
{
    _connectionLineSet = new std::set<BubbleConnectionLine*>();
    _connectedTuringLine = turingLine;
    _allConnectionLine = new QVBoxLayout();
    QVBoxLayout* connectionManagerLayout = new QVBoxLayout();

    QPushButton* okButton = new QPushButton("&Ok", this);
    QPushButton* addButton = new QPushButton("&Add connection", this);
    connectionManagerLayout->addWidget(okButton);
    connectionManagerLayout->addWidget(addButton);
    connectionManagerLayout->addLayout(_allConnectionLine);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(AddLine()));
    _line_count = new int(0);
    this->setWindowTitle("Bubble connection manager");
    this->setLayout(connectionManagerLayout);
    AddLine();
}


bool BubbleConnectionManager::AddLine()
{
    ++(*_line_count);
    BubbleConnectionLine* currentLine = new BubbleConnectionLine(this, _connectedTuringLine);
    _connectionLineSet->insert(currentLine);
    LinePushButton* removeButton = new LinePushButton((char*)"&-", this, currentLine);
    connect(removeButton, &LinePushButton::clicked, removeButton, &LinePushButton::RemoveCurrentLine);

    QHBoxLayout* currentLineLayout = new QHBoxLayout();
    currentLineLayout->addWidget(currentLine->GetSymbolBeforeLine());
    currentLineLayout->addWidget(currentLine->GetComboBox());
    currentLineLayout->addWidget(currentLine->GetSymbolAfterLine());
    currentLineLayout->addWidget(removeButton);
    removeButton->SetLineLayout(currentLineLayout);

    _allConnectionLine->addLayout(currentLineLayout);

    return true;
}

bool BubbleConnectionManager::RemoveLine(BubbleConnectionLine* connection_line_for_delete, LinePushButton* current_line_remove_button) {
    _connectionLineSet->erase(connection_line_for_delete);
    _allConnectionLine->removeItem(current_line_remove_button->GetLineLayout());
    QTimer::singleShot(0, this, SLOT(fixSize()));

    return true;
}

bool BubbleConnectionManager::fixSize() {
    resize(sizeHint());

    return true;
}

std::set<BubbleConnectionLine*>* BubbleConnectionManager::GetConnectionLineSet() {
    return _connectionLineSet;
}


int BubbleConnectionManager::execWindow() {
    return exec();
}
