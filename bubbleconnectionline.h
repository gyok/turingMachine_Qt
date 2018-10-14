#ifndef BUBBLECONNECTIONLINE_H
#define BUBBLECONNECTIONLINE_H

#include <QLineEdit>
#include <QComboBox>
#include "turingline.h"

class BubbleConnectionLine : public QObject
{
    Q_OBJECT
public:
    enum WayToMove{L = 0, R, N};
    static enum WayToMove wayToMove;
    BubbleConnectionLine(QWidget*, TuringLine*);
    ~BubbleConnectionLine();
    QLineEdit* GetSymbolBeforeLine();
    QLineEdit* GetSymbolAfterLine();
    QComboBox* GetComboBox();
    QString GetSelectedWay();
private:
    QLineEdit* _symbolBeforeLine;
    QLineEdit* _symbolAfterLine;
    QComboBox* _wayToMoveCombobox;
};

#endif // BUBBLECONNECTIONLINE_H
