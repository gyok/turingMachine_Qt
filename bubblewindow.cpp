 #include "bubblewindow.h"

#define BUBBLE_DEFAULT_SIZE 15
#define DOUBLE_PI 6.2831854
#define ANGLE_160 2.8085

using namespace std;

BubbleWindow::BubbleWindow(Core* core, TuringLine* turingLine, QWidget* pwgt /*= 0*/) : QOpenGLWidget(pwgt)
{
    _bubble_count = new int(0);
    _bubbleDrag = new bool(false);
    _bubbleConnect = new bool(false);
    _name_label_font = new QFont("cairo");
    _turingLine = turingLine;
    _max_id = new int(0);
    _core = core;

    setFocusPolicy(Qt::StrongFocus);

    SetBubbleSet(new set<Bubble*>());
    SetSelectedBubbleSet(new set<Bubble*>());
    glEnable(GL_MULTISAMPLE);

}

void BubbleWindow::initializeGL()
{
}

void BubbleWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(0, w, h, 0, -1, 1);
}

void BubbleWindow::paintGL()
{
    QPainter* painter = new QPainter(this);
    painter->beginNativePainting();
    glClearColor(0.97, 0.98, 0.98, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        DrawBubble(*it, painter);
    }

    if (*_bubbleConnect) {
//        std::cout << this->pos().x() << std::endl;
        DrawArrowToPoint(_connecting_bubble, new QPoint(_current_position));
    }
    painter->endNativePainting();
    painter->~QPainter();
}

void BubbleWindow::mousePressEvent(QMouseEvent *event) {
    bool* bubble_finded = new bool(false);
    Bubble* finded_bubble_iterator = FindBubbleAtPoint(event->pos(), bubble_finded);

    if (*bubble_finded) {
        if (event->button() == Qt::RightButton ||
                (event->button() == Qt::LeftButton && QGuiApplication::keyboardModifiers() == Qt::CTRL)) {
            StopDragBubble();
            StartConnectBubble(finded_bubble_iterator);
        } else if (event->button() == Qt::LeftButton){
            StopConnectBubble();
            StartDragBubble(finded_bubble_iterator);
        }

        if (QGuiApplication::keyboardModifiers() != Qt::SHIFT) {
            for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
                DeselectBubble(*it);
            }
        }
        SelectBubble(finded_bubble_iterator);
    }

    if (!*_bubbleDrag) {
        for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
            DeselectBubble(*it);
        }

        _selected_bubble_set->clear();
    }
}

void BubbleWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (*_bubbleConnect && (event->button() == Qt::RightButton ||
                            (event->button() == Qt::LeftButton && QGuiApplication::keyboardModifiers() == Qt::CTRL))) {
        bool* bubble_finded = new bool(false);
        Bubble* finded_bubble_iterator = FindBubbleAtPoint(event->pos(), bubble_finded);
        if (*bubble_finded) {
            ConnectBubble(finded_bubble_iterator);
        }

        StopConnectBubble();
    } else if (*_bubbleDrag && event->button() == Qt::LeftButton) {
        StopDragBubble();
    }

    update();
}

void BubbleWindow::mouseMoveEvent(QMouseEvent *event) {
    if (*_bubbleDrag) {
        _dragged_bubble->SetPosition(new QPoint(InRange(0, event->x(), width()), InRange(0, event->y(), height())));
        update();
    } else if (*_bubbleConnect) {
        _current_position = event->pos();
        update();
    }
}

void BubbleWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    bool* bubble_finded = new bool(false);
    Bubble* finded_bubble_iterator = FindBubbleAtPoint(event->pos(), bubble_finded);

    if (*bubble_finded) {
        RenameBubble(finded_bubble_iterator);
    }
}

void BubbleWindow::keyPressEvent(QKeyEvent *event) {
    if (_rename_bubble_mode) {
        if (event->key() != Qt::Key_Enter
                && event->key() != Qt::Key_Return
                && event->key() != Qt::Key_Escape
                && event->key() != Qt::Key_Backspace) {
            std::cout << "key" << event->text().toStdString() << std::endl;
            _rename_bubble_name += event->text();
            _rename_bubble->SetName(new QString(_rename_bubble_name));

            update();
        } else if (event->key() == Qt::Key_Backspace) {
            _rename_bubble_name.truncate(_rename_bubble_name.length() - 1);
            _rename_bubble->SetName(new QString(_rename_bubble_name));

            update();
        } else if (event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Return) {
            SkipRenameBubble(true);
        } else {
            SkipRenameBubble(false);
        }
    }
}

void BubbleWindow::DrawBubble(Bubble *bubble, QPainter* painter){
    QPoint* bubble_center_position = bubble->GetPosition();
    double* bubble_size = bubble->GetBubbleSize();
    GLfloat red_color_level = (float)bubble->GetColor()->red();
    GLfloat green_color_level = (float)bubble->GetColor()->green();
    GLfloat blue_color_level = (float)bubble->GetColor()->blue();

    const int smoothness = 272;

    glBegin(GL_POLYGON);
    glColor3f(red_color_level/255, green_color_level/255, blue_color_level/255);
    for (int i = 0; i < smoothness; i++) {
        glVertex2f((bubble_center_position->rx() + *bubble_size * cosf(DOUBLE_PI * i / smoothness)),
                   bubble_center_position->ry() + *bubble_size * sinf(DOUBLE_PI * i / smoothness));
    }
    glEnd();

    for (set<Bubble*>::iterator it = bubble->GetConnectionBubbleSet()->begin(); it != bubble->GetConnectionBubbleSet()->end(); it++) {
        BubbleArrowConnect(bubble, *it, painter);
    }

    glColor3f(red_color_level/255, green_color_level/255, blue_color_level/255);
    painter->endNativePainting();
    painter->drawText(bubble_center_position->rx() + *bubble_size + 3, bubble_center_position->ry() + *bubble_size / 2, *bubble->GetName());
    painter->beginNativePainting();
}

double BubbleWindow::PointDistance(QPoint p1, QPoint p2) {
    return sqrt(pow(p1.rx() - p2.rx(), 2) + pow(p1.ry() - p2.ry(), 2));
}

int BubbleWindow::InRange(int min, int current, int max) {
    int result = current > min ? current : min;
    return result < max ? result : max;
}

set<Bubble*>* BubbleWindow::GetBubbleSet() {
    return _bubble_set;
}

void BubbleWindow::SetBubbleSet(set<Bubble*>* bubble_set) {
    _bubble_set = bubble_set;
}

set<Bubble*>* BubbleWindow::GetSelectedBubbleSet() {
    return _selected_bubble_set;
}

void BubbleWindow::SetSelectedBubbleSet(std::set<Bubble*>* selected_bubble_set) {
    _selected_bubble_set = selected_bubble_set;
}

void BubbleWindow::AddBubble() {
    QPoint* point = new QPoint(mapFromGlobal(QCursor::pos()));
    if (!((0 < point->x())
          && (point->x() < _scrollArea->widget()->size().width())
          && (0 < point->y())
          && (point->y() < _scrollArea->widget()->size().height()))) {
        point->setX(BUBBLE_DEFAULT_SIZE);
        point->setY(BUBBLE_DEFAULT_SIZE);
    }

    GetBubbleSet()->insert(new Bubble(new int(GetNewId()), point, new QColor(112, 122, 116),new QString(QStringLiteral("A %1").arg((*_bubble_count)++)), new double(BUBBLE_DEFAULT_SIZE)));

    this->update();
}

void BubbleWindow::DeleteSelectedBubbles() {
    // go throught all babbles and dellete they connections with selected bubbles
    for (set<Bubble*>::iterator bubble_it = GetBubbleSet()->begin(); bubble_it != GetBubbleSet()->end(); bubble_it++) {
        for (set<Bubble*>::iterator it = GetSelectedBubbleSet()->begin(); it != GetSelectedBubbleSet()->end(); it++) {
            if ((*bubble_it)->GetConnectionBubbleSet()->find(*it) != (*bubble_it)->GetConnectionBubbleSet()->end()) {
                (*bubble_it)->GetConnectionBubbleSet()->erase(*it);
            }
        }
    }

    for (set<Bubble*>::iterator it = GetSelectedBubbleSet()->begin(); it != GetSelectedBubbleSet()->end(); ++it) {
        (*it)->GetConnectionBubbleSet()->clear();
        GetBubbleSet()->erase(*it);
        GetSelectedBubbleSet()->erase(it);
        delete(*it);
    }

    update();
}

void BubbleWindow::MakeStartSelectedBubble() {
    if (GetSelectedBubbleSet()->empty()) {
        return;
    }
    MakeStartBubble(*GetSelectedBubbleSet()->begin());
}

void BubbleWindow::MakeFinishSelectedBubble() {
    if (GetSelectedBubbleSet()->empty()) {
        return;
    }
    MakeFinishBubble(*GetSelectedBubbleSet()->begin());
}


void BubbleWindow::MakeStartBubble(Bubble* start_bubble_candidate) {
    QColor* default_color = new QColor(112, 122, 116);
    if (_start_bubble != 0) {
        if (_start_bubble->GetBubbleId() == start_bubble_candidate->GetBubbleId()) {
            return;
        }
        _start_bubble->SetDefaultColor(default_color);
        _start_bubble->SetColor(default_color);
    }
    if (_finish_bubble != 0
            && _finish_bubble->GetBubbleId() == start_bubble_candidate->GetBubbleId()) {
        _finish_bubble->SetDefaultColor(default_color);
        _finish_bubble->SetColor(default_color);
        _finish_bubble = 0;
    }
    _start_bubble = start_bubble_candidate;
    _start_bubble->SetDefaultColor(new QColor(31, 177, 224));
    _start_bubble->SetColor(new QColor(31, 177, 224));
    _core->SetCurrentBubble(_start_bubble);
    _core->SetCurrentCellCode(1);
    update();
}


void BubbleWindow::MakeFinishBubble(Bubble* finish_bubble_candidate) {
    QColor* default_color = new QColor(112, 122, 116);
    if (_finish_bubble != 0) {
        if (_finish_bubble->GetBubbleId() == finish_bubble_candidate->GetBubbleId()) {
            return;
        }
        _finish_bubble->SetDefaultColor(default_color);
        _finish_bubble->SetColor(default_color);
    }
    if (_start_bubble != 0
            && _start_bubble->GetBubbleId() == finish_bubble_candidate->GetBubbleId()) {
        _start_bubble->SetDefaultColor(default_color);
        _start_bubble->SetColor(default_color);
        _start_bubble = 0;
    }
    _finish_bubble = finish_bubble_candidate;
    _finish_bubble->SetDefaultColor(new QColor(88, 204, 26));
    _finish_bubble->SetColor(new QColor(88, 204, 26));
    _core->SetFinishBubbleId(_finish_bubble->GetBubbleId());
    update();
}


int BubbleWindow::GetStartBubbleId() {
    return _start_bubble->GetBubbleId();
}


int BubbleWindow::GetFinishBubbleId() {
    return _finish_bubble->GetBubbleId();
}


void BubbleWindow::SelectBubble(Bubble* bubble) {
    _selected_bubble_set->insert(bubble);
    bubble->SetColor(bubble->GetSelectColor());
}

void BubbleWindow::DeselectBubble(Bubble* bubble) {
    _selected_bubble_set->erase(bubble);
    bubble->SetColor(bubble->GetDefaultColor());
    SkipRenameBubble(false);
}

bool BubbleWindow::StartConnectBubble(Bubble* bubble) {
    _connecting_bubble = bubble;
    *_bubbleConnect = true;

    return true;
}

bool BubbleWindow::ConnectBubble(Bubble* bubble_iterator) {
    set<BubbleConnectionLine*>* connectionInfo = (*_connecting_bubble->GetConnectionInfo())[bubble_iterator->GetBubbleId()];
    BubbleConnectionManager* connectionManager = new BubbleConnectionManager(_turingLine, connectionInfo);
    if (connectionManager->execWindow() == QDialog::Accepted) {
        _connecting_bubble->GetConnectionBubbleSet()->insert(bubble_iterator);
        set<BubbleConnectionLine*>* connectionSet = connectionManager->GetConnectionLineSet();
        set<BubbleConnectionLine*>::iterator itToDelete;
        bool needDelete = false;
        for (set<BubbleConnectionLine*>::iterator it = connectionSet->begin();
             it != connectionSet->end();
             it++) {
            if (needDelete) {
                needDelete = false;
                connectionSet->erase(itToDelete);
            }
            if ((*it)->GetSymbolBeforeLine()->text() == NULL
                || (*it)->GetSymbolAfterLine()->text() == NULL) {
                itToDelete = it;
                needDelete = true;
            }
//            cout << (*it)->GetSymbolBeforeLine()->text().toStdString() << endl;
//            cout << (*it)->GetComboBox()->currentText().toStdString() << endl;
//            cout << (*it)->GetSymbolAfterLine()->text().toStdString() << endl;
        }
        if (needDelete) {
            needDelete = false;
            connectionSet->erase(itToDelete);
        }
//        cout << "dialog closed" << endl;
        (*_connecting_bubble->GetConnectionInfo())[bubble_iterator->GetBubbleId()] = connectionManager->GetConnectionLineSet();
    }

    *_bubbleConnect = false;
    _connecting_bubble = 0;

    return true;
}

bool BubbleWindow::StopConnectBubble() {
    _connecting_bubble = 0;
    *_bubbleConnect = false;

    return true;
}

bool BubbleWindow::StartDragBubble(Bubble* bubble) {
    _dragged_bubble = bubble;
    *_bubbleDrag = true;

    return true;
}

bool BubbleWindow::StopDragBubble() {
    _dragged_bubble = 0;
    *_bubbleDrag = false;

    return true;
}

Bubble* BubbleWindow::FindBubbleAtPoint(QPoint point, bool* bubble_finded) {
    for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        if (PointDistance(*(*it)->GetPosition(), point) <= *(*it)->GetBubbleSize()) {
            *bubble_finded = true;
            return *it;
        }
    }

    *bubble_finded = false;
    return *(GetBubbleSet()->begin());
}

bool BubbleWindow::BubbleArrowConnect(Bubble* bubble_from, Bubble* bubble_to, QPainter* painter) {
    QPoint* bubble_from_point = bubble_from->GetPosition();
    QPoint* bubble_to_point = bubble_to->GetPosition();

    double point_disctance = PointDistance(*bubble_from_point, *bubble_to_point);
    // draw cicle arrow or connect two bubbles
    if (point_disctance == 0) {
        QPoint* ungle_center = new QPoint(bubble_from_point->x() + 4 * *bubble_from->GetBubbleSize() * sinf(ANGLE_160),
                                          bubble_from_point->y() + 4 * *bubble_from->GetBubbleSize() * cosf(ANGLE_160));
        QPoint* arrow_start = new QPoint(bubble_from_point->x() + *bubble_from->GetBubbleSize() * sinf(2.617),
                                         bubble_from_point->y() + *bubble_from->GetBubbleSize() * cosf(2.617));
        QPoint* arrow_ungle_start = new QPoint(bubble_from_point->x() + 4 * *bubble_from->GetBubbleSize() * sinf(2.617),
                                               bubble_from_point->y() + 4 * *bubble_from->GetBubbleSize() * cosf(2.617));
        QPoint* arrow_ungle_end = new QPoint(bubble_from_point->x() + 4 * *bubble_from->GetBubbleSize() * sinf(3.0),
                                             bubble_from_point->y() + 4 * *bubble_from->GetBubbleSize() * cosf(3.0));
        QPoint* arrow_end = new QPoint(bubble_from_point->x() + *bubble_from->GetBubbleSize() * sinf(3.0),
                                       bubble_from_point->y() + *bubble_from->GetBubbleSize() * cosf(3.0));

        double ungle_radius = PointDistance(*arrow_ungle_start, *arrow_ungle_end) / 2;
        int smoothness = 272;
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glColor3f(0.49f, 0.52f, 0.88f);
        glVertex2f(arrow_start->x(), arrow_start->y());
        glVertex2f(arrow_ungle_start->x(), arrow_ungle_start->y());
        glVertex2f(arrow_ungle_end->x(), arrow_ungle_end->y());
        glVertex2f(arrow_end->x(), arrow_end->y());
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (int i = smoothness / 2.2; i < smoothness + smoothness / 9; i++) {
            glVertex2f((ungle_center->rx() + ungle_radius * cosf(DOUBLE_PI * i / smoothness)),
                       ungle_center->ry() + ungle_radius * sinf(DOUBLE_PI * i / smoothness));
        }
        glEnd();

    } else {
        double distance_cos_ratio = (double)(bubble_from_point->x() - bubble_to_point->x()) / point_disctance;
        double distance_sin_ratio = (double)(bubble_from_point->y() - bubble_to_point->y()) / point_disctance;

        int section = 0;
        bool a1 =  (double)(bubble_from_point->x() - bubble_to_point->x()) > 0;
        bool b1 = (double)(bubble_from_point->y() - bubble_to_point->y()) > 0;
        if (a1 && !b1) {
            section = 2;
        } else if (!a1 && !b1) {
            section = 3;
        } else if (!a1 && b1) {
            section = 0;
        } else if (a1 && b1) {
            section = 1;
        }

        const double _pi = 3.1415927;
        double alpha = section > 1 ? _pi - acos(distance_cos_ratio) : _pi + acos(distance_cos_ratio);
        double betta = 0.38;
        double a = 22;

        QPoint* arrow_start = new QPoint(bubble_from_point->x() - *bubble_from->GetBubbleSize() * distance_cos_ratio,
                                         bubble_from_point->y() - *bubble_from->GetBubbleSize() * distance_sin_ratio);
        QPoint* arrow_end = new QPoint(bubble_to_point->x() + *bubble_to->GetBubbleSize() * distance_cos_ratio,
                                       bubble_to_point->y() + *bubble_to->GetBubbleSize() * distance_sin_ratio);
        QPoint* arrow_end_l_spear = new QPoint(bubble_to_point->x() + *bubble_to->GetBubbleSize() * distance_cos_ratio - a * cos(alpha - betta),
                                               bubble_to_point->y() + *bubble_to->GetBubbleSize() * distance_sin_ratio - a * sin(alpha - betta));
        QPoint* arrow_end_r_spear = new QPoint(bubble_to_point->x() + *bubble_to->GetBubbleSize() * distance_cos_ratio - a * cos(alpha + betta),
                                               bubble_to_point->y() + *bubble_to->GetBubbleSize() * distance_sin_ratio - a * sin(alpha + betta));

        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glColor3f(0.49f, 0.52f, 0.88f);
        glVertex2f(arrow_start->x(), arrow_start->y());
        glVertex2f(arrow_end->x(), arrow_end->y());
        glVertex2f(arrow_end->x(), arrow_end->y());
        glVertex2f(arrow_end_l_spear->x(), arrow_end_l_spear->y());
        glVertex2f(arrow_end->x(), arrow_end->y());
        glVertex2f(arrow_end_r_spear->x(), arrow_end_r_spear->y());
        glEnd();
    }
    QString font_name = "cairo";
    // TODO Duplication of arrow shouldn`t remove previouse connect description
    // TODO Add ability to delete connection betwen bubbles
    DrawArrowTextDescription(bubble_from, bubble_to, font_name, painter);

    return true;
}

void BubbleWindow::SetScrollArea(QScrollArea* scrollArea) {
    _scrollArea = scrollArea;
}

void BubbleWindow::RenameBubble(Bubble* bubble) {
//    cout << "RenameBubble" << endl;
    _rename_bubble_mode = true;
    _rename_bubble = bubble;
    _before_rename_bubble_name = *bubble->GetName();
    _rename_bubble->SetName(new QString("type in new name, Enter - save, Esc - dismiss"));
}

void BubbleWindow::SkipRenameBubble(bool saveBubbleName) {
//    cout << "SkipRenameBubble" << endl;
    if (_rename_bubble_mode) {
        if (!saveBubbleName) {
            _rename_bubble->SetName(new QString(_before_rename_bubble_name));
        }

        _rename_bubble_mode = false;
        _rename_bubble_name = "";
        _before_rename_bubble_name = "";
        _rename_bubble = 0;
        update();
    }
}

void BubbleWindow::DrawArrowToPoint(Bubble* bubble_from, QPoint* bubble_to_point) {
    QPoint* bubble_from_point = bubble_from->GetPosition();
    double point_disctance = PointDistance(*bubble_from_point, *bubble_to_point);
    double distance_cos_ratio = (double)(bubble_from_point->x() - bubble_to_point->x()) / point_disctance;
    double distance_sin_ratio = (double)(bubble_from_point->y() - bubble_to_point->y()) / point_disctance;

    int section = 0;
    bool a1 =  (double)(bubble_from_point->x() - bubble_to_point->x()) > 0;
    bool b1 = (double)(bubble_from_point->y() - bubble_to_point->y()) > 0;
    if (a1 && !b1) {
        section = 2;
    } else if (!a1 && !b1) {
        section = 3;
    } else if (!a1 && b1) {
        section = 0;
    } else if (a1 && b1) {
        section = 1;
    }

    double alpha = section > 1 ? 3.1415927 - acos(distance_cos_ratio) : 3.1415927 + acos(distance_cos_ratio);
    double betta = 0.38;
    // angle of arrow parts
    double a = 22;

    QPoint* arrow_start = new QPoint(bubble_from_point->x() - *bubble_from->GetBubbleSize() * distance_cos_ratio,
                                     bubble_from_point->y() - *bubble_from->GetBubbleSize() * distance_sin_ratio);
    QPoint* arrow_end = new QPoint(bubble_to_point->x(),
                                   bubble_to_point->y());
    QPoint* arrow_end_l_spear = new QPoint(bubble_to_point->x() - a * cos(alpha - betta),
                                           bubble_to_point->y() - a * sin(alpha - betta));
    QPoint* arrow_end_r_spear = new QPoint(bubble_to_point->x() - a * cos(alpha + betta),
                                           bubble_to_point->y() - a * sin(alpha + betta));

    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(0.49f, 0.52f, 0.88f);
    glVertex2f(arrow_start->x(), arrow_start->y());
    glVertex2f(arrow_end->x(), arrow_end->y());
    glVertex2f(arrow_end->x(), arrow_end->y());
    glVertex2f(arrow_end_l_spear->x(), arrow_end_l_spear->y());
    glVertex2f(arrow_end->x(), arrow_end->y());
    glVertex2f(arrow_end_r_spear->x(), arrow_end_r_spear->y());
    glEnd();

//    std::cout << arrow_start->x() <<" " << arrow_start->y() << " " << arrow_end_r_spear->x() << " " << arrow_end_r_spear->y() << std::endl;
}

void BubbleWindow::DrawArrowTextDescription(Bubble* bubble_from, Bubble* bubble_to,
                                            const QString font_name, QPainter* painter) {
    set<BubbleConnectionLine*>* connection_operations = (*bubble_from->GetConnectionInfo())[bubble_to->GetBubbleId()];
    QString* connect_description = new QString("");
    QString* description_part_connector = new QString(":");
    QString* descriptions_connector = new QString(", ");
    for (set<BubbleConnectionLine*>::iterator it = connection_operations->begin(); it != connection_operations->end(); it++) {
        *connect_description += (*it)->GetSymbolBeforeLine()->text()
                + *description_part_connector
                + (*it)->GetSelectedWay()
                + *description_part_connector
                + (*it)->GetSymbolAfterLine()->text();
        if (it != prev(connection_operations->end())) {
            *connect_description += *descriptions_connector;
        }
    }
    if (connect_description->isEmpty()) {
        return;
    }

//    cout << "start draw connection" << endl;

    painter->endNativePainting();
    painter->setFont(QFont(font_name));

    QPoint* centerBetweenBubbles = new QPoint((bubble_from->GetPosition()->x() +
                                               + bubble_to->GetPosition()->x()) / 2,
                                            (bubble_from->GetPosition()->y()
                                             + bubble_to->GetPosition()->y()) / 2);
    QPoint* p = new QPoint((bubble_from->GetPosition()->x() - bubble_to->GetPosition()->x()),
                           (bubble_from->GetPosition()->y() - bubble_to->GetPosition()->y()));
//    cout << p->x() << "x" << p->y() << endl;
    double r_angle = atan(double(p->x()) / double(p->y())) / 3.14;
    double angle = r_angle * 180 + 90 + double(p->y() >= 0 ? 180 : 0);
    angle = angle > 90 && angle < 270
            ? angle + 180
            : angle;

    // center of round arrow
    QPoint* round_circle_center = new QPoint(bubble_from->GetPosition()->x() + 4 * *bubble_from->GetBubbleSize() * sinf(ANGLE_160) + 1 * *bubble_from->GetBubbleSize(),
                                      bubble_from->GetPosition()->y() + 4 * *bubble_from->GetBubbleSize() * cosf(ANGLE_160) - 2.5 * *bubble_from->GetBubbleSize());
    // rotate text if connect two different bubbles
    if (bubble_from != bubble_to) {
        painter->translate(centerBetweenBubbles->x(), centerBetweenBubbles->y());
        painter->rotate(-angle);
    } else {
        painter->translate(round_circle_center->x(), round_circle_center->y());
    }
    QFont cairo_f = QFont(font_name);
    QFontMetrics fm(cairo_f);
    int fm_one_w = fm.width("A");
    int fm_w = fm.width(*connect_description);
    int fm_h = fm.height();
    painter->setFont(cairo_f);
    QPen pen = painter->pen();
    QPen grayPen(QColor(117, 114, 115, 244));
    QPen greenPen(QColor(46, 184, 101, 244));
    painter->setPen(greenPen);
    if (bubble_from->GetPosition()->x() < bubble_to->GetPosition()->x()) {
        painter->drawText(-fm_one_w * 1.5 + -fm_w / 2, -fm_h/2, "→");
        painter->setPen(grayPen);
        painter->drawText(-fm_w / 2, -fm_h/2, *connect_description);
    } else {
        painter->drawText(-fm_one_w * 1.5 + -fm_w / 2, fm_h, "←");
        painter->setPen(grayPen);
        painter->drawText(-fm_w/2, fm_h, *connect_description);
    }
    painter->setPen(pen);
    // rotate text if connect two different bubbles
    if (bubble_from != bubble_to) {
        painter->rotate(angle);
        painter->translate(-centerBetweenBubbles->x(), -centerBetweenBubbles->y());
    } else {
        painter->translate(-round_circle_center->x(), -round_circle_center->y());
    }

    painter->beginNativePainting();
//    cout << "end draw connection" << endl;
}

int BubbleWindow::GetNewId() {
    return ++(*_max_id);
}
