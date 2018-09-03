#include "bubbleconnectionline.h"

BubbleConnectionLine::BubbleConnectionLine(QWidget* parent, TuringLine* turingLine)
    : QObject(parent)
{
    _symbolBeforeLine = new QLineEdit(parent);
    _symbolAfterLine = new QLineEdit(parent);
    _wayToMoveCombobox = new QComboBox(parent);

    _wayToMoveCombobox->addItem("L", "L");
    _wayToMoveCombobox->addItem("R", "R");
    _wayToMoveCombobox->addItem("N", "N");
    _wayToMoveCombobox->setCurrentIndex(1);
}


QLineEdit* BubbleConnectionLine::GetSymbolBeforeLine() {
    return _symbolBeforeLine;
}


QLineEdit* BubbleConnectionLine::GetSymbolAfterLine() {
    return _symbolAfterLine;
}


QComboBox* BubbleConnectionLine::GetComboBox() {
    return _wayToMoveCombobox;
}


QString BubbleConnectionLine::GetSelectedWay() {
    return _wayToMoveCombobox->currentText();
}
