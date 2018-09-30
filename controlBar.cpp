#include "controlBar.h"

#define CONTROL_BUTTON_SIZE 25

ControlBar::ControlBar(QWidget* parent)
    : QObject(parent)
{
    _run_button = new QPushButton(parent);
    _pause_button = new QPushButton(parent);

    QString img_path = QString("../TuringMachine/img");
    _start_img = new QIcon(img_path + "/start.xpm");
    _stop_img = new QIcon(img_path + "/stop.xpm");
    _pause_active_img = new QIcon(img_path + "/pause_active.xpm");
    _pause_inactive_img = new QIcon(img_path + "/pause_inactive.xpm");

    _run_button->setIcon(*_start_img);
    _pause_button->setIcon(*_pause_active_img);

    _run_button->setIconSize(QSize(CONTROL_BUTTON_SIZE, CONTROL_BUTTON_SIZE));
    _pause_button->setIconSize(QSize(CONTROL_BUTTON_SIZE, CONTROL_BUTTON_SIZE));

    _run_button_activated = false;
    _pause_button_activated = false;
}

void ControlBar::RunButtonClicked() {
    _run_button_activated = !_run_button_activated;
    _run_button->setIcon(_run_button_activated
                            ? *_stop_img
                            : *_start_img);
}

void ControlBar::PauseButtonClicked() {
    _pause_button_activated = !_pause_button_activated;
    _pause_button->setIcon(_pause_button_activated
                            ? *_pause_inactive_img
                            : *_pause_active_img);
}

QPushButton* ControlBar::GetRunButton() {
    return _run_button;
}

QPushButton* ControlBar::GetPauseButton() {
    return _pause_button;
}
