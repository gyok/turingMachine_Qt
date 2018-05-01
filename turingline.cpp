#include "turingline.h"
#define CELL_COUNT 100
#define LAMBDA "λ"


TuringLine::TuringLine(QWidget *parent) : QTableWidget(parent)
{
    std::cout << "line->width(): " << width() << std::endl;
    std::cout << "row count: " << rowCount() << " column count: " << columnCount() << std::endl;
    setRowCount(1);
    this->window()->width();
    setColumnCount(CELL_COUNT);
    for (int i = 0; i < CELL_COUNT; i++) {
        setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
    }
    for (int i = 0; i < CELL_COUNT; i++) {
        setItem(0, i, new QTableWidgetItem("λ"));
    }
    _left_border = 0;
    _right_border = CELL_COUNT;

    setFixedHeight(65);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    _current_cell = item(0, _current_cell_num);


    std::cout << "row count: " << rowCount() << ", column count: " << columnCount() << std::endl;
}

bool TuringLine::R() {
    QTableWidgetItem* current_cell = GetCurrentCell();
    if (columnCount() <= current_cell->column()) {
        AddCellFromRight();
    }

    SelectCell(_current_cell_num + 1);
    std::cout << _current_cell->text().toStdString() << std::endl;

    return true;
}

bool TuringLine::L() {
    if (_current_cell->column() == 0) {
        AddCellFromLeft();
    }

    SelectCell(_current_cell_num - 1);
    std::cout << _current_cell->text().toStdString() << std::endl;

    return true;
}

bool TuringLine::N() {
    return true;
}

QTableWidgetItem* TuringLine::GetCurrentCell() {

    return _current_cell;
}

bool TuringLine::AddCellFromLeft() {
    std::cout << "add cell from left" << std::endl;
    this->insertColumn(0);
    this->setItem(0, 0, new QTableWidgetItem(QString(LAMBDA)));
    this->setHorizontalHeaderItem(0, new QTableWidgetItem(QString::number(--_left_border)));

    return true;
}


bool TuringLine::RemoveCellFromLeft() {
    std::cout << "remove cell from left" << std::endl;
    if (columnCount() > 0) {
        this->removeColumn(0);
        _left_border++;
    }

    return true;
}


bool TuringLine::AddCellFromRight() {
    std::cout << "add cell from right" << std::endl;
    int columnCount = this->columnCount();
    this->insertColumn(columnCount);
    this->setItem(0, columnCount, new QTableWidgetItem(QString(LAMBDA)));
    this->setHorizontalHeaderItem(columnCount, new QTableWidgetItem(QString::number(_right_border++)));

    return true;
}


bool TuringLine::RemoveCellFromRight() {
    std::cout << "remove cell from right" << std::endl;
    if (columnCount() > 0) {
        this->removeColumn(columnCount() - 1);
        _right_border--;
    }

    return true;
}


bool TuringLine::SelectCell(int selected_cell) {
    _current_cell->background() = Qt::white;
    _current_cell_num = selected_cell;
    _current_cell = item(0, _current_cell_num);
    _current_cell->background() = Qt::yellow;

    return true;
}
