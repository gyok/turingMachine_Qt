#include "bubblewindow.h"

using namespace std;

BubbleWindow::BubbleWindow(QWidget* pwgt /*= 0*/) : QGLWidget(pwgt)
{
    _bubbleDrag = new bool(false);
    SetBubbleSet(new set<Bubble*>());
    SetSelectedBubbleSet(new set<Bubble*>());

}

void BubbleWindow::initializeGL()
{
    qglClearColor(QColor(247, 241, 245));
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    QPoint pe = event->pos();
    cout << pe.x() << " " << pe.y() << std::endl;
    QPoint *bubble_position = new QPoint();
    double distance;

    for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        bubble_position = (*it)->GetPosition();
        distance = PointDistance(*bubble_position, pe);
        if (distance <= *(*it)->GetBubbleSize()) {
            _draggedBubble = *it;
            *_bubbleDrag = true;
            SelectBubble(*it);
            break;
        }
    }

    if (!*_bubbleDrag) {
        for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
            DeselectBubble(*it);
        }

        _selected_bubble_set->clear();
    }
}

void BubbleWindow::mouseReleaseEvent(QMouseEvent *event) {
    _draggedBubble = 0;
    *_bubbleDrag = false;
    updateGL();
}

void BubbleWindow::mouseMoveEvent(QMouseEvent *event) {
    if (*_bubbleDrag) {
        _draggedBubble->SetPosition(new QPoint(event->x(), event->y()));
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

    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_POLYGON);
        for (int i = 0; i < smoothness; i++) {
            glColor3f(red_color_level/255, green_color_level/255, blue_color_level/255);
            glVertex2f((bubble_center_position->rx() + *bubble_size * cosf(doublePi * i / smoothness)),
                       bubble_center_position->ry() + *bubble_size * sinf(doublePi * i / smoothness));
        }
   glEnd();
}

double BubbleWindow::PointDistance(QPoint p1, QPoint p2) {
    return sqrt(pow(p1.rx() - p2.rx(), 2) + pow(p1.ry() - p2.ry(), 2));
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
    cout << "add bubble" << std::endl;
    GetBubbleSet()->insert(new Bubble(new QPoint(15, 15), new QColor(112, 122, 116), new float(15)));

    for (set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        cout << "o" << std::endl;
    }

    this->updateGL();
}

void BubbleWindow::SelectBubble(Bubble* bubble) {
    cout << "select bubble" << endl;
    _selected_bubble_set->insert(bubble);
    bubble->SetColor(_selected_bubble_color);
    cout << "bubble selected" << endl;
}

void BubbleWindow::DeselectBubble(Bubble* bubble) {
    bubble->SetColor(_default_bubble_color);
}
