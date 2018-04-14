#include "bubblewindow.h"

using namespace std;

BubbleWindow::BubbleWindow(QWidget* pwgt /*= 0*/) : QGLWidget(pwgt)
{
    _bubble_count = new int(0);
    _bubbleDrag = new bool(false);
    _bubbleConnect = new bool(false);
    _name_label_font = new QFont("cairo");

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
                    cout << "no SHIFT" << endl;
                }
            }
            SelectBubble(finded_bubble_iterator);
    }

    if (!*_bubbleDrag) {
        for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
            DeselectBubble(*it);
        }

        cout << "deselect" << endl;
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
    }
}

void BubbleWindow::DrawBubble(Bubble *bubble){
    QPoint* bubble_center_position = bubble->GetPosition();
    float* bubble_size = bubble->GetBubbleSize();
    GLfloat red_color_level = (float)bubble->GetColor()->red();
    GLfloat green_color_level = (float)bubble->GetColor()->green();
    GLfloat blue_color_level = (float)bubble->GetColor()->blue();

    const int smoothness = 272;
    const float doublePi = 2.0f * 3.1415927;

    glBegin(GL_POLYGON);
        for (int i = 0; i < smoothness; i++) {
            glColor3f(red_color_level/255, green_color_level/255, blue_color_level/255);
            glVertex2f((bubble_center_position->rx() + *bubble_size * cosf(doublePi * i / smoothness)),
                       bubble_center_position->ry() + *bubble_size * sinf(doublePi * i / smoothness));
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
    GetBubbleSet()->insert(new Bubble(new QPoint(15, 15), new QColor(112, 122, 116),new QString(QStringLiteral("A %1").arg((*_bubble_count)++)), new float(15)));

    this->updateGL();
}

void BubbleWindow::DeleteSelectedBubbles() {
    // go throught all babbles and dellete they connections with selected bubbles
    for (set<Bubble*>::iterator bubble_it = GetBubbleSet()->begin(); bubble_it != GetBubbleSet()->end(); bubble_it++) {
        cout << (*bubble_it)->GetName()->toStdString() << endl;
        for (set<Bubble*>::iterator it = GetSelectedBubbleSet()->begin(); it != GetSelectedBubbleSet()->end(); it++) {
            if ((*bubble_it)->GetConnectionBubbleSet()->find(*it) != (*bubble_it)->GetConnectionBubbleSet()->end()) {
                cout << "\tfinded " << (*it)->GetName()->toStdString() << endl;
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

    cout << 172 << endl;
}

void BubbleWindow::SelectBubble(Bubble* bubble) {
    _selected_bubble_set->insert(bubble);
    bubble->SetColor(new QColor(_selected_bubble_color));
}

void BubbleWindow::DeselectBubble(Bubble* bubble) {
    _selected_bubble_set->erase(bubble);
    bubble->SetColor(new QColor(_default_bubble_color));
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


    return true;
}
