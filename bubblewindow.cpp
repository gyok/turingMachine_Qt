#include "bubblewindow.h"

#define BUBBLE_DEFAULT_SIZE 15
#define DOUBLE_PI 6.2831854

using namespace std;

BubbleWindow::BubbleWindow(QWidget* pwgt /*= 0*/) : QGLWidget(pwgt)
{
    _bubble_count = new int(0);
    _bubbleDrag = new bool(false);
    _bubbleConnect = new bool(false);
    _name_label_font = new QFont("cairo");


    setFocusPolicy(Qt::StrongFocus);

    SetBubbleSet(new set<Bubble*>());
    SetSelectedBubbleSet(new set<Bubble*>());
    glEnable(GL_MULTISAMPLE);

}

void BubbleWindow::initializeGL()
{
    qglClearColor(QColor(247, 241, 245));
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        DrawBubble(*it);
    }

    if (*_bubbleConnect) {
        std::cout << this->pos().x() << std::endl;
        DrawArrowToPoint(_connectingBubble, new QPoint(_currentPosition));
    }
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

    updateGL();
}

void BubbleWindow::mouseMoveEvent(QMouseEvent *event) {
    if (*_bubbleDrag) {
        _draggedBubble->SetPosition(new QPoint(InRange(0, event->x(), width()), InRange(0, event->y(), height())));
        updateGL();
    } else if (*_bubbleConnect) {
        _currentPosition = event->pos();
        updateGL();
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
                && event->key() != Qt::Key_Escape) {
            _rename_bubble_name += event->text();
            _rename_bubble->SetName(new QString(_rename_bubble_name));

            updateGL();
        } else if (event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Return) {
            SkipRenameBubble(true);
        } else {
            SkipRenameBubble(false);
        }
    }
}

void BubbleWindow::DrawBubble(Bubble *bubble){
    QPoint* bubble_center_position = bubble->GetPosition();
    float* bubble_size = bubble->GetBubbleSize();
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
        BubbleArrowConnect(bubble, *it);
    }

    glColor3f(red_color_level/255, green_color_level/255, blue_color_level/255);
    renderText(bubble_center_position->rx() + *bubble_size + 3, bubble_center_position->ry() + *bubble_size / 2, *bubble->GetName(), *_name_label_font);
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
          && (point->x() < _scrollArea->size().width())
          && (0 < point->y())
          && (point->y() < _scrollArea->size().height()))) {
        point->setX(BUBBLE_DEFAULT_SIZE);
        point->setY(BUBBLE_DEFAULT_SIZE);
    }

    GetBubbleSet()->insert(new Bubble(point, new QColor(112, 122, 116),new QString(QStringLiteral("A %1").arg((*_bubble_count)++)), new float(BUBBLE_DEFAULT_SIZE)));

    this->updateGL();
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

    updateGL();
}

void BubbleWindow::SelectBubble(Bubble* bubble) {
    _selected_bubble_set->insert(bubble);
    bubble->SetColor(new QColor(_selected_bubble_color));
}

void BubbleWindow::DeselectBubble(Bubble* bubble) {
    _selected_bubble_set->erase(bubble);
    bubble->SetColor(new QColor(_default_bubble_color));
    SkipRenameBubble(false);
}

bool BubbleWindow::StartConnectBubble(Bubble* bubble) {
    _connectingBubble = bubble;
    *_bubbleConnect = true;

    return true;
}

bool BubbleWindow::ConnectBubble(Bubble* bubble_iterator) {
    _connectingBubble->GetConnectionBubbleSet()->insert(bubble_iterator);
    *_bubbleConnect = false;
    _connectingBubble = 0;

    return true;
}

bool BubbleWindow::StopConnectBubble() {
    _connectingBubble = 0;
    *_bubbleConnect = false;

    return true;
}

bool BubbleWindow::StartDragBubble(Bubble* bubble) {
    _draggedBubble = bubble;
    *_bubbleDrag = true;

    return true;
}

bool BubbleWindow::StopDragBubble() {
    _draggedBubble = 0;
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

bool BubbleWindow::BubbleArrowConnect(Bubble* bubble_from, Bubble* bubble_to) {
    QPoint* bubble_from_point = bubble_from->GetPosition();
    QPoint* bubble_to_point = bubble_to->GetPosition();

    double point_disctance = PointDistance(*bubble_from_point, *bubble_to_point);
    if (point_disctance == 0) {
        QPoint* ungle_center = new QPoint(bubble_from_point->x() + 4 * *bubble_from->GetBubbleSize() * sinf(2.8085),
                                          bubble_from_point->y() + 4 * *bubble_from->GetBubbleSize() * cosf(2.8085));
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
        std::cout << "distance " << PointDistance(*arrow_start, *ungle_center) << std::endl;
        for (int i = smoothness / 2.2; i < smoothness + smoothness / 9; i++) {
//            if (i < smoothness / 5 || i > smoothness / 2) {
                glVertex2f((ungle_center->rx() + ungle_radius * cosf(DOUBLE_PI * i / smoothness)),
                           ungle_center->ry() + ungle_radius * sinf(DOUBLE_PI * i / smoothness));
//            }
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

        double alpha = section > 1 ? 3.1415927 - acos(distance_cos_ratio) : 3.1415927 + acos(distance_cos_ratio);
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

    return true;
}

void BubbleWindow::SetScrollArea(QScrollArea* scrollArea) {
    _scrollArea = scrollArea;
}

void BubbleWindow::RenameBubble(Bubble* bubble) {
    cout << "RenameBubble" << endl;
    _rename_bubble_mode = true;
    _rename_bubble = bubble;
    _before_rename_bubble_name = *bubble->GetName();
}

void BubbleWindow::SkipRenameBubble(bool saveBubbleName) {
    cout << "SkipRenameBubble" << endl;
    if (_rename_bubble_mode) {
        if (!saveBubbleName) {
            _rename_bubble->SetName(new QString(_before_rename_bubble_name));
        }

        _rename_bubble_mode = false;
        _rename_bubble_name = "";
        _before_rename_bubble_name = "";
        _rename_bubble = 0;
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

    std::cout << arrow_start->x() <<" " << arrow_start->y() << " " << arrow_end_r_spear->x() << " " << arrow_end_r_spear->y() << std::endl;
}
