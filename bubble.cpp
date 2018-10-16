#include "bubble.h"

using namespace std;

Bubble::Bubble(int* id, QPoint* position, QColor* color, QString* name, double* size) {
    SetPosition(position);
    SetColor(color);
    SetDefaultColor(new QColor(112, 122, 116));
    SetSelectColor(new QColor(144, 14, 249));
    SetName(name);
    SetBubbleSize(size);
    SetConnectionBubbleSet(new set<Bubble*>());
    _id = id;

    _conn_info = new map<int, set<BubbleConnectionLine*>*>();
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

QColor* Bubble::GetDefaultColor() {
    return _default_color;
}

void Bubble::SetDefaultColor(QColor* default_color) {
    _default_color = default_color;
}

QColor* Bubble::GetSelectColor() {
    return _select_color;
}

void Bubble::SetSelectColor(QColor* select_color) {
    _select_color = select_color;
}

QString* Bubble::GetName() {
    return _name;
}

void Bubble::SetName(QString* name) {
    _name = name;
}

double* Bubble::GetBubbleSize() {
    return _size;
}

void Bubble::SetBubbleSize(double* size) {
    _size = size;
}

set<Bubble*>* Bubble::GetConnectionBubbleSet() {
    return _connection_set;
}

bool Bubble::SetConnectionBubbleSet(set<Bubble*>* connection_set) {
    _connection_set = connection_set;

    return true;
}

map<int, set<BubbleConnectionLine*>*>* Bubble::GetConnectionInfo() {
    return _conn_info;
}

int Bubble::GetBubbleId() {
    return *_id;
}
