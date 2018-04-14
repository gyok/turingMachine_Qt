#include "bubble.h"

using namespace std;

Bubble::Bubble(QPoint* position, QColor* color, QString* name, float* size) {
    SetPosition(position);
    SetColor(color);
    SetName(name);
    SetBubbleSize(size);
    SetConnectionBubbleSet(new set<Bubble*>());
}

Bubble::~Bubble() {
    cout << "destructor " << _name->toStdString() << endl;
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

set<Bubble*>* Bubble::GetConnectionBubbleSet() {
    return _connection_set;
}

bool Bubble::SetConnectionBubbleSet(set<Bubble*>* connection_set) {
    _connection_set = connection_set;

    return true;
}
