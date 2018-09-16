#include "linepushbutton.h"

LinePushButton::LinePushButton(char* button_name, QWidget* parent, BubbleConnectionLine* current_line)
    : QPushButton(button_name, parent)
{
    // line which this button should delete
    _current_line = current_line;
}

BubbleConnectionLine* LinePushButton::GetConnectionLine() {
    return _current_line;
}

void LinePushButton::SetLineLayout(QHBoxLayout* line_layout) {
    _line_layout = line_layout;
}

QHBoxLayout* LinePushButton::GetLineLayout() {
    return _line_layout;
}

void LinePushButton::RemoveCurrentLine() {
    auto bubble_connection_manager = (BubbleConnectionManager*)this->parent();
    auto line_layout = GetLineLayout();
    line_layout->removeWidget(_current_line->GetSymbolBeforeLine());
    line_layout->removeWidget(_current_line->GetComboBox());
    line_layout->removeWidget(_current_line->GetSymbolAfterLine());
    line_layout->removeWidget(this);
    bubble_connection_manager->RemoveLine(_current_line, this);
    delete _current_line->GetSymbolBeforeLine();
    delete _current_line->GetComboBox();
    delete _current_line->GetSymbolAfterLine();
    delete _current_line;
    delete this;
}
