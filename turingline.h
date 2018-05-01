#ifndef TURINGLINE_H
#define TURINGLINE_H

#include <QWidget>
#include <QTableWidget>
#include <QBoxLayout>
#include <QScrollArea>
#include <string>
#include <iostream>

class TuringLine : public QTableWidget
{
    Q_OBJECT
public:
    explicit TuringLine(QWidget *parent = nullptr);


    bool SetScrollArea(QScrollArea*);

 // go left at line [ ] <- [o]
    bool L();
 // go right at line [o] -> [ ]
    bool R();
 // stay at position [o]
    bool N();

    QTableWidgetItem* GetCurrentCell();
signals:

public slots:
 // buttons for adding and removing cells from turing line
    bool AddCellFromLeft();
    bool RemoveCellFromLeft();
    bool AddCellFromRight();
    bool RemoveCellFromRight();
private:
    QScrollArea* _line_sa;
    QTableWidget* _line;
    QTableWidgetItem* _current_cell;
    int _left_border = 0;
    int _right_border = 0;
    int _current_cell_num = 0;

 // select cell and colorize it
    bool SelectCell(int);
};

#endif // TURINGLINE_H
