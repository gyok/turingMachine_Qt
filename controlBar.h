#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QPushButton>

class ControlBar : public QObject
{
    Q_OBJECT
private:
    QPushButton* _run_button;
    QPushButton* _pause_button;
    QIcon* _start_img;
    QIcon* _stop_img;
    QIcon* _pause_active_img;
    QIcon* _pause_inactive_img;

    bool _run_button_activated;
    bool _pause_button_activated;
public:
    ControlBar(QWidget* parent);

    QPushButton* GetRunButton();
    QPushButton* GetPauseButton();
public slots:
    void RunButtonClicked();
    void PauseButtonClicked();
};

#endif // CONTROLBAR_H
