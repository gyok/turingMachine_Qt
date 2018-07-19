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
