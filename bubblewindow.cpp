#include "bubblewindow.h"

BubbleWindow::BubbleWindow(QWidget* pwgt /*= 0*/) : QGLWidget(pwgt)
{
}

void BubbleWindow::initializeGL()
{
    qglClearColor(QColor(247, 241, 245));
}

void BubbleWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)w, (GLint)h);
    glOrtho(0, 100, 100, 0, -1, 1);
}

void BubbleWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (std::set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        DrawBubble(*it);
    }
}

void BubbleWindow::mousePressEvent(QMouseEvent *event) {
    QPoint pe = event->pos();
    std::cout << pe.x() << " " << pe.y() << std::endl;
    QPoint *bubble_position = new QPoint();
    double distance;

    for (std::set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        bubble_position = (*it)->GetPosition();
        pe.setX(pe.x() * geometry().height() / geometry().width());
        pe.setY(pe.y() * 100 / geometry().height());
        distance = PointDistance(*bubble_position, pe);
        if (distance <= *(*it)->GetBubbleSize()) {
            _draggedBubble = *it;
            _bubbleDrag = new bool(true);
            std::cout << "you are at bubble" << std::endl;
            break;
        }
    }
}

void BubbleWindow::mouseReleaseEvent(QMouseEvent *event) {

}

void BubbleWindow::mouseMoveEvent(QMouseEvent *event) {
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
            glVertex2f((bubble_center_position->rx() + *bubble_size * cosf(doublePi * i / smoothness)) * geometry().height() / geometry().width(),
                       bubble_center_position->ry() + *bubble_size * sinf(doublePi * i / smoothness));
        }
   glEnd();
}

double BubbleWindow::PointDistance(QPoint p1, QPoint p2) {
    return sqrt(pow(p1.rx() - p2.rx(), 2) + pow(p1.ry() - p2.ry(), 2));
}

std::set<Bubble*>* BubbleWindow::GetBubbleSet() {
    return _bubble_set;
}

void BubbleWindow::SetBubbleSet(std::set<Bubble*>* bubble_set) {
    _bubble_set = bubble_set;
}

void BubbleWindow::AddBubble() {
    std::cout << "add bubble" << std::endl;
    GetBubbleSet()->insert(new Bubble(new QPoint(15, 15), new QColor(112, 122, 116), new float(15)));

    for (std::set<Bubble*>::iterator it = GetBubbleSet()->begin(); it != GetBubbleSet()->end(); ++it) {
        std::cout << "o" << std::endl;
    }

    this->updateGL();
}
