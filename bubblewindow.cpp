#include "bubblewindow.h"

BubbleWindow::BubbleWindow(QWidget* pwgt /*= 0*/) : QGLWidget(pwgt)
{

}

void BubbleWindow::initializeGL()
{
    qglClearColor(QColor(113, 217, 135));
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

void BubbleWindow::DrawBubble(Bubble *bubble){
    QPoint* bubble_center_position = bubble->GetPosition();
    float* bubble_size = bubble->GetBubbleSize();
    GLfloat red_color_level = (float)bubble->GetColor()->red();
    GLfloat green_color_level = (float)bubble->GetColor()->green();
    GLfloat blue_color_level = (float)bubble->GetColor()->blue();

    const int smoothness = 272;
    const float doublePi = 2.0f * 3.1415927;

    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < smoothness; i++) {
            glColor3f(red_color_level, green_color_level, blue_color_level);
            glVertex2f(bubble_center_position->rx() + *bubble_size * cosf(doublePi * i / smoothness),
                       bubble_center_position->ry() + *bubble_size * sinf(doublePi * i / smoothness));
        }
   glEnd();
}

std::set<Bubble*>* BubbleWindow::GetBubbleSet() {
    return _bubble_set;
}

void BubbleWindow::SetBubbleSet(std::set<Bubble*>* bubble_set) {
    _bubble_set = bubble_set;
}
