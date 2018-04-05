#include "bubble.h"

Bubble::Bubble(QPoint* position, QColor* color, float* size) {
    SetPosition(position);
    SetColor(color);
    SetBubbleSize(size);
}

Bubble::~Bubble() {
    delete _position;
    delete _color;
    delete _name;
}

QPoint* Bubble::GetPosition() {
    return _position;
}

void Bubble::SetPosition(QPoint* position) {
    _position = position;
}

QColor* Bubble::GetColor() {
    return _color;
}

void Bubble::SetColor(QColor* color) {
    _color = color;
}

QString* Bubble::GetName() {
    return _name;
}

void Bubble::SetName(QString* name) {
    _name = name;
}

float* Bubble::GetBubbleSize() {
    return _size;
}

void Bubble::SetBubbleSize(float* size) {
    _size = size;
}
